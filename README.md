ce121, assignment 3, signal controlled execution environment
============================================================

a3
--
This application implements an execution environment in which signals are used to stop, resume, terminate, and get information from spawned child processes, while a list of them is maintained and updated.

supported commands
------------------
exec, execute a file given its path and required arguments.

kill, terminate a spawned process given its PID.

stop, stop the execution of a running spawned process given its PID.

cont, resume the execution of a stopped spawned process given its PID.

list, print a list of the running spawned processes.

info, request information about a spawned process given it PID.

quit, terminate the application.

The commands can be requested using just the first letter of their name.

integers, times
---------------
integers and times are simple auxiliary programs to be called by the main application. They also handle the signal SIGUSR1. Once compiled, they can be executed via their i and t symbolic links, respectively.

integers
--------
A simple program that prints to stdout the integers from 1 to 128 in ascending order, one every 4 seconds or the number of seconds given as an argument. This value must be in the range [1, 32].

seconds
-------
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

license
=======
The MIT License, see LICENSE.md.
