//
// Created by Ярослав Окатьев  on 2019-03-05.
//
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#ifndef VCPU_COMPILER_H
#define VCPU_COMPILER_H

const int PROGRAM_SIZE = 100;

struct UserCommand
{
	std::string name;
	std::string firstArg;
	std::string secondArg;
};

struct DefinedCommand
{
	std::string name;
	int code;
	int argOne;
	int argTwo;
};

struct Label
{
	std::string name;
	int address;
};

size_t fileSize (FILE* file);
std::string* fileRead (FILE* file);
UserCommand* readUserProgram (FILE* programFile, int* programLen);
void skipSpaces (std::string& str, int* i);
void assignString (std::string* name_, std::string& rawInput, int* i);
DefinedCommand* readCommandList (FILE* config, int* cmdAmount);
int readCommandCode (std::string &rawInput, int* i);
void setCommand (std::string name, int code, DefinedCommand* command);
void generateMachineCode (DefinedCommand* cmdList, UserCommand* userProgram, int cmdAmount, int programLen,
						  std::string exeName);
bool isDigit (std::string str);
int searchCommand (UserCommand userProgram, int programLen, DefinedCommand* cmdList, int cmdAmount, int line);
bool setRegister (std::string* machineCodeStr, std::string argument, UserCommand userProgram, int line);
int labelAnalysis (std::string &codeStr, Label* labelList, int labelCount);
void compilation (std::string programName, std::string config);

#endif //VCPU_COMPILER_H
