// file scee.c
////////////////////////////////////////////////////////////////////////////////
// Description
// This application implements an execution environment in which signals are
// used to stop, resume, terminate, and get information from spawned child
// processes, while a list of them is maintained and updated.
//
// supported commands
// exec, execute a file given its path and required arguments.
// kill, terminate a spawned process given its PID.
// stop, stop the execution of a running spawned process given its PID.
// cont, resume the execution of a stopped spawned process given its PID.
// list, print a list of the running spawned processes.
// info, request information about a spawned process given it PID.
// quit, terminate the application.
//
// The commands can be requested using just the first letter of their name.
// Once compiled, the integers and times programs can be executed via their
// i and t symbolic links, respectively.
////////////////////////////////////////////////////////////////////////////////

// #include directives
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
////////////////////////////////////////////////////////////////////////////////

// #define directives
////////////////////////////////////////////////////////////////////////////////
#define MAX_INPUT_LENGTH 512
#define MAX_LINE_LENGTH 80
#define MAX_STATUS_LENGTH 16
#define MAX_TASK_LENGTH 4
#define MAX_PATH_LENGTH 512

// http://stackoverflow.com/a/3219471
// http://en.wikipedia.org/wiki/ANSI_escape_code
#define ANSI_RED "\x1b[31m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_RESET "\x1b[0m"

#define SLEEP_SECONDS 0
#define SLEEP_NANOSECONDS 100000000
        // 100,000,000 nanoseconds, equal to 0.1 seconds.
////////////////////////////////////////////////////////////////////////////////

// custom data types
////////////////////////////////////////////////////////////////////////////////
struct process_s {
    // This struct is a node of a doubly linked list. Its data is information
    // about a process.
    int pid;  // PID
    char *path;  // Path to the executable file.
    int stopped;  // Boolean indicator that the process has been stopped.
    struct process_s *next;  // The next node of the list.
    struct process_s *previous;  // The previous node of the list.
};
typedef struct process_s process_t;
////////////////////////////////////////////////////////////////////////////////

// global variable declaration
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// function prototypes
////////////////////////////////////////////////////////////////////////////////
int get_input(char **input, const int max_length);
int str_split(const char *str, char ***tokens, const char *delimiters);
void clear_screen();
int list_create(process_t **list);
int list_add(process_t *list, int pid, char *path);
int list_remove(process_t *node);
int list_print(process_t *list);
int list_search(process_t *list, process_t **result, int pid);
int parent_signal_handling();
int child_signal_handling();
int process_exec(process_t *processes, char *arguments[]);
int low_level_process_kill(process_t *process);
int process_kill(process_t *processes, char *string_pid);
int process_stop(process_t *processes, char *string_pid);
int process_cont(process_t *processes, char *string_pid);
int process_list(process_t *list);
int process_info(process_t *processes, char *string_pid);
int process_quit(process_t *list);
int task_queue();
////////////////////////////////////////////////////////////////////////////////

// functions
////////////////////////////////////////////////////////////////////////////////
int get_input(char **input, const int max_length) {
    // Description
    // This function reads a single line input from stdin. The input should
    // be at most of length max_length. It is stored in a dynamically allocated
    // string and a pointer to this string is stored in input. If the length of
    // the input exceeds max_length, the value of input is not changed.
    //
    // Returns
    // get_input returns the length of the input read which could be equal to 0,
    // -1 in case of failure, or if the length of the input exceeded max_length.

    // variable declaration
    char *temp_input;
    size_t buffer_size;
    int input_length;
    int return_value;  // integer placeholder for error checking
    void *return_pointer;  // pointer placeholder for error checking

    temp_input = NULL;
    buffer_size = 0;  // NOTE Is it a good practice to include a cast here?
            // "buffer_size = (size_t)0;"

    return_value = getline(&temp_input, &buffer_size, stdin);
    if (return_value == -1) {
        perror("error, getline");
        return -1;
    }

    // Store the input length.
    input_length = strlen(temp_input);

    // Verify that an entire line, terminating in a newline character, was read.
    if (temp_input[input_length - 1] != '\n') {
        printf("Error, getline didn't read an entire line.\n");

        free(temp_input);
        return -1;
    }

    // Remove the trailing newline character.
    temp_input[input_length - 1] = '\0';

    // Update the input length.
    input_length = strlen(temp_input);

    // The input length should be at most equal to max_length.
    if (input_length > max_length) {
        printf("Error, the input was too large.");
        printf(" Input length: %d", input_length);
        printf(" Maximum permitted length: %d\n", max_length);

        free(temp_input);
        return -1;
    }

    // Reallocate the temp_input char array so that its size is reduced by
    // one byte, previously occupied by the trailing newline character.
    return_pointer = (char *)realloc(temp_input,
            (size_t)((input_length + 1) * sizeof(*temp_input)));
    if (return_pointer == NULL) {
        perror("error, realloc");
        free(temp_input);
        return -1;
    } else {
        temp_input = return_pointer;
    }

    *input = temp_input;

    return input_length;
}

