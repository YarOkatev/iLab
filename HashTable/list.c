//
// Created by Ярослав Окатьев  on 2019-02-10.
//

#ifndef _LIST_C_INCLUDED_
#define _LIST_C_INCLUDED_

#include "list.h"

Node* initNode (Contact person_) {
	Node* ret = (Node*) calloc (1, sizeof(Node));
	ret->next = NULL;
	ret->prev = NULL;
	ret->person = person_;
	return ret;
}

Node* findNode (List* list, int num) {
	if (list->size < num) {
		printf("Invalid number\n");
		return NULL;
	}
	Node* ret = NULL;
	if ((list->size / 2) < num) {
		ret = list->head;
		for (int i = 0; i < list->size - num - 1; i++)
			ret = ret->prev;
	}
	else {
		ret = list->tail;
		for (int i = 0; i < num; i++)
			ret = ret->next;
	}
	return ret;
}

void deleteNode (Node* del, List* list) {
	if (del == NULL) {
		printf("Deleting error\n");
		return;
	}
	if (list->size == 1) {
		free (del);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
	}
	connectNodes (del->prev, del->next);
	if (del->next == NULL)
		list->head = del->prev;
	if (del->prev == NULL)
		list->tail = del->next;
	list->size -= 1;
	free (del);
	del = NULL;
}

void connectNodes (Node* left, Node* right) {
	if (left == NULL) {
		right->prev = left;
		return;
	}
	if (right == NULL) {
		left->next = right;
		return;
	}
	right->prev = left;
	left->next = right;
}

void printList (List* out) {
	int i = out->size;
	if (i == 0) {
		printf("List is empty\n");
		return;
	}
	Node* tmp = out->tail;
	for (; i > 0; i--) {
		if (printNode (tmp) != 0)
			return;
		tmp = tmp->next;
	}
	printf("\n");
}

void printListToFile (List* out, FILE* file) {
	int i = out->size;
	if (i == 0) {
		//printf("List is empty\n");
		return;
	}
	Node* tmp = out->tail;
	for (; i > 0; i--) {
		printNodeToFile (tmp, file);
		tmp = tmp->next;
	}
}

List* initList () {
	List* tmp = (List*) calloc (1, sizeof(List));
	tmp->key = 0;
	tmp->size = 0;
	tmp->head = NULL;
	tmp->tail = NULL;
	return tmp;
}

void listDelete (List* del) {
	Node* tmp = NULL;
	while (del->head != NULL) {
		tmp = del->head;
		del->head = del->head->prev;
		free (tmp);
	}
	del->tail = NULL;
	free (del);
}

int printNode (Node* tmp) {
	printf(" Phone number: +%lld\n Name: %s\n", tmp->person.num, tmp->person.name);
	return 0;
}

int printNodeToFile (Node* tmp, FILE* file) {
	fprintf(file, "%lld:%s\n", tmp->person.num, tmp->person.name);
	return 0;
}

Node* pushHead (List* list, Contact person) {
	Node* old = list->head;
	list->head = initNode (person);
	list->size += 1;
	connectNodes (old, list->head);
	if (list->tail == NULL)
		list->tail = list->head;
	return list->head;
}

Node* pushTail (List* list, Contact person) {
	Node* old = list->tail;
	list->tail = initNode (person);
	list->size += 1;
	connectNodes (list->tail, old);
	if (list->head == NULL)
		list->head = list->tail;
	return list->tail;
}

Contact popHead (List* list) {
	Contact value = list->head->person;
	Node* tmp = list->head->prev;
	list->head->prev->next = NULL;
	free (list->head);
	list->head = tmp;
	list->size -= 1;
	return value;
}

Contact popTail (List* list) {
	Contact value = list->tail->person;
	Node* tmp = list->tail->next;
	list->tail->next->prev = NULL;
	free (list->tail);
	list->tail = tmp;
	list->size -= 1;
	return value;
}

Node* replace (Node* node, Contact person_) {
	node->person = person_;
	return node;
}

Node* insertNode (List* list, Node* node, Contact person, int location) { // before - 0, after - 1
	Node* tmp = initNode (person);
	if (location == 0) {
		connectNodes (node->prev, tmp);
		connectNodes (tmp, node);
	}
	else {
		connectNodes (tmp, node->next);
		connectNodes (node, tmp);
	}
	list->size += 1;
	return tmp;
}

#endif

