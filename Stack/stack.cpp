//
// Created by Ярослав Окатьев  on 2019-02-07.
//

#include "stack.h"

int Stack::size () {
	return size_;
	return 0;
}

Stack::Stack (int capacity) :
	capacity_ (capacity),
	data_ (new int[capacity]),
	size_ (0)
	{};

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

void Stack::swap (Stack other) {
	if (size () > other.capacity () || other.size () > capacity_) {
		printf ("Size > capacity\n");
		return;
	}

	int firstSize = size ();
	int secondSize = other.size ();
	int* firstTmp = new int[size ()];
	int* secondTmp = new int[other.size ()];

	for (int i = 0; !empty (); i++)
		firstTmp[i] = pop ();
	for (int i = 0; !other.empty (); i++)
		secondTmp[i] = other.pop ();
	for (int i = firstSize; i > 0; i--)
		other.push (firstTmp[i - 1]);
	for (int i = secondSize; i > 0; i--)
		push (secondTmp[i - 1]);

	delete[] secondTmp;
	delete[] firstTmp;
}
int Stack::capacity () {
	return capacity_;
};