int str_split(const char *str, char ***tokens, const char *delimiters) {
    // Description
    // This function parses the string str, finds the substrings of it
    // being separated by one or more characters of the delimiters string,
    // stores them in dynamically allocated memory, and stores pointers to them
    // in the array tokens, which is also dynamically allocated. A terminating
    // NULL pointer is appended to tokens, cast to (char *).
    //
    // Returns
    // str_split returns the number of tokens found in str which could be
    // equal to 0, or -1 in case of failure.

    // variable declaration
    int num_elements;  // number of elements added to tokens
    char *str_copy;  // dynamically allocated copy of the string str
    char *token;  // pointer to token returned by strtok
    char **temp_tokens;
    void *return_pointer;  // pointer placeholder for error checking
    int i;  // generic counter

    // str and delimiters should point to char arrays.
    if ((str == NULL) || (delimiters == NULL)) {
        return -1;
    }

    // Copy the initial string to avoid its mutation.
    return_pointer = strdup(str);
    if (return_pointer == NULL) {
        perror("error, strdup");
        return -1;
    } else {
        str_copy = return_pointer;
    }

    // Find tokens and store them in a dynamically allocated strings.
    // Store pointers to these strings in the tokens array.
    num_elements = 0;
    temp_tokens = NULL;
    do {
        if (num_elements == 0) {
            // Initial call of strtok.
            token = strtok(str_copy, delimiters);
        } else {
            // Subsequent calls of strtok.
            token = strtok(NULL, delimiters);
        }

        // Allocate memory for a new element in temp_tokens.
        num_elements++;
        return_pointer = (char **)realloc(temp_tokens,
                (size_t)(num_elements * sizeof(char **)));
        if (return_pointer == NULL) {
            perror("error, realloc");

            // Memory deallocation.
            for (i = 0; temp_tokens[i] != NULL; i++) {
                free(temp_tokens[i]);
            }
            free(temp_tokens);

            return -1;
        } else {
            temp_tokens = return_pointer;
        }
        temp_tokens[num_elements - 1] = NULL;

        // Store the token found in a dynamically allocated string.
        if (token != NULL) {
            return_pointer = strdup(token);
            if (return_pointer == NULL) {
                perror("error, strdup");

                // Memory deallocation.
                for (i = 0; temp_tokens[i] != NULL; i++) {
                    free(temp_tokens[i]);
                }
                free(temp_tokens);

                return -1;
            } else {
                token = return_pointer;
            }
        }

        // Store a pointer to the token in the temp_tokens array.
        temp_tokens[num_elements - 1] = token;

    } while (temp_tokens[num_elements - 1] != NULL);

    *tokens = temp_tokens;

    return (num_elements - 1);
}

void clear_screen() {
    // Description
    // This function clears the terminal screen and places the cursor in
    // the position (1,1)
    // http://stackoverflow.com/q/1348563
    // http://stackoverflow.com/q/2347770
    //
    // Returns
    // clear_screen does not return any value.

    // variable declaration
    //const char cls_ansi_string[] = "\e[1;1H\e[2J";

    //printf("%s", cls_ansi_string);

    printf("\e[1;1H\e[2J");
}

int list_create(process_t **list) {
    // Description
    // This function creates a doubly linked list of information about processes
    // by creating and initializing its sentinel node. The address of
    // the sentinel is stored in list.
    //
    // Returns
    // list_create returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    process_t *sentinel;
    void *return_pointer;  // pointer placeholder for error checking

    return_pointer = (process_t *)malloc(1 * sizeof(process_t));
    if (return_pointer == NULL) {
        perror("error, malloc");
        return -1;
    } else {
        sentinel = return_pointer;
    }

    // Data initialization.
    sentinel->pid = 0;
    sentinel->path = NULL;
    sentinel->stopped = 0;

    // Pointer initialization.
    sentinel->next = sentinel;
    sentinel->previous = sentinel;

    *list = sentinel;

    return 0;
}

int list_add(process_t *list, int pid, char *path) {
    // Description
    // This function adds a node containing the data pid and path right after
    // the sentinel node of the list list.
    //
    // Returns
    // list_add returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    process_t *node;
    void *return_pointer;  // pointer placeholder for error checking

    if (list == NULL) {
        // list should point to a valid list.
        return -1;
    }

    // Create the new node.
    return_pointer = (process_t *)malloc(1 * sizeof(process_t));
    if (return_pointer == NULL) {
        perror("error, malloc");
        return -1;
    } else {
        node = return_pointer;
    }

    // Populate the node with data.
    node->pid = pid;
    return_pointer = strdup(path);
    if (return_pointer == NULL) {
        perror("error, strdup");
        return -1;
    } else {
        node->path = return_pointer;
    }
    node->stopped = 0;

    // Add the node to the list.
    node->next = list->next;
    node->previous = list;
    list->next->previous = node;
    list->next = node;

    return 0;
}

int list_remove(process_t *node) {
    // Description
    // This function removes the node node from the list it is residing and
    // subsequently deletes it.
    //
    // Returns
    // list_remove returns 0 on successful completion or -1 in case of failure.

    // variable declaration

    if ((node == NULL) || (node->next == NULL) || (node->previous == NULL)){
        // node should point to a valid node residing in a list.
        return -1;
    }

    // Remove the node from the list.
    node->next->previous = node->previous;
    node->previous->next = node->next;

    // Delete the node.
    free(node->path);
    free(node);

    return 0;
}

int list_print(process_t *list) {
    // Description
    // This function prints the contents of the nodes of the list list.
    //
    // Returns
    // list_print returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    process_t *node;
    int i;  // generic counter

    if (list == NULL) {
        // list should point to a valid list.
        return -1;
    }

    i = 0;
    for (node = list->next; node != list; node = node->next) {
        i++;
        printf("node: %d\n", i);
        printf("\tpid: %d\n", node->pid);
        printf("\tpath: %s\n", node->path);
        printf("\tstopped: %d\n", node->stopped);
        printf("\tnext: %p\n", node->next);
        printf("\tprevious: %p\n", node->previous);
    }

    return 0;
}

