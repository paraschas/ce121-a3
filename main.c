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
int get_input(char **input, const int max_length);
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

//int str_split(const char *raw, char **tokens, const char *delimiters) {
//    // Description
//    // This function parses the string raw, extracts the substrings
//    // of it that are separated by one or more characters of
//    // the delimiters string, stores them in dynamically allocated memory,
//    // and stores pointers to them in the array tokens, which is also
//    // dynamically allocated. A terminating NULL pointer is appended to tokens,
//    // cast to (char *).
//    //
//    // Returns
//    // str_split returns 0 on successful completion or -1 in case of failure.
//
//    // variable declaration
//    char temp_string_format[16];
//    int temp_num_arguments;  // counter of the number of arguments read
//    int return_value;  // integer placeholder for error checking
//    char *input;  // dynamically allocated string containing the raw user input
//    char temp_argument[MAX_INPUT_LENGTH + 1];
//    void *return_pointer;  // pointer placeholder for error checking
//
//    // Generate a scanf format string.
//    sprintf(temp_string_format, " %%%ds ", MAX_INPUT_LENGTH);
//
//    // Store each argument in input in a dynamically allocated string.
//    // Store pointers to these strings in input_arguments.
//    temp_num_arguments = 0;
//    do {
//        // Read the next argument in input string.
//        return_value = sscanf(input, temp_string_format, &temp_argument);
//        if (return_value == EOF) {
//            perror("error, sscanf");
//            break;
//        }
//
//        if (return_value > 0) {
//            temp_num_arguments++;
//
//            // Allocate memory for a pointer to the argument.
//            return_pointer = (char **)realloc(input_arguments,
//                    temp_num_arguments * sizeof(char **));
//            if (return_pointer == NULL) {
//                printf("error, realloc\n");
//                return -1;
//            }
//            input_arguments[temp_num_arguments - 1] = NULL;
//
//            // Allocate memory for the argument.
//            input_arguments[temp_num_arguments - 1] =
//                    (char *)malloc((strlen(temp_argument) + 1));
//            if (input_arguments[temp_num_arguments - 1] == NULL) {
//                perror("error, malloc");
//                // TODO memory cleanup
//                return -1;
//            }
//
//            // Store the argument in input_arguments array.
//            strcpy(input_arguments[temp_num_arguments - 1], temp_argument);
//        } else {
//            break;
//        }
//    } while (input_arguments[temp_num_arguments - 1] != NULL);
//}

//int get_task(char *input, char **arguments) {
//    // Description
//    // This function reads an input from stdin which is parsed and the distinct
//    // arguments contained in it are extracted and stored in the char* array
//    // arguments. The input is then verified to be a valid task of
//    // the application. [...] TODO
//    //
//    // Returns
//    // TODO
//
//    // variable declaration
//    char *input;  // dynamically allocated string containing the raw user input
//    size_t buffer_size;
//    int input_length;
//    char **arguments;
//    int return_value;  // integer placeholder for error checking
//    const char *space_tabs = " \t";
//
//    // Read the user input as a line from stdin.
//    input = NULL;
//    buffer_size = 0;
//    return_value = getline(&input, &buffer_size, stdin);
//    if (return_value == -1) {
//        perror("error, getline");
//        return -1;
//    }
//
//    // Store the input length, excluding the newline character.
//    input_length = strlen(input) - 1;
//
//    // Verify that an entire line, terminating in a newline character, was read.
//    if (input[input_length] == '\n') {
//        // Remove the trailing newline character.
//        input[input_length] = '\0';
//    } else {
//        printf("Error, getline didn't read an entire line.\n");
//        free(input);
//        return -1;
//    }
//
//    // The input length should be at most equal to MAX_INPUT_LENGTH.
//    if (input_length > MAX_INPUT_LENGTH) {
//        printf("Error, the input was too large.\n");
//        free(input);
//        return -1;
//    }
//
//    // Split the input into distinct arguments.
//    return_value = str_split(input, arguments, space_tabs);
//    if (return_value == -1) {
//        printf("error, str_split\n");
//        return -1;
//    }
//
//    // Deallocate allocated memory.
//    free(input);
//
//    return 0;
//}
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
    char *input;
    int max_input_length;
    int return_value;  // integer placeholder for error checking

    // function to be tested: get_input
    input = NULL;
    max_input_length = MAX_INPUT_LENGTH;
    do {
        return_value = get_input(&input, max_input_length);
        if (return_value != -1) {
            printf("input read:\n");
            printf("%s\n", input);
            printf("input length returned by get_input: %d\n", return_value);
            printf("\n");
        }
    } while (return_value != -1);

    /*char **arguments;*/
    /*int i;  // generic counter*/
    /*// function to be tested: str_split*/
    /*return_value = str_split(input, arguments, space_tabs);*/
    /*if (return_value == -1) {*/
        /*printf("error, str_split\n");*/
        /*return -1;*/
    /*}*/

    /*for (i = 0; arguments[i] != NULL; i++) {*/
        /*printf("argument %2d: %s\n", i, arguments[i]);*/
    /*}*/

    /*// function to be tested: get_input*/
    /*arguments = NULL;*/
    /*return_value = get_input(arguments);*/
    /*if (return_value == -1) {*/
        /*printf("error, get_input failed\n");*/
    /*}*/

    /*for (i = 0; arguments[i] != NULL; i++) {*/
        /*printf("argument %2d: %s\n", i, arguments[i]);*/
    /*}*/

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
