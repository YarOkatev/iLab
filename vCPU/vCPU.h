//
// Created by Ярослав Окатьев  on 2019-03-17.
//

#ifndef COMPILER_VCPU_H
#define COMPILER_VCPU_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "stack.h"

const int MEM_SIZE = 1024;
const int STACK_SIZE = 32;

enum {AX, BX, CX, DX, LX};

class Processor;

class CpuMemory
{
private:
	int* regArray;
	int* memArray;
	int currentPtr;
public:
	CpuMemory ();
	void flashMem (FILE* bootFile);
	int  getWord ();
	friend Processor;
};

class Processor
{
private:
	CpuMemory memory;
	Stack stack;

	void halt ();
	void dump ();
	void loop ();

	void pop (); //51
	void scmp ();
	//add, sub, mul, div -- stack functions

	void top ();
	void rpush ();
	void rpop ();
	void jmp ();
	void je ();
	void jne ();
	void ja ();
	void jb ();
	void print ();

	void write ();

	void npush ();

	void rcmp ();

	void reg ();
public:
	Processor (FILE* bootFile);
	void tact ();
};

void  startCPU (std::string romFile);

#endif //COMPILER_VCPU_H