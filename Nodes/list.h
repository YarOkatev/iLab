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

Node* InitNode(int val_, int key);
int KeyGen ();
void DeleteNode (Node* del, List* list, int key);
void OutputList (List* out, int key);
List* InitList ();
void ConnectNodes (Node* left, Node* right, int key);
void ListDelete (List* del);
int Checksum (Node* tmp, int key);
int PrintNode (Node* tmp, int key);
void SecurefyNode (Node* node, int key);
Node* PushHead (List* list, int val, int key);
Node* PushTail (List* list, int val, int key);
int PopHead (List* list, int key);
int PopTail (List* list, int key);
Node* InsertNode (List*list, Node* node, int val, int key, int location); // before - 0, after - 1
Node* FindNode (List* list, int num);
Node* Replace (Node* node, int val_, int key);
void IllustrateList (List* list, int key);
int CalcSum (Node* node, int key);

#endif /* _LIST_H_INCLUDED_ */
