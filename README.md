ce121, assignment 3, signal controlled execution environment
============================================================
This application implements an execution environment in which signals are used to stop, resume, terminate, and get information from spawned child processes.

integers and times are simple auxiliary programs to be called by the main application. They also handle the signal SIGUSR1. Once compiled, they can be executed via their i and t symbolic links, respectively.

integers
A simple program that prints to stdout the integers from 1 to 128 in ascending order, one every 4 seconds or the number of seconds given as an argument. This value must be in the range [1, 32].

seconds
A simple program that prints to stdout the current time 128 times, once every 4 seconds or the number of seconds given as an argument. This value must be in the range [1, 32].

course
======
Programming II

http://inf-server.inf.uth.gr/courses/CE121/

spring 2013


author
======
Dimitrios Paraschas
1562
