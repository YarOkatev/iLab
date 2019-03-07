//
// Created by Ярослав Окатьев  on 2019-03-05.
//
#include <cstdio>
#include <cstdlib>
#include <iostream>

#ifndef VCPU_COMPILER_H
#define VCPU_COMPILER_H

const int PROGRAM_SIZE = 100;

struct userCommand
{
	std::string* name;
	std::string* firstArg;
	std::string* secondArg;
};

struct definedCommand
{
	std::string* name;
	int code;
	int argOne;
	int argTwo;
};

size_t fileSize (FILE* file);
std::string* fileRead (FILE* file);
struct userCommand* readUserProgram (FILE* programFile, int* programLen);
void skipSpaces (std::string& str, int* i);
void assignString (std::string* name_, std::string& rawInput, int* i);
struct definedCommand* generateCommandList (FILE* config, int* cmdAmount);
int readCode (std::string& rawInput, int* i);
void setCommand (std::string* name, int code, struct definedCommand* command);
void generateMachineCode (struct definedCommand* cmdList, struct userCommand* userProgram, int cmdAmount, int programLen);

#endif //VCPU_COMPILER_H
