#! /bin/bash

# compile the source code
gcc -Wall -g scee.c -o scee

# compile auxiliary programs
gcc -Wall -g integers.c -o integers
gcc -Wall -g times.c -o times

# run the application
./scee
