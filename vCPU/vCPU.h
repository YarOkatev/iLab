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
const int STACK_SIZE = 2048;
const int REG_NUM = 8;

enum {AX, BX, CX, DX, LX, SP, FP, RX};  //SP - Stack Pointer, FP - Frame Pointer, RX - Return

class Processor;

class CpuMemory
{
private:
	int* regArray;
	int* memArray;
	int currentPtr;
public:
	CpuMemory ();
	~CpuMemory ();
	void flashMem (FILE* bootFile);
	int  getWord ();
	friend Processor;
};

class Processor
{
private:
	CpuMemory memory;
	Stack stack;

	void halt (); //0
	void dump (); //1
	void loop (); //100

	void pop (); //51
	void scmp (); //52
	//add, sub, mul, div -- stack functions // 53, 54, 55, 56

	void top (); //101
	void rpush (); //103
	void rpop (); //104
	void jmp (); //105
	void je (); //106
	void jne (); //107
	void ja (); //108
	void jb (); //109
	void print (); //110

	void write (); //111

	void npush (); //151

	void rcmp (); //201

	void movN (); //252

	void movSN (); //301
	void movSR (); //302
	void retR (); //112
	void retN (); //152
	void call (); //303

	void stacking (int argNum);
	void unstacking ();
public:
	Processor (FILE* bootFile);
	void tact ();
};

void  startCPU (std::string romFile);

#endif //COMPILER_VCPU_H