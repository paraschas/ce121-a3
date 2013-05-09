// file seconds.c
////////////////////////////////////////////////////////////////////////////////
// Description
// A simple program that prints to stdout the current time 128 times, once
// every 4 seconds or the number of seconds given as an argument. This value
// must be in the range [1, 32].
////////////////////////////////////////////////////////////////////////////////

// #include directives
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////

// #define directives
////////////////////////////////////////////////////////////////////////////////
#define PROGRAM_DESCRIPTION "A simple program that prints to stdout the current time 128 times, once every 4 seconds or the number of seconds given as an argument. This value must be in the range [1, 32]."
////////////////////////////////////////////////////////////////////////////////

// custom data types
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// global variable declaration
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// function prototypes
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// functions
////////////////////////////////////////////////////////////////////////////////
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
    time_t system_time;
    struct tm *bd_time;  // broken down time
    int delay;  // number of seconds to wait between successive outputs
    int return_value;  // integer placeholder for error checking
    int i;  // generic counter

    printf("\n%s\n", PROGRAM_DESCRIPTION);

    if (argc < 2) {
        delay = 4;
    } else {
        return_value = atoi(argv[1]);
        if ((return_value <= 0) || (return_value > 32)) {
            // The number of seconds between successive outputs must be in
            // the range [1, 32].
            delay = 4;
        } else {
            delay = return_value;
        }
    }

    for (i = 0; i <= 128; i++) {
        // Get the current time.
        time(&system_time);
        bd_time = localtime(&system_time);

        printf("\n\ttimes: %02d:%02d:%02d, delay: %d\n",
                bd_time->tm_hour, bd_time->tm_min, bd_time->tm_sec, delay);
        sleep(delay);
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
