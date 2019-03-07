//
// Created by Ярослав Окатьев  on 2019-03-05.
//
#include <cstdio>
#include <cstdlib>
#include <iostream>

#ifndef VCPU_COMPILER_H
#define VCPU_COMPILER_H

const int PROGRAM_SIZE = 100;

class userCommand
{
public:
	std::string name;
	std::string firstArg;
	std::string secondArg;
};

class definedCommand
{
public:
	std::string name;
	int code;
	int argOne;
	int argTwo;
};

size_t fileSize (FILE* file);
std::string* fileRead (FILE* file);
class userCommand* readUserProgram (FILE* programFile, int* programLen);
void skipSpaces (std::string& str, int* i);
void assignString (std::string* name_, std::string& rawInput, int* i);
class definedCommand* generateCommandList (FILE* config, int* cmdAmount);
int readCode (std::string& rawInput, int* i);
void setCommand (std::string name, int code, class definedCommand* command);
void generateMachineCode (class definedCommand* cmdList, class userCommand* userProgram, int cmdAmount, int programLen);
bool isDigit (std::string str);

#endif //VCPU_COMPILER_H
