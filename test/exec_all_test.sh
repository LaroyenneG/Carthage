#!/bin/bash

EXEC_NAME="test.exe"

gcc -c -Wall *.c ../src/*.c
gcc -o $EXEC_NAME *.o -lpthread

rm *.o

(./$EXEC_NAME);

(valgrind ./$EXEC_NAME);

rm -f $EXEC_NAME;