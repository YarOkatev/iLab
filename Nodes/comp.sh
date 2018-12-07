#!/bin/bash

gcc -c node.c -o node.o
gcc -c list.c -o list.o
gcc -Wall -g node.o list.o -o node.exe
rm *.o
echo "Compilation succeful"
