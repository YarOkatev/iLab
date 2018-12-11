#ifndef _LIST_H_INCLUDED_
#define _LIST_H_INCLUDED_

struct _Contact {
  long long int num;
  char name[32];
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

Node* InitNode(Contact person_);
void DeleteNode (Node* del, List* list);
void PrintList (List* out);
List* InitList ();
void ConnectNodes (Node* left, Node* right);
void ListDelete (List* del);
int PrintNode (Node* tmp);
Node* PushHead (List* list, Contact person);
Node* PushTail (List* list, Contact person);
Contact PopHead (List* list);
Contact PopTail (List* list);
Node* InsertNode (List*list, Node* node, Contact person, int location); // before - 0, after - 1
Node* FindNode (List* list, int num);
Node* Replace (Node* node, Contact person_);
void IllustrateList (List* list);

#endif /* _LIST_H_INCLUDED_ */
