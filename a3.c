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
/*#include <curses.h>*/
////////////////////////////////////////////////////////////////////////////////

// #define directives
////////////////////////////////////////////////////////////////////////////////
#define MAX_TASK_LENGTH 4
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
    //const char *cls_ansi_string = "\e[1;1H\e[2J";

    //printf("%s", cls_ansi_string);

    printf("\e[1;1H\e[2J");
}

int task_queue() {
    // Description
    // TODO
    //
    // Returns
    // task_queue returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    char task[MAX_TASK_LENGTH + 1];  // task code

    // Clear the screen.
    // TODO Maybe implement with ncurses.
    /*clear_screen();*/

    // Print a welcome message.
    printf("\tSCEE signal controlled execution environment\n\n");

    strcpy(task, "");
    while (strcmp(task, "quit")) {
        return_value = get_task(task);
        if (return_value == -1) {
            // TODO Research error checking with macros.
            perror("get_task error");
        }

        if (!strcmp(task, "exec")) {
            process_exec();
        } else if (!strcmp(task, "kill")) {
            process_kill();
        } else if (!strcmp(task, "stop")) {
            process_stop();
        } else if (!strcmp(task, "cont")) {
            process_cont();
        } else if (!strcmp(task, "list")) {
            process_list();
        } else if (!strcmp(task, "info")) {
            process_info();
        } else {
            printf("No valid task requested.");
        }
    }

    return 0;
}

void process_exec(char *progname, (char *)NULL) {
    // TODO or execv
    execl
}

process_kill();

process_stop();

process_cont();

process_list();

process_info();

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
