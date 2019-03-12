//
// Created by Ярослав Окатьев  on 2019-03-05.
//
#include <cstdio>
#include <cstdlib>
#include <iostream>

#ifndef VCPU_COMPILER_H
#define VCPU_COMPILER_H

const int PROGRAM_SIZE = 100;

class UserCommand
{
public:
	std::string name;
	std::string firstArg;
	std::string secondArg;
};

class DefinedCommand
{
public:
	std::string name;
	int code;
	int argOne;
	int argTwo;
};

class Label
{
public:
	std::string name;
	int address;
};

size_t fileSize (FILE* file);
std::string* fileRead (FILE* file);
class UserCommand* readUserProgram (FILE* programFile, int* programLen);
void skipSpaces (std::string& str, int* i);
void assignString (std::string* name_, std::string& rawInput, int* i);
class DefinedCommand* readCommandList (FILE* config, int* cmdAmount);
int readCommandCode (std::string &rawInput, int* i);
void setCommand (std::string name, int code, class DefinedCommand* command);
void compilationCode (class DefinedCommand* cmdList, class UserCommand* userProgram, int cmdAmount, int programLen);
bool isDigit (std::string str);
int searchCommand (class UserCommand userProgram, int programLen, class DefinedCommand* cmdList, int cmdAmount, int line);
bool setRegister (std::string* machineCodeStr, std::string argument, class UserCommand userProgram, int line);
int labelAnalysis (std::string &codeStr, class Label* labelList, int labelCount);

#endif //VCPU_COMPILER_H
