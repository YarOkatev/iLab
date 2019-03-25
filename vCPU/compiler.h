//
// Created by Ярослав Окатьев  on 2019-03-05.
//
#include <cstdio>
#include <iostream>
#include <vector>

#ifndef VCPU_COMPILER_H
#define VCPU_COMPILER_H

const int PROGRAM_SIZE = 200;

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

class Compiler
{
private:
	std::string readBuffer;

	FILE* programFile;
	UserCommand* userProgram;
	int programLen;

	FILE* configFile;
	DefinedCommand* cmdList;
	int cmdAmount;

	Label* labelList;
	int labelCount;
public:
	Compiler (FILE* programFile_, FILE* configFile_);
	void readUserProgram ();
	void fileRead (FILE* file);
	void assignString (std::string* name_, int* i);
	void readCommandList ();
	int readCommandCode (int* i);
	void setCommand (std::string name, int code);
	void generateMachineCode (std::string exeName);
	int searchCommand (int line);
	bool setRegister (std::string* machineCodeStr, std::string argument, int line);
	int labelAnalysis (std::string &codeStr);
	void skipSpaces (int* i);
	void memoryClean ();
};

size_t fileSize (FILE* file);
bool isDigit (std::string str);
void compilation (std::string programName, std::string config);

#endif //VCPU_COMPILER_H
