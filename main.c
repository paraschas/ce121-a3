// file main.c
////////////////////////////////////////////////////////////////////////////////
// Description
// TODO
////////////////////////////////////////////////////////////////////////////////

// #include directives
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////////////

// #define directives
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

// functions
////////////////////////////////////////////////////////////////////////////////
int get_input(int *num_input_elements, char *input_elements[]) {
    // Description
    // This function reads a raw input string from stdin. The string is parsed
    // and the distinct elements contained are extracted and stored in
    // the char array input_elements. The first of the elements is expected to be
    // a task and the subsequent elements are its possible arguments.
    // The last element of the array is set as the NULL pointer, cast to (char *).
    // which it stores in the char array arguments. It additionally stores
    // the number of elements parsed in num_input_elements.
    //
    // Returns
    // get_input returns 0 on successful completion or -1 in case of failure.

    // variable declaration
    char input_format[16];  // scanf format string
    char *input;  // dynamically allocated string containing the raw user input
    int input_length;
    char temp_string[MAX_INPUT_LENGTH + 1];
    int return_value;  // generic integer placeholder for error checking
    /*void *return_pointer;  // pointer placeholder for error checking*/

    input = NULL;
    input_length = MAX_INPUT_LENGTH;

    input = malloc(MAX_INPUT_LENGTH + 1);
    if (input == NULL) {
        perror("error, malloc failed");
        return -1;
    }

    // Read the user input as a line from stdin.
    return_value = getline(&input, &input_length, stdin);
    if (return_value == -1) {
        perror("error, getline failed");
        return -1;
    }
    if (input_length > MAX_INPUT_LENGTH) {
    }

    // Generate the scanf format string.
    sprintf(temp_string_format, " %%%ds ", MAX_INPUT_LENGTH);

    num_input_elements = 0;
    while (1) {
        // Read the next element in input.
        return_value = sscanf(input, temp_string_format, &temp_string);

        if (return_value > 0) {
            input_elements[num_input_elements] = NULL;
            // Allocate memory for the element read.
            input_elements[num_input_elements] = malloc(strlen(temp_string) + 1);
            if (input_elements[num_input_elements] == NULL) {
                perror("error, malloc failed");
                // TODO memory cleanup
                return -1;
            }

            // Store the element in input_elements array.
            strcpy(input_elements[num_input_elements], temp_string);

            num_input_elements++;
        }
    }

    // Deallocate allocated memory.
    free(input);

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
    int return_value;  // integer placeholder for error checking
    int *num_input_elements;  // number of elements of the user input
    char **input_elements;  // array of elements of the user input

    // function to be tested.
/*int get_input(int *num_input_elements, char *input_elements[]) {*/
    return_value = get_input(num_input_elements, input_elements) {

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
