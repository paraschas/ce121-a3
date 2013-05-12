// file integers.c
////////////////////////////////////////////////////////////////////////////////
// Description
// A simple program that prints to stdout the integers from 1 to 128 in
// ascending order, one every 4 seconds or the number of seconds passed as
// an argument. This value must be in the range [1, 32].
////////////////////////////////////////////////////////////////////////////////

// #include directives
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
////////////////////////////////////////////////////////////////////////////////

// #define directives
////////////////////////////////////////////////////////////////////////////////
#define PROGRAM_DESCRIPTION "A simple program that prints to stdout the integers from 1 to 128 in ascending order, one every 4 seconds or the number of seconds passed as an argument. This value must be in the range [1, 32]."
////////////////////////////////////////////////////////////////////////////////

// custom data types
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// global variable declaration
////////////////////////////////////////////////////////////////////////////////
static volatile sig_atomic_t counter = 0;  // number of outputs
static volatile sig_atomic_t delay = 4;  // number of seconds to wait between
        // successive outputs
////////////////////////////////////////////////////////////////////////////////

// function prototypes
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// functions
////////////////////////////////////////////////////////////////////////////////
static void handler_sigusr1(int signal_received) {
    // Description
    // This function is the signal handler for SIGUSR1.
    //
    // NOTE
    // I'd prefer to avoid using the non async-signal-safe function printf here,
    // and use write multiple times instead, calculating the length of counter
    // and delay values as strings with simple comparisons.
    //
    // NOTE
    // Of course sending back a signal to the parent is a still superior
    // implementation.
    //
    // Returns
    // handler_sigusr1 does not return any value.

    // variable declaration

    printf("\n!!! integers, delay %d: %d outputs so far, doing just fine !!!\n",
            delay, counter);
}
////////////////////////////////////////////////////////////////////////////////

// tests
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// main function
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    // Description
    // The main function implements the functionality described in the program
    // description.
    //
    // Returns
    // main returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    struct sigaction action = { {0} };
    int return_value;  // integer placeholder for error checking

    action.sa_handler = handler_sigusr1;
    return_value = sigaction(SIGUSR1, &action, NULL);
    if (return_value == -1) {
        perror("error, sigaction");
        return -1;
    }

    printf("\n%s\n", PROGRAM_DESCRIPTION);

    if (argc < 2) {
        /*delay = 4;  // redundant*/
    } else {
        return_value = atoi(argv[1]);
        if ((return_value <= 0) || (return_value > 32)) {
            // The number of seconds between successive outputs must be in
            // the range [1, 32].
            /*delay = 4;  // redundant*/
        } else {
            delay = return_value;
        }
    }

    for (counter = 0; counter <= 128; counter++) {
        printf("\n\tintegers, delay %d: %d\n", delay, counter);
        sleep(delay);
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
