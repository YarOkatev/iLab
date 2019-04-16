//
// Created by Ярослав Окатьев  on 2019-02-07.
//

#include <iostream>
#include <cmath>
#include "stack.h"

int Stack::size () {
	return size_;
}

Stack::Stack (int capacity):
	capacity_ (capacity),
	data_ (new int[capacity] {}),
	size_ (0) {};

Stack::~Stack () {
	delete[] data_;
	capacity_ = -1;
	size_ = -1;
};

int Stack::pop () {
	if (empty ()) {
		printf ("Stack is empty\n");
		return -1;
	}
	int ret = data_[size_ - 1];
	data_[size_ - 1] = 0;
	size_--;
	return ret;
}

void Stack::push (int value) {
	if (size_ <= capacity_) {
		size_++;
		data_[size_ - 1] = value;
		return;
	}
	printf ("Stack is full\n");
}

int Stack::top () {
	if (!empty ())
		return data_[size_ - 1];
	printf ("Stack is empty\n");
	return -1;
}

bool Stack::empty () {
	if (size_ > 0)
		return false;
	return true;
}

int Stack::capacity () {
	return capacity_;
}

void Stack::add () {
	int a = pop ();
	int b = pop ();
	push (a + b);
}

void Stack::sub () {
	int a = pop ();
	int b = pop ();
	push (a - b);
}

void Stack::div () {
	int a = pop ();
	int b = pop ();
	if (b == 0)
		std::cout << "Division by zero!\n";
	else
		push (a / b);
}

void Stack::mul () {
	int a = pop ();
	int b = pop ();
	push (a * b);
}

int Stack::cmp () {
	if (size_ < 2) {
		std::cout << "Stack contains less than two elements\n";
		return -1;
	}
	if (data_[size_ - 1] == data_[size_ - 2])
		return 0;
	if (data_[size_ - 1] > data_[size_ - 2])
		return 1;
	else
		return 2;
}

void Stack::dump () {
	std::cout << "Stack: ";
	for (int i = 0; i < size (); i++)
		std::cout << data_[i] << " ";
	std::cout << "\n";
}

void Stack::ssqrt () {
	int x = pop ();
	push (sqrt (x));
}

int* Stack::data () {
	return data_;
}

