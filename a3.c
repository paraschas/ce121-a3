// file a3.c
////////////////////////////////////////////////////////////////////////////////
// Description
// TODO
//
// TODO I'd like to implement manipulation functions for a generic list.
// The list should be doubly linked and have a sentinel node.
////////////////////////////////////////////////////////////////////////////////

// #include directives
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////

// #define directives
////////////////////////////////////////////////////////////////////////////////
#define MAX_TASK_LENGTH 4
#define MAX_INPUT_LENGTH 512

// http://stackoverflow.com/a/3219471
// http://en.wikipedia.org/wiki/ANSI_escape_code
#define ANSI_RED "\x1b[31m"
#define ANSI_BOLD "\x1b[1m"
#define ANSI_RESET "\x1b[0m"
////////////////////////////////////////////////////////////////////////////////

// custom data types
////////////////////////////////////////////////////////////////////////////////
struct process_s {
    // This struct is a node of a doubly linked list. Its data is information
    // about a process.
    int pid;  // PID
    char *path;  // Path to the executable file.
    /*char *name;  // Executable file name. TODO requires string manipulation*/
    int stopped;  // Boolean indicator that the process has been stopped.
    struct process_s *next;  // The next node of the list.
    struct process_s *prev;  // The previous node of the list.
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
    //const char cls_ansi_string[] = "\e[1;1H\e[2J";  // TODO The pointer asterisk
            // was replaced by square brackets. Verify their equivalence.

    //printf("%s", cls_ansi_string);

    printf("\e[1;1H\e[2J");
}

int list_create(process_t *list) {
    // Description
    // This function initializes a doubly linked list. The sentinel node of
    // the list contains the data pointed by data.
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
    sentinel->prev = sentinel;

    list = sentinel;

    return 0;
}

// struct process_s {
//     // This struct is a node of a doubly linked list. Its data is information
//     // about a process.
//     int pid;  // PID
//     char *path;  // Path to the executable file.
//     /*char *name;  // Executable file name. TODO requires string manipulation*/
//     int stopped;  // Boolean indicator that the process has been stopped.
//     struct process_s *next;  // The next node of the list.
//     struct process_s *prev;  // The previous node of the list.
// };
// typedef struct process_s process_t;

// TODO Implement list manipulation functions. Some tasks that come to mind:
// create, search, add element, remove element. The list should include
// a sentinel.

int process_exec(process_t *processes, char *arguments[]) {
    // Description
    // This function spawns a new process of the executable file specified in
    // the path which is the first element of arguments and adds a node
    // containing information about it in processes.
    //
    // Returns
    // process_exec returns 0 on successful completion or -1 in case of failure.

    // variable declaration

    printf("process_exec called\n");
    // TODO Create a new process.

    return 0;
}

int process_kill() {
    // Description
    // This function TODO
    //
    // Returns
    // process_kill returns TODO

    // variable declaration

    printf("process_kill called\n");
    // TODO Send a signal to kill a process.
    return 0;
}

int process_stop() {
    // Description
    // This function TODO
    //
    // Returns
    // process_stop returns TODO

    // variable declaration

    printf("process_stop called\n");
    // TODO Send a signal to stop a running process.
    return 0;
}

int process_cont() {
    // Description
    // This function TODO
    //
    // Returns
    // process_cont returns TODO

    // variable declaration

    printf("process_cont called\n");
    // TODO Send a signal to resume a stopped process.
    return 0;
}

int process_list() {
    // Description
    // This function TODO
    //
    // Returns
    // process_list returns TODO

    // variable declaration

    printf("process_list called\n");
    // TODO printf a list of the processes, including for each process its PID,
    // the path used to run the program (even better the stripped name of it),
    // and its status.
    return 0;
}

int process_info() {
    // Description
    // This function TODO
    //
    // Returns
    // process_info returns TODO

    // variable declaration

    printf("process_info called\n");
    // TODO printf information about the process.
    return 0;
}

int process_quit() {
    // Description
    // This function TODO
    //
    // Returns
    // process_quit returns TODO

    // variable declaration

    printf("process_quit called\n");
    // TODO kill all processes.
    return 0;
}