int list_search(process_t *list, process_t **result, int pid) {
    // Description
    // This function searches for a node with pid of pid in the list list.
    // It stores the address of the node found in result, or NULL, if no such
    // node was found.
    //
    // Returns
    // list_search returns 1 if a matching node was found in the list,
    // 0 if no matching node was found, or -1 in case of failure.

    // variable declaration
    process_t *node;

    if (list == NULL) {
        // list should point to a valid list.
        return -1;
    }

    // Store the query in the sentinel.
    list->pid = pid;

    for (node = list->next; node->pid != pid; node = node->next);

    if (node == list) {
        *result = NULL;
        return 0;
    } else {
        *result = node;
        return 1;
    }
}

//static void handler_sigint(int signal) {
//    // Description
//    // This function is the signal handler for SIGINT.
//    //
//    // Returns
//    // handler_sigint does not return any value.
//
//    // variable declaration
//
//    write(STDOUT_FILENO, "\nI can't let you do that, Dave.\n\n", 32);
//}

int parent_signal_handling() {
    // Description
    // This function contains the signal handling code of the parent process
    // of the application. It blocks all signals, except for SIGCHLD.
    // // TODO and SIGINT.
    //
    // Returns
    // parent_signal_handling returns 0 on successful completion or
    // -1 in case of failure.

    // variable declaration
    //struct sigaction action = { {0} };
    struct sigaction action_sigchld = { {0} };
    sigset_t signals_set;
    int return_value;  // integer placeholder for error checking

    return_value = sigfillset(&signals_set);
    if (return_value == -1) {
        perror("error, sigfillset");
        return -1;
    }

    //return_value = sigdelset(&signals_set, SIGINT);
    //if (return_value == -1) {
    //    perror("error, sigdelset");
    //    return -1;
    //}

    return_value = sigdelset(&signals_set, SIGCHLD);
    if (return_value == -1) {
        perror("error, sigdelset");
        return -1;
    }

    return_value = sigprocmask(SIG_BLOCK, &signals_set, NULL);
    if (return_value == -1) {
        perror("error, sigprocmask");
        return -1;
    }

    // TODO Results to an error in getline.
    //action.sa_handler = handler_sigint;
    //return_value = sigaction(SIGINT, &action, NULL);
    //if (return_value == -1) {
    //    perror("error, sigaction");
    //    return -1;
    //}

    // Stop terminated child processes from becoming defunct.
    // Improved implementation of http://stackoverflow.com/a/6718997
    action_sigchld.sa_handler = SIG_IGN;
    return_value = sigaction(SIGCHLD, &action_sigchld, NULL);
    if (return_value == -1) {
        perror("error, sigaction");
        return -1;
    }

    return 0;
}

int child_signal_handling() {
    // Description
    // This function contains the signal handling code of the child processes
    // of the application. It unblocks the signals SIGTERM, SIGSTOP, SIGCONT,
    // and SIGUSR1.
    //
    // Returns
    // child_signal_handling returns 0 on successful completion or
    // -1 in case of failure.

    // variable declaration
    sigset_t signals_set;
    int return_value;  // integer placeholder for error checking

    return_value = sigemptyset(&signals_set);
    if (return_value == -1) {
        perror("error, sigemptyset");
        return -1;
    }

    return_value = sigaddset(&signals_set, SIGTERM);
    if (return_value == -1) {
        perror("error, sigaddset");
        return -1;
    }
    return_value = sigaddset(&signals_set, SIGSTOP);
    if (return_value == -1) {
        perror("error, sigaddset");
        return -1;
    }
    return_value = sigaddset(&signals_set, SIGCONT);
    if (return_value == -1) {
        perror("error, sigaddset");
        return -1;
    }
    return_value = sigaddset(&signals_set, SIGUSR1);
    if (return_value == -1) {
        perror("error, sigaddset");
        return -1;
    }

    return_value = sigprocmask(SIG_UNBLOCK, &signals_set, NULL);
    if (return_value == -1) {
        perror("error, sigprocmask");
        return -1;
    }

    return 0;
}

int process_exec(process_t *processes, char *arguments[]) {
    // Description
    // This function spawns a new process of the executable file specified in
    // the path which is the first element of arguments and adds a node
    // containing information about it in processes.
    //
    // Returns
    // process_exec returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    int pid; // TODO Should it be pid_t instead?
    char path[MAX_PATH_LENGTH + 1];
    struct timespec sleep_time;
    int return_value;  // integer placeholder for error checking

    // process_exec requires a valid path.
    if (arguments[0] == NULL) {
        printf("error, " ANSI_BOLD "exec" ANSI_RESET " requires a valid PATH\n");
        return 0;
    }

    sleep_time.tv_sec = SLEEP_SECONDS;
    sleep_time.tv_nsec = (long)SLEEP_NANOSECONDS;

    // Store the path to the executable file.
    strcpy(path, arguments[0]);

    // fork
    return_value = (int)fork();
    if (return_value == -1) {
        perror("error, fork");
        return -1;
    } else if (return_value == 0) {
        // child code
        return_value = child_signal_handling();
        if (return_value == -1) {
            printf("error, child_signal_handling\n");
        }

        return_value = execv(path, arguments);
        if (return_value == -1) {
            perror("error, execv");
            exit(-1);
        }
    } else {
        // parent code
        pid = return_value;

        // Wait a short period of time before you check whether the child is
        // still running. This could fail if the access to the file is too slow
        // and, conceivably, for other reasons.
        return_value = nanosleep(&sleep_time, NULL);
        if (return_value == -1) {
            perror("error, nanosleep");
            return -1;
        }

        // Perform error checking for sending a signal to the child process.
        // http://stackoverflow.com/q/5460702
        return_value = kill(pid, 0);
        if (return_value == 0) {
            // The process exists, the file probably executed successfully.

            return_value = list_add(processes, pid, path);
            if (return_value == -1) {
                printf("error, list_add\n");
            }

            printf("a process with PID %d was spawned\n", pid);
        } else if (errno == ESRCH) {
            printf("error in executing the program, or the program exited");
            printf(" instantly; nothing was added to the process list\n");
        } else {
            perror("error, kill");
            return -1;
        }
    }

    return 0;
}

