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
int str_split(const char *str, char ***tokens, const char *delimiters);
void clear_screen();
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

void process_exec() {
/*void process_exec(char *child_argv[]) {*/
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
    /*char ***input;*/
    char task[MAX_TASK_LENGTH + 1];  // task code
    /*int return_value;  // integer placeholder for error checking*/

    // Clear the screen.
    //clear_screen();

    // Print a welcome message.
    printf("\tSCEE signal controlled execution environment\n\n");

    // TODO Print the program functions.

    strcpy(task, "");
    while (strcmp(task, "quit")) {
//        get_task();
//
//        input = NULL;
//        return_value = get_input(input);
//        if (return_value == -1) {
//            printf("error, get_input\n");
//        }
//
//        arguments = NULL;
//        return_value = get_input(input);
//        if (return_value == -1) {
//            printf("error, get_input\n");
//        }

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
            process_quit();
        } else {
                printf("No valid task requested.");
        }

        // TODO free allocated memory of the array input.
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
    const char space_tabs[] = " \t";
    int num_tests;  // number of tests
    int num_passed;  // number of tests that were passed
    int failed;  // boolean indicator that a test failed
    int return_value;  // integer placeholder for error checking
    void *return_pointer;  // pointer placeholder for error checking
    int i;  // generic counter

    num_tests = 0;
    num_passed = 0;

    // test 01
    num_tests++;

    return_pointer = strdup("a");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO memory cleanup
        return -1;
    } else {
        string = return_pointer;
    }

    return_pointer = strdup(space_tabs);
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO memory cleanup
        return -1;
    } else {
        delimiters = return_pointer;
    }

    expected_tokens = NULL;
    return_pointer = (char **)realloc(expected_tokens,
            (size_t)(1 * sizeof(*expected_tokens)));
    if (return_pointer == NULL) {
        perror("error, realloc");
        // TODO memory cleanup
        return -1;
    } else {
        expected_tokens = return_pointer;
    }

    expected_tokens[0] = NULL;
    return_pointer = strdup("a");
    if (return_pointer == NULL) {
        perror("error, strdup");
        // TODO memory cleanup
        return -1;
    } else {
        expected_tokens[0] = return_pointer;
    }

    failed = 0;
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
    // TODO memory cleanup

    if (num_passed == num_tests) {
        printf("All str_split tests were passed.\n");
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
    int num_passed;  // number of tests that were passed
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
        printf("All tests were passed.\n");
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

    /*test_str_split();*/

    test_all();

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
