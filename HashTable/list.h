//
// Created by Ярослав Окатьев  on 2019-02-10.
//

#ifndef HASHTABLE_LIST_H
#define HASHTABLE_LIST_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const int MAXLEN = 64;

//int MAXLEN () {
//	return 64;
//

struct _Contact {
	long long int num;
	char name[64];
};

struct _Node {
	struct _Contact person;
	struct _Node* prev;
	struct _Node* next;
};

struct _List {
	long long int key;
	int size;
	struct _Node* head;
	struct _Node* tail;
};

typedef struct _List List;
typedef struct _Node Node;
typedef struct _Contact Contact;

Node* initNode (Contact person_);
void deleteNode (Node* del, List* list);
void printList (List* out);
List* initList ();
void connectNodes (Node* left, Node* right);
void listDelete (List* del);
int printNode (Node* tmp);
Node* pushHead (List* list, Contact person);
Node* pushTail (List* list, Contact person);
Contact popHead (List* list);
Contact popTail (List* list);
Node* insertNode (List*list, Node* node, Contact person, int location); // before - 0, after - 1
Node* findNode (List* list, int num);
Node* replace (Node* node, Contact person_);
int printNodeToFile (Node* tmp, FILE* file);
void printListToFile (List* out, FILE* file);

#include "list.c"

#endif //HASHTABLE_LIST_H