int low_level_process_kill(process_t *process) {
    // Description
    // This function kills the process process and removes its node from
    // the list it resides.
    //
    // Returns
    // low_level_process_kill returns 0 on successful completion
    // or -1 in case of failure.

    // variable declaration
    int return_value;  // integer placeholder for error checking

    // kill the process.
    return_value = kill((pid_t)process->pid, SIGTERM);
    if (return_value == -1) {
        perror("error, kill");
        return -1;
    }

    // TODO
    // I would like a check here of whether the process exists after several
    // specified periods of time. If it does, it could be killed with
    // kill((pid_t)process->pid, SIGKILL);

    // Remove its node from the list.
    return_value = list_remove(process);
    if (return_value == -1) {
        printf("error, list_remove\n");
        return -1;
    }

    return 0;
}

int process_kill(process_t *processes, char *string_pid) {
    // Description
    // This function kills the process with PID equal to pid, provided it
    // exists in the list processes, and removes its node from it.
    //
    // Returns
    // process_kill returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    int pid;
    process_t *result;
    int return_value;  // integer placeholder for error checking

    // process_kill requires a valid PID.
    if (string_pid == NULL) {
        printf("error, " ANSI_BOLD "kill" ANSI_RESET " requires a valid PID\n");
        return 0;
    }

    // Store the PID as an integer.
    pid = atoi(string_pid);

    // Search in list processes for a process with PID equal to pid.
    return_value = list_search(processes, &result, pid);
    if (return_value == -1) {
        printf("error, list_search\n");
        return -1;
    } else if (return_value == 0) {
        printf("no process with PID %d\n", pid);
    } else {
        return_value = low_level_process_kill(result);
        if (return_value == -1) {
            printf("error, low_level_process_kill\n");
            return -1;
        } else {
            printf("the process with PID %d was killed\n", pid);
        }
    }

    return 0;
}

int process_stop(process_t *processes, char *string_pid) {
    // Description
    // This function stops the process with PID equal to pid, provided it
    // exists in the list processes and isn't already stopped, and sets its
    // stopped status to 1.
    //
    // Returns
    // process_stop returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    int pid;
    process_t *result;
    process_t *process;
    int return_value;  // integer placeholder for error checking

    // process_stop requires a valid PID.
    if (string_pid == NULL) {
        printf("error, " ANSI_BOLD "stop" ANSI_RESET " requires a valid PID\n");
        return 0;
    }

    // Store the PID as an integer.
    pid = atoi(string_pid);

    // Search in list processes for a process with PID equal to pid.
    return_value = list_search(processes, &result, pid);
    if (return_value == -1) {
        printf("error, list_search\n");
        return -1;
    } else if (return_value == 0) {
        printf("no process with PID %d\n", pid);
    } else {
        process = result;
        if (!(process->stopped)) {
            // Stop the process.
            return_value = kill((pid_t)pid, SIGSTOP);
            if (return_value == -1) {
                perror("error, kill");
                return -1;
            }

            // Set the stopped status of the process node to 1.
            process->stopped = 1;

            printf("the process with PID %d was stopped\n", pid);
        } else {
            printf("the process with PID %d is already stopped\n", pid);
        }
    }

    return 0;
}

int process_cont(process_t *processes, char *string_pid) {
    // Description
    // This function resumes the process with PID equal to pid, provided it
    // exists in the list processes and isn't already running, and sets its
    // stopped status to 0.
    //
    // Returns
    // process_cont returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    int pid;
    process_t *result;
    process_t *process;
    int return_value;  // integer placeholder for error checking

    // process_cont requires a valid PID.
    if (string_pid == NULL) {
        printf("error, " ANSI_BOLD "cont" ANSI_RESET " requires a valid PID\n");
        return 0;
    }

    // Store the PID as an integer.
    pid = atoi(string_pid);

    // Search in list processes for a process with PID equal to pid.
    return_value = list_search(processes, &result, pid);
    if (return_value == -1) {
        printf("error, list_search\n");
        return -1;
    } else if (return_value == 0) {
        printf("no process with PID %d\n", pid);
    } else {
        process = result;
        if (process->stopped) {
            // Resume the process.
            return_value = kill((pid_t)pid, SIGCONT);
            if (return_value == -1) {
                perror("error, kill");
                return -1;
            }

            // Set the stopped status of the process node to 0.
            process->stopped = 0;

            printf("the process with PID %d was resumed\n", pid);
        } else {
            printf("the process with PID %d is already running\n", pid);
        }
    }

    return 0;
}