int task_queue() {
    // Description
    // This function TODO
    //
    // Returns
    // task_queue returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    char *raw_input;
    char **input;
    char task[MAX_INPUT_LENGTH + 1];
    const char space_tab[] = " \t";
    int return_value;  // integer placeholder for error checking
    int i;  // generic counter

    process_t *processes;

    // TODO Clear the screen.
    /*clear_screen();*/

    // Print a welcome message.
    printf(ANSI_BOLD "SCEE" ANSI_RESET " - ");
    printf(ANSI_RED "Signal Controlled Execution Environment" ANSI_RESET);
    printf("\n");

    // TODO Initialize the processes list.
    // NEXT Call list_create.

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
        /*printf("\n");*/
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

        strcpy(task, input[0]);

        // Execute the command.
        if (!strcmp(task, "exec") || !strcmp(task, "e")) {
            process_exec(processes, input);
        } else if (!strcmp(task, "kill") || !strcmp(task, "k")) {
            process_kill();
        } else if (!strcmp(task, "stop") || !strcmp(task, "s")) {
            process_stop();
        } else if (!strcmp(task, "cont") || !strcmp(task, "c")) {
            process_cont();
        } else if (!strcmp(task, "list") || !strcmp(task, "l")) {
            process_list();
        } else if (!strcmp(task, "info") || !strcmp(task, "i")) {
            process_info();
        } else if (!strcmp(task, "quit") || !strcmp(task, "q")) {
            process_quit();
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
    // -1 in case of any test or itself failed.

    // variable declaration
    char *string;
    /*char **strings;*/
    char **tokens;
    char *delimiters;
    char **expected_tokens;
    const char space[] = " ";
    /*const char tab[] = "\t";*/
    const char space_tab[] = " \t";
    int num_tests;  // number of tests
    int num_passed;  // number of tests passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking
    void *return_pointer;  // pointer placeholder for error checking
    int i;  // generic counter

    num_tests = 0;
    num_passed = 0;

    // test 01
    num_tests++;
    failed = 0;

    return_pointer = strdup("a");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO free memory
        return -1;
    } else {
        string = return_pointer;
    }

    return_pointer = strdup(space_tab);
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO free memory
        return -1;
    } else {
        delimiters = return_pointer;
    }

    expected_tokens = NULL;
    return_pointer = (char **)realloc(expected_tokens,
            (size_t)(1 * sizeof(*expected_tokens)));
    if (return_pointer == NULL) {
        perror("error, realloc");
        // TODO free memory
        return -1;
    } else {
        expected_tokens = return_pointer;
    }

    expected_tokens[0] = NULL;
    return_pointer = strdup("a");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO free memory
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
    // TODO free memory

    // test 02
    num_tests++;
    failed = 0;

    return_pointer = strdup("a b c");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO free memory
        return -1;
    } else {
        string = return_pointer;
    }

    return_pointer = strdup(space);
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO free memory
        return -1;
    } else {
        delimiters = return_pointer;
    }

    expected_tokens = NULL;
    return_pointer = (char **)realloc(expected_tokens,
            (size_t)(3 * sizeof(*expected_tokens)));
    if (return_pointer == NULL) {
        perror("error, realloc");
        // TODO free memory
        return -1;
    } else {
        expected_tokens = return_pointer;
    }

    expected_tokens[0] = NULL;
    return_pointer = strdup("a");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO free memory
        return -1;
    } else {
        expected_tokens[0] = return_pointer;
    }

    expected_tokens[1] = NULL;
    return_pointer = strdup("b");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO free memory
        return -1;
    } else {
        expected_tokens[1] = return_pointer;
    }

    expected_tokens[2] = NULL;
    return_pointer = strdup("c");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO free memory
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
    // TODO free memory

    if (num_passed == num_tests) {
        printf("All str_split tests passed.\n");
        return 0;
    } else {
        printf("At least one str_split test failed.\n");
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

    if (num_passed == num_tests) {
        printf("\n");
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("\n");
        printf("At least one test failed.\n");
        return -1;
    }
}
////////////////////////////////////////////////////////////////////////////////

// main function
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    // Description
    // The main function TODO
    //
    // Returns
    // main returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    int return_value;  // integer placeholder for error checking

    /*test_str_split();*/

    return_value = task_queue();
    if (return_value == -1) {
        printf("error, task_queue\n");
    }

    /*test_all();*/

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
