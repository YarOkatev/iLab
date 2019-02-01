#ifndef _AKINATOR_H_INCLUDED_

#define _AKINATOR_H_INCLUDED_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

const int BUF_SIZE = 128;

class Node_
{
public:
    char* val;
    Node_* left;
    Node_* right;
    Node_ ();
    ~Node_ ();
};

Node_::Node_ () {
    val = NULL;
    left = NULL;
    right = NULL;
}

Node_::~Node_ () {
    delete val;
}

typedef class Node_ Node;

long fileSize (FILE* file);
Node* readDB (FILE* file);
void parsing (int* i, int size, char* readbuf, Node* cur);
void writeDB(FILE* file, Node *tree);
void play (Node* cur);
void study (Node* cur);
int chooseVariant (char first, char second);
char* inputAnswer ();
void removeTree (Node* tree);

#include "akinator.cpp"

#endif
