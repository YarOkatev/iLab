//
// Created by Ярослав Окатьев  on 2019-02-07.
//

#ifndef STACK_STACK_H
#define STACK_STACK_H

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
	int push (int value);
	int top ();
	int size ();
	bool empty ();
	void swap (Stack other);
};

#endif //STACK_STACK_H
