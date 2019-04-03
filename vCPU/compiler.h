//
// Created by Ярослав Окатьев  on 2019-03-05.
//
#include <cstdio>
#include <iostream>
#include <vector>

#ifndef VCPU_COMPILER_H
#define VCPU_COMPILER_H

const int DEFAULT_SIZE = 100;

struct UserCommand
{
	std::string name;
	int argNum;
	std::string firstArg;
	std::string secondArg;
	std::vector<std::string> arg;
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
	std::string& readBuffer;
	char*  currWord;

	FILE* programFile;
	std::vector<UserCommand>& userProgram;
	int programLen;

	FILE* configFile;
	std::vector<DefinedCommand>& cmdList;
	int cmdAmount;

	std::vector<Label>& labelList;
	int labelCount;

	std::string& machineCode;

	void fileRead (FILE* file);
	void assignString (std::string* name_, int* i);
	int  readCommandCode (int* i);
	void setCommand (std::string name, int code);
	int  searchCommand (int line);
	bool setRegister (std::string argument, int line);
	int  labelAnalysis ();
	void skipSpaces (int* i);
public:
	Compiler (FILE* programFile_, FILE* configFile_);
	~Compiler ();
	void readUserProgram ();
	void readCommandList ();
	void generateMachineCode (std::string exeName);
};

size_t fileSize (FILE* file);
bool isDigit (std::string str);
void compilation (std::string programName, std::string config);

#endif //VCPU_COMPILER_H
