## UID: 505962397

## Pipe Up

This program uses low-level functions in C including execlp(), fork(), pipe(), dup2(), and wait() to simulate the | operator implemented in the shell, and accommodates as many process names as arguments as desired.

## Building

To build, first cd into the directory "lab1/" then run the command 'make'

Example:
```
cs111@cs111 cs111_labs/lab1 (master !*+%) » make
cc -std=c17 -Wpedantic -Wall -O2 -pipe -fno-plt   -c -o pipe.o pipe.c
cc -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now  pipe.o   -o pipe
```

## Running

To run the program, type the command ./pipe [arg1] [arg2] ... [argn], and the expected output would be the shell equivalent of [arg1] | [arg2] | ... | [argn]. This works for any whole number of n arguments.

Example: 
```
cs111@cs111 cs111_labs/lab1 (master !*+%) » ./pipe ls cat sort wc
      7       7      63
cs111@cs111 cs111_labs/lab1 (master !*+%) » ls | cat | sort | wc 
      7       7      63
```

Example of included Python unittest:
```
cs111@cs111 cs111_labs/lab1 (master !*+%) » python -m unittest
..      8       8      73
.
----------------------------------------------------------------------
Ran 3 tests in 1.610s

OK
```

## Cleaning up

To clean up from the execution, run the command 'make clean' in the same directory.

Example:
```
cs111@cs111 cs111_labs/lab1 (master !*+%) » make clean
rm -f pipe.o pipe
```