#ifndef _LIST_H_INCLUDED_
#define _LIST_H_INCLUDED_

struct _Node {
  int canary1;
  int val;
  int checksum;
  struct _Node* prev;
  struct _Node* next;
  int canary2;
};

struct _List {
  int canary1;
  int size;
  struct _Node* head;
  struct _Node* tail;
  int canary2;
};

typedef struct _List List;
typedef struct _Node Node;

Node* InitNode(int val_, Node* prev_, Node* next_, int key, List* list);
int KeyGen ();
void DeleteNode (Node* del, List* list);
void OutputList (List* out, int key);
List* InitList ();
void ConnectNodes (Node* left, Node* right);
void ListDelete (List* del);
int Checksum (Node* tmp, int key);
int PrintNode (Node* tmp, int key);

#endif /* _LIST_H_INCLUDED_ */
