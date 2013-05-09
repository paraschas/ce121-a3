#! /bin/bash

# compile the source code
gcc -Wall -g a3.c -o a3

# compile auxiliary programs
gcc -Wall -g integers.c -o integers
gcc -Wall -g times.c -o times

# run the application
./a3
