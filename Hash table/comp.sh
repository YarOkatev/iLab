#!/bin/bash

gcc -c main.c -o main.o
gcc -c list.c -o list.o
gcc -Wall -g main.o list.o -o main.exe
rm *.o
echo "Compilation succeful"
./main.exe
