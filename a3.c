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
void clear_screen();
int task_queue();
int get_task(char *task);
////////////////////////////////////////////////////////////////////////////////

// functions
////////////////////////////////////////////////////////////////////////////////
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

int get_input(int *num_elements, char *elements[]) {

    return 0;
}

void process_exec(char *progname, (char *)NULL) {
    /*return_value = execv*/
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

int task_queue() {
    // Description
    // TODO
    //
    // Returns
    // task_queue returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    char task[MAX_TASK_LENGTH + 1];  // task code
    int num_elements;
    char *elements[];
    int return_value;
    /*char *return_value;  // placeholder pointer for error checking*/
    int return_value;  // integer placeholder for error checking

    // Clear the screen.
    /*clear_screen();*/

    // Print a welcome message.
    printf("\tSCEE signal controlled execution environment\n\n");

    // TODO Print the program functions.

    strcpy(task, "");
    while (strcmp(task, "quit")) {
        return_value = get_input(num_elements, elements);
        if (return_value == -1) {
            // TODO Research error checking with macros.
            perror("get_task error");
        }

        if (!strcmp(task, "exec") || !strcmp(task, "e")) {
            process_exec();
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
            process_info();
        } else {
                printf("No valid task requested.");
        }
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