int process_list(process_t *list) {
    // Description
    // This function prints a table with information about the spawned running
    // processes. It contains the processes PID, their status, and the path
    // used to execute the program. The obsolete entries of the processes
    // no longer existing are removed.
    //
    // Returns
    // process_list returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    process_t *node;
    char status[MAX_STATUS_LENGTH + 1];
    int num_removed;  // The number of obsolete entries removed from the list.
    int return_value;  // integer placeholder for error checking

    if (list == NULL) {
        // list should point to a valid list.
        return -1;
    }

    // Check all entries of whether they are still current.
    num_removed = 0;
    for (node = list->next; node != list; node = node->next) {
        // Perform error checking for sending a signal to the child process.
        // http://stackoverflow.com/q/5460702
        return_value = kill(node->pid, 0);
        if (return_value == 0) {
            // The process still exists.
        } else if (errno == ESRCH) {
            // The process doesn't exist.

            // Remove its node from the list.
            return_value = list_remove(node);
            if (return_value == -1) {
                printf("error, list_remove\n");
                return -1;
            }

            num_removed++;
        } else {
            perror("error, kill");
            return -1;
        }
    }

    printf("\n");

    // top border
    printf("+-------------------");
    printf("------------------------------------------------------------");
    printf("\n");

    // title
    printf("|  spawned running processes\n");

    // separator
    printf("+-------+----------+");
    printf("------------------------------------------------------------");
    printf("\n");

    // table headers
    printf("%c", '|');
    printf("  PID  ");
    printf("%c", '|');
    printf("  status  ");
    printf("%c", '|');
    printf("  path  ");
    printf("\n");

    // separator
    printf("+-------+----------+");
    printf("------------------------------------------------------------");
    printf("\n");

    if (list->next == list) {
        // empty entry
        printf("|       |          |");
        printf("        ");
        printf("\n");
    }

    // process data
    for (node = list->next; node != list; node = node->next) {
        printf("%c", '|');
        printf(" %5d ", node->pid);
        printf("%c", '|');
        if (node->stopped == 0) {
            strcpy(status, "running");
        } else {
            strcpy(status, "stopped");
        }
        printf("  %s ", status);
        printf("%c", '|');
        printf(" %s ", node->path);
        printf("\n");
    }

    // bottom border
    printf("+-------+----------+");
    printf("------------------------------------------------------------");
    printf("\n");

    if (num_removed == 1) {
        printf("\n1 obsolete process entry was removed\n\n");
    } else if (num_removed > 1) {
        printf("\n%d obsolete process entries were removed\n\n", num_removed);
    }

    return 0;
}

int process_info(process_t *processes, char *string_pid) {
    // Description
    // This function sends the signal SIGUSR1 to the process with PID equal to
    // pid, provided it exists in the list processes. It subsequently prints
    // TODO information about the process.
    //
    // NOTE
    // I'd prefer to implement this with the child sending back the signal
    // SIGCHLD and having a handler for it in the parent. SIGCHLD fills in
    // some elements of the sigaction struct providing information about
    // the child.
    //
    // Returns
    // process_info returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    int pid;
    process_t *result;
    int return_value;  // integer placeholder for error checking

    // process_stop requires a valid PID.
    if (string_pid == NULL) {
        printf("error, " ANSI_BOLD "info" ANSI_RESET " requires a valid PID\n");
        return 0;
    }

    // Store the PID as an integer.
    pid = atoi(string_pid);

    // Search in list processes for a process with PID equal to pid.
    return_value = list_search(processes, &result, pid);
    if (return_value == -1) {
        printf("error, list_search\n");
        return -1;
    } else if (return_value == 0) {
        printf("no process with PID %d\n", pid);
    } else {
        // Send the signal SIGUSR1 to the process.
        return_value = kill((pid_t)pid, SIGUSR1);
        if (return_value == -1) {
            perror("error, kill");
            return -1;
        }
    }

    return 0;
}

int process_quit(process_t *list) {
    // Description
    // This function kills all spawned processes that are still running.
    //
    // TODO There is no check here whether they are still running.
    // This could be implemented by making a function, maybe named
    // process_check, that does this check, that process_list could also use.
    //
    // Returns
    // process_quit returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    process_t *node;
    int return_value;  // integer placeholder for error checking

    // kill all processes.
    for (node = list->next; node != list; node = node->next) {
        return_value = low_level_process_kill(node);
        if (return_value == -1) {
            printf("error, low_level_process_kill\n");
            return -1;
        }
    }

    return 0;
}

