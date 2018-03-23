#!/bin/bash

EXEC_NAME="test.exe"

gcc -c -Wall *.c ../src/*.c
gcc -o $EXEC_NAME *.o -lpthread

rm *.o


valgrind ./$EXEC_NAME;

rm -f $EXEC_NAME;