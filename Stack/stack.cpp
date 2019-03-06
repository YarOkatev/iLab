//
// Created by Ярослав Окатьев  on 2019-02-07.
//

#include "stack.h"

int Stack::size () {
	return size_;
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

int Stack::capacity () {
	return capacity_;
}

void Stack::math (Node* root) {
	if (root != nullptr) {
		math (root->left);
		math (root->right);
	}


}
