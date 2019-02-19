#ifndef AKINATOR_AKINATOR_H
#define AKINATOR_AKINATOR_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

const int BUF_SIZE = 128;

class Node_
{
private:
    char* val;
    Node_* left;
    Node_* right;
public:
    char* getValue ();
    Node_* getLeft ();
    Node_* getRight ();
    void setValue (char* data);
    void setLeft (Node_* left_);
    void setRight (Node_* right_);
    Node_ ();
    ~Node_ ();
};

Node_::Node_ ():
    val (nullptr),
    left (nullptr),
    right (nullptr)
    {};

typedef class Node_ Node;

size_t fileSize (FILE* file);
Node* readDB (FILE* file);
void parsing (int* i, size_t size, char* readBuf, Node* cur);
void writeDB(FILE* file, Node *tree);
void play (Node* cur);
void study (Node* cur);
int chooseVariant (char first, char second);
char* readAnswer ();
void removeTree (Node* tree);
void changeAnswer (Node* cur);

#include "akinator.cpp"

#endif //AKINATOR_AKINATOR_H