int task_queue() {
    // Description
    // This function sets up the execution environment, prints the interface
    // of the application, and executes the tasks requested by the user.
    //
    // Returns
    // task_queue returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    process_t *processes;
    char *raw_input;
    char **input;
    char task[MAX_INPUT_LENGTH + 1];
    const char space_tab[] = " \t";
    int return_value;  // integer placeholder for error checking
    int i;  // generic counter

    // Clear the screen.
    clear_screen();

    // Print a welcome message.
    printf(ANSI_BOLD "SCEE" ANSI_RESET " - ");
    printf(ANSI_RED "Signal Controlled Execution Environment" ANSI_RESET);
    printf("\n");

    // Create the processes list.
    processes = NULL;
    return_value = list_create(&processes);
    if (return_value == -1) {
        printf("error, list_create\n");
        return -1;
    }

    strcpy(task, "");
    while (strcmp(task, "quit") && strcmp(task, "q")) {
        // Print available commands.
        printf("\nCOMMANDS\n");
        printf("    " ANSI_BOLD "exec" ANSI_RESET " <PATH> [arg1] [arg2] ...\n");
        printf("    " ANSI_BOLD "kill" ANSI_RESET " <PID>\n");
        printf("    " ANSI_BOLD "stop" ANSI_RESET " <PID>\n");
        printf("    " ANSI_BOLD "cont" ANSI_RESET " <PID>\n");
        printf("    " ANSI_BOLD "list" ANSI_RESET "\n");
        printf("    " ANSI_BOLD "info" ANSI_RESET " <PID>\n");
        printf("    " ANSI_BOLD "quit" ANSI_RESET "\n");
        printf("> ");

        // Get a command.
        raw_input = NULL;
        return_value = get_input(&raw_input, MAX_INPUT_LENGTH);
        if (return_value == -1) {
            printf("error, get_input\n");
        }

        input = NULL;
        return_value = str_split(raw_input, &input, space_tab);
        if (return_value == -1) {
            printf("error, str_split\n");
        }

        if (strlen(raw_input) != 0) {
            strcpy(task, input[0]);
        } else {
            strcpy(task, "");
        }

        // Execute the command.
        if (!strcmp(task, "exec") || !strcmp(task, "e")) {
            process_exec(processes, &input[1]);
        } else if (!strcmp(task, "kill") || !strcmp(task, "k")) {
            process_kill(processes, input[1]);
        } else if (!strcmp(task, "stop") || !strcmp(task, "s")) {
            process_stop(processes, input[1]);
        } else if (!strcmp(task, "cont") || !strcmp(task, "c")) {
            process_cont(processes, input[1]);
        } else if (!strcmp(task, "list") || !strcmp(task, "l")) {
            process_list(processes);
        } else if (!strcmp(task, "info") || !strcmp(task, "i")) {
            process_info(processes, input[1]);
        } else if (!strcmp(task, "quit") || !strcmp(task, "q")) {
            process_quit(processes);
        } else if (!strcmp(task, "")) {
        } else {
            printf("invalid command\n");
        }

        // Memory deallocation.
        free(raw_input);
        for (i = 0; input[i] != NULL; i++) {
            free(input[i]);
        }
        free(input);
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////

// tests
////////////////////////////////////////////////////////////////////////////////
int test_str_split() {
    // Description
    // This function tests the str_split function.
    //
    // Returns
    // test_str_split returns 0 on successful completion of all tests or
    // -1 in case of any test or itself failing.

    // variable declaration
    char *string;
    char **tokens;
    char *delimiters;
    char **expected_tokens;
    const char space[] = " ";
    //const char tab[] = "\t";  // not used yet
    const char space_tab[] = " \t";
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking
    void *return_pointer;  // pointer placeholder for error checking
    int i;  // generic counter

    printf("testing str_split\n");

    num_tests = 0;
    num_passed = 0;

    // test 01
    num_tests++;
    failed = 0;

    return_pointer = strdup("a");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO_PRIME free memory
        return -1;
    } else {
        string = return_pointer;
    }

    return_pointer = strdup(space_tab);
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO_PRIME free memory
        return -1;
    } else {
        delimiters = return_pointer;
    }

    expected_tokens = NULL;
    return_pointer = (char **)realloc(expected_tokens,
            (size_t)(1 * sizeof(*expected_tokens)));
    if (return_pointer == NULL) {
        perror("error, realloc");
        // TODO_PRIME free memory
        return -1;
    } else {
        expected_tokens = return_pointer;
    }

    expected_tokens[0] = NULL;
    return_pointer = strdup("a");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO_PRIME free memory
        return -1;
    } else {
        expected_tokens[0] = return_pointer;
    }

    tokens = NULL;
    return_value = str_split(string, &tokens, delimiters);
    if (return_value != -1) {
        for (i = 0; tokens[i] != NULL; i++) {
            if (strcmp(tokens[i], expected_tokens[i])) {
                failed = 1;
            }
        }
        if (!failed) {
            num_passed++;
        }
    }
    // TODO_PRIME free memory

    // test 02
    num_tests++;
    failed = 0;

    return_pointer = strdup("a b c");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO_PRIME free memory
        return -1;
    } else {
        string = return_pointer;
    }

    return_pointer = strdup(space);
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO_PRIME free memory
        return -1;
    } else {
        delimiters = return_pointer;
    }

    expected_tokens = NULL;
    return_pointer = (char **)realloc(expected_tokens,
            (size_t)(3 * sizeof(*expected_tokens)));
    if (return_pointer == NULL) {
        perror("error, realloc");
        // TODO_PRIME free memory
        return -1;
    } else {
        expected_tokens = return_pointer;
    }

    expected_tokens[0] = NULL;
    return_pointer = strdup("a");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO_PRIME free memory
        return -1;
    } else {
        expected_tokens[0] = return_pointer;
    }

    expected_tokens[1] = NULL;
    return_pointer = strdup("b");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO_PRIME free memory
        return -1;
    } else {
        expected_tokens[1] = return_pointer;
    }

    expected_tokens[2] = NULL;
    return_pointer = strdup("c");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO_PRIME free memory
        return -1;
    } else {
        expected_tokens[2] = return_pointer;
    }

    tokens = NULL;
    return_value = str_split(string, &tokens, delimiters);
    if (return_value != -1) {
        for (i = 0; tokens[i] != NULL; i++) {
            if (strcmp(tokens[i], expected_tokens[i])) {
                failed = 1;
            }
        }
        if (!failed) {
            num_passed++;
        }
    }
    // TODO_PRIME free memory

    if (num_passed == num_tests) {
        printf("\tall tests passed\n");
        return 0;
    } else {
        printf("\tat least one test failed\n");
        return -1;
    }
}

