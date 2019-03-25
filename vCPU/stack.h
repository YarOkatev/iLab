//
// Created by Ярослав Окатьев  on 2019-02-07.
//

#ifndef STACK_STACK_H
#define STACK_STACK_H

#include <cassert>
#include <cstdio>

class Stack
{
private:
	int* data_;
	int size_;
	int capacity_;
public:
	explicit Stack (int);
	~Stack ();
	int pop ();
	void push (int value);
	int top ();
	int size ();
	bool empty ();
	int capacity ();
	void add ();
	void sub ();
	void div ();
	void mul ();
	void ssqrt ();
	int cmp ();
	void dump ();
};

#endif //STACK_STACK_H
