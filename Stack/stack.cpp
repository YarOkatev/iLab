//
// Created by Ярослав Окатьев  on 2019-02-07.
//

#include "stack.h"

int Stack::size () {

	return 0;
}

Stack::Stack (int capacity) :
	capacity_ (capacity),
	data_ (new int[capacity_]),
	size_ (0)
	{};

Stack::~Stack () {
	delete data_;
	capacity_ = -1;
	size_ = -1;
};

int Stack::pop () {

	return 0;
};