int test_list_create() {
    // Description
    // This function tests the list_create function.
    //
    // Returns
    // test_list_create returns 0 on successful completion of all tests or
    // -1 in case of any test or itself failing.

    // variable declaration
    process_t *list;
    process_t expected_list;
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking

    printf("testing list_create\n");

    num_tests = 0;
    num_passed = 0;

    // test 01
    num_tests++;
    failed = 0;

    // expected_list initialization.
    expected_list.pid = 0;
    expected_list.path = NULL;
    expected_list.stopped = 0;
    expected_list.next = NULL;
    expected_list.previous = NULL;

    list = NULL;
    return_value = list_create(&list);
    if (return_value != -1) {
        if ((list == NULL) ||
                (list->pid != expected_list.pid) ||
                (list->path != expected_list.path) ||
                (list->stopped != expected_list.stopped) ||
                (list->next != list) ||
                (list->previous != list)) {
            failed = 1;
        }
        if (!failed) {
            num_passed++;
        }
    }
    // TODO_PRIME free memory

    if (num_passed == num_tests) {
        printf("\tall tests passed\n");
        return 0;
    } else {
        printf("\tat least one test failed\n");
        return -1;
    }
}

int test_list_add() {
    // Description
    // This function tests the list_add function.
    //
    // Returns
    // test_list_add returns 0 on successful completion of all tests or
    // -1 in case of any test or itself failing.

    // variable declaration
    process_t *list;
    int pid;
    char path[MAX_PATH_LENGTH + 1];
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking

    printf("testing list_add\n");

    num_tests = 0;
    num_passed = 0;

    // test 01
    num_tests++;
    failed = 0;

    list = NULL;
    return_value = list_create(&list);
    if (return_value == -1) {
        printf("error, list_create\n");
        return -1;
    }

    pid = 1;
    strcpy(path, "./program");
    return_value = list_add(list, pid, path);
    if (return_value != -1) {
        if ((list->next->pid != pid) ||
                (strcmp(list->next->path, path)) ||
                (list->next->stopped != 0) ||
                (list->next->next != list) ||
                (list->next->previous != list)) {
            failed = 1;
        }
        if ((list->previous->pid != pid) ||
                (strcmp(list->previous->path, path)) ||
                (list->previous->stopped != 0) ||
                (list->previous->next != list) ||
                (list->previous->previous != list)) {
            failed = 1;
        }
        if (!failed) {
            num_passed++;
        }
    }
    // TODO_PRIME free memory

    if (num_passed == num_tests) {
        printf("\tall tests passed\n");
        return 0;
    } else {
        printf("\tat least one test failed\n");
        return -1;
    }
}

