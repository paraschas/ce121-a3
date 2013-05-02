// file a3.c
////////////////////////////////////////////////////////////////////////////////
// Description
// TODO
//
// TODO Implement list manipulation functions. Some tasks that come to mind:
// create, search, add element, remove element. The list should include
// a sentinel.
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
#define MAX_INPUT_LENGTH 1024
////////////////////////////////////////////////////////////////////////////////

// custom data types
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// global variable declaration
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// function prototypes
////////////////////////////////////////////////////////////////////////////////
int get_input(char **input, const int max_length);
void clear_screen();
int task_queue();
int get_task(char **arguments);
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
    buffer_size = 0;  // TODO Is it a good practice to include a cast here?
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
        printf("Error, the input was too long.");
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

    // Store a pointer of the input string to input.
    *input = temp_input;

    return input_length;
}

void clear_screen() {
    // Description
    // TODO
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

int get_task(char **arguments) {
    return 0;
}

void process_exec(char *child_argv[]) {
    //return_value = execv
}

int process_kill() {
    return 0;
}

int process_stop() {
    return 0;
}

int process_cont() {
    return 0;
}

int process_list() {
    return 0;
}

int process_info() {
    return 0;
}

int process_quit() {
    return 0;
}

int task_queue() {
    // Description
    // TODO
    //
    // Returns
    // task_queue returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    char **arguments;
    char task[MAX_TASK_LENGTH + 1];  // task code
    int return_value;  // integer placeholder for error checking

    // Clear the screen.
    //clear_screen();

    // Print a welcome message.
    printf("\tSCEE signal controlled execution environment\n\n");

    // TODO Print the program functions.

    strcpy(task, "");
    while (strcmp(task, "quit")) {
        arguments = NULL;
        return_value = get_task(arguments);
        // TODO Check get_task return value.

        if (return_value == -1) {
            // TODO Research error checking with macros.
            printf("error, get_task");
        }

        if (!strcmp(task, "exec") || !strcmp(task, "e")) {
            process_exec(arguments);
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
                printf("No valid task requested.");
        }

        // TODO free allocated memory of the array arguments. Probably write
        // a function for this.
    }

    return 0;
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

    task_queue();

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