int test_list_remove() {
    // Description
    // This function tests the list_remove function.
    //
    // Returns
    // test_list_remove returns 0 on successful completion of all tests or
    // -1 in case of any test or itself failing.

    // variable declaration
    process_t *list;
    process_t *result;
    int pid;
    char path[MAX_PATH_LENGTH + 1];
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking

    printf("testing list_remove\n");

    num_tests = 0;
    num_passed = 0;

    list = NULL;
    return_value = list_create(&list);
    if (return_value == -1) {
        printf("error, list_create\n");
        return -1;
    }

    pid = 1;
    strcpy(path, "./program");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 10;
    strcpy(path, "./program_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 100;
    strcpy(path, "./program_double_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 1000;
    strcpy(path, "./program_triple_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    // test 01
    num_tests++;
    failed = 0;

    pid = 1;
    return_value = list_search(list, &result, pid);
    if (return_value == -1) {
        printf("error, list_search\n");
    }
    return_value = list_remove(result);
    if (return_value != -1) {
        return_value = list_search(list, &result, pid);
        if (return_value == -1) {
            printf("error, list_search\n");
        }
        if (return_value != 0) {
            failed = 1;
        }
    } else {
        failed = 1;
    }
    if (!failed) {
        num_passed++;
    }

    // TODO_PRIME free memory

    if (num_passed == num_tests) {
        printf("\tall tests passed\n");
        return 0;
    } else {
        printf("\tat least one test failed\n");
        return -1;
    }
}

int test_list_print() {
    // Description
    // This function tests the list_print function.
    //
    // Returns
    // test_list_print returns 0 on successful completion of all tests or
    // -1 in case of any test or itself failing.

    // variable declaration
    process_t *list;
    int pid;
    char path[MAX_PATH_LENGTH + 1];
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking

    printf("testing list_print\n");

    num_tests = 0;
    num_passed = 0;

    // test 01
    num_tests++;
    failed = 0;

    list = NULL;
    return_value = list_create(&list);
    if (return_value == -1) {
        printf("error, list_create\n");
        return -1;
    }

    pid = 1;
    strcpy(path, "./program");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 10;
    strcpy(path, "./program_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 100;
    strcpy(path, "./program_double_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 1000;
    strcpy(path, "./program_triple_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    return_value = list_print(list);
    if (return_value == -1) {
        failed = 1;
    }
    // TODO_PRIME free memory

    if (!failed) {
        num_passed++;
    }

    if (num_passed == num_tests) {
        printf("\tall tests passed\n");
        return 0;
    } else {
        printf("\tat least one test failed\n");
        return -1;
    }
}

int test_list_search() {
    // Description
    // This function tests the list_search function.
    //
    // Returns
    // test_list_search returns 0 on successful completion of all tests or
    // -1 in case of any test or itself failing.

    // variable declaration
    process_t *list;
    process_t *result;
    int pid;
    char path[MAX_PATH_LENGTH + 1];
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking

    printf("testing list_search\n");

    num_tests = 0;
    num_passed = 0;

    list = NULL;
    return_value = list_create(&list);
    if (return_value == -1) {
        printf("error, list_create\n");
        return -1;
    }

    pid = 1;
    strcpy(path, "./program");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 10;
    strcpy(path, "./program_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 100;
    strcpy(path, "./program_double_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 1000;
    strcpy(path, "./program_triple_prime");
    return_value = list_add(list, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    // test 01
    num_tests++;
    failed = 0;

    pid = 1;
    return_value = list_search(list, &result, pid);
    if (return_value == 1) {
        if ((result == NULL) || (result->pid != pid)) {
            failed = 1;
        }
    } else {
        failed = 1;
    }
    if (!failed) {
        num_passed++;
    }

    // test 02
    num_tests++;
    failed = 0;

    pid = 100;
    return_value = list_search(list, &result, pid);
    if (return_value == 1) {
        if ((result == NULL) || (result->pid != pid)) {
            failed = 1;
        }
    } else {
        failed = 1;
    }
    if (!failed) {
        num_passed++;
    }

    // test 03
    num_tests++;
    failed = 0;

    pid = 4;
    return_value = list_search(list, &result, pid);
    if (return_value != 0) {
        failed = 1;
    } else {
        if (result != NULL) {
            failed = 1;
        }
    }
    if (!failed) {
        num_passed++;
    }

    // test 04
    num_tests++;
    failed = 0;

    pid = 111;
    return_value = list_search(list, &result, pid);
    if (return_value != 0) {
        failed = 1;
    } else {
        if (result != NULL) {
            failed = 1;
        }
    }
    if (!failed) {
        num_passed++;
    }

    // TODO_PRIME free memory

    if (num_passed == num_tests) {
        printf("\tall tests passed\n");
        return 0;
    } else {
        printf("\tat least one test failed\n");
        return -1;
    }
}

int test_process_list() {
    // Description
    // This function tests the process_list function.
    //
    // Returns
    // test_process_list returns 0 on successful completion of all tests or
    // -1 in case of any test or itself failing.

    // variable declaration
    process_t *processes;
    int pid;
    char path[MAX_PATH_LENGTH + 1];
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking

    printf("testing process_list\n");

    num_tests = 0;
    num_passed = 0;

    // test 01
    num_tests++;
    failed = 0;

    // Create the processes list.
    processes = NULL;
    return_value = list_create(&processes);
    if (return_value == -1) {
        printf("error, list_create\n");
        return -1;
    }

    pid = 1;
    strcpy(path, "./program");
    return_value = list_add(processes, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    pid = 1000;
    strcpy(path, "./program_prime");
    return_value = list_add(processes, pid, path);
    if (return_value == -1) {
        printf("error, list_add\n");
    }

    return_value = process_list(processes);
    if (return_value == -1) {
        failed = 1;
    }
    // TODO_PRIME free memory

    if (!failed) {
        num_passed++;
    }

    if (num_passed == num_tests) {
        printf("\tall tests passed\n");
        return 0;
    } else {
        printf("\tat least one test failed\n");
        return -1;
    }
}

int test_all() {
    // Description
    // This function calls all the test functions of this program.
    //
    // Returns
    // test_all returns 0 on successful completion of all test functions or
    // -1 in case of any test function or itself failing.

    // variable declaration
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int return_value;  // integer placeholder for error checking

    num_tests = 0;
    num_passed = 0;

    // test_str_split
    num_tests++;
    return_value = test_str_split();
    if (return_value == 0) {
        num_passed++;
    }

    // test_list_create
    num_tests++;
    return_value = test_list_create();
    if (return_value == 0) {
        num_passed++;
    }

    // test_list_add
    num_tests++;
    return_value = test_list_add();
    if (return_value == 0) {
        num_passed++;
    }

    // test_list_remove
    num_tests++;
    return_value = test_list_remove();
    if (return_value == 0) {
        num_passed++;
    }

    // test_list_print
    num_tests++;
    return_value = test_list_print();
    if (return_value == 0) {
        num_passed++;
    }

    // test_list_search
    num_tests++;
    return_value = test_list_search();
    if (return_value == 0) {
        num_passed++;
    }

    //// test_process_list
    //num_tests++;
    //return_value = test_process_list();
    //if (return_value == 0) {
    //    num_passed++;
    //}

    if (num_passed == num_tests) {
        printf("\n");
        printf("all tests passed\n");
        return 0;
    } else {
        printf("\n");
        printf("at least one test failed\n");
        return -1;
    }
}
////////////////////////////////////////////////////////////////////////////////

// main function
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    // Description
    // The main function acts as a wrapper of the functions that implement
    // the application functionality.
    //
    // Returns
    // main returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    int return_value;  // integer placeholder for error checking

    /*test_str_split();*/

    /*test_list_create();*/

    /*test_list_add();*/

    /*test_list_remove();*/

    /*test_list_print();*/

    /*test_list_search();*/

    /*test_process_list();*/

    /*test_all();*/

    return_value = parent_signal_handling();
    if (return_value == -1) {
        printf("error, parent_signal_handling\n");
    }

    return_value = task_queue();
    if (return_value == -1) {
        printf("error, task_queue\n");
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
