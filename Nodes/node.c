#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int CAN = 31415;

struct _Node {
  int canary1;
  int val;
  int checksum;
  struct _Node* prev;
  struct _Node* next;
  int canary2;
};

struct _List {
  int size;
  struct _Node* head;
  struct _Node* tail;
};

typedef struct _List List;
typedef struct _Node Node;

Node* InitNode(int val_, Node* prev_, Node* next_, int key, List* list);
int KeyGen ();
void DeleteNode (Node* del, List* list);
void OutputList (List* out, int key);
List* InitList ();
void ConnectNodes (Node* left, Node* right);

//------------------------------------------------//

int main () {
  int key = KeyGen ();
  List* first = InitList ();
  Node* n = InitNode(1, NULL, NULL, key, first);
  for (int i = 2; i <= 10; i++) {
    n = InitNode(i, n, NULL, key, first);
    ConnectNodes (n->prev, n);
  }

  //
  // char* a = (char*) calloc (1, sizeof(char));
  // for (int i = 0; i < 1000; i++)
  //   a[i] = 1;
  // printf("\n");

  OutputList (first, key);
}

//------------------------------------------------//

int KeyGen () {
  int seed = time (NULL);
  srand (seed);
  int key = rand () % 1000000000;
  return key;
}

Node* InitNode(int val_, Node* prev_, Node* next_, int key, List* list) {
  Node* ret = (Node*) calloc (1, sizeof(Node));
  ret->canary1 = CAN;
  ret->val = val_;
  ret->checksum = val_ ^ key;
  ret->prev = prev_;
  ret->next = next_;
  ret->canary2 = CAN;
  if (prev_ == NULL)
    list->tail = ret;
  if (next_ == NULL)
    list->head = ret;
  list->size += 1;
  return ret;
}

void DeleteNode (Node* del, List* list) {
  if (!del) {
    del->prev->next = del->next;
    del->next->prev = del->prev;
  }
  else {
    printf("Deleting error\n");
  }
  if (del->next == NULL)
    list->head = del->prev;
  if (del->prev == NULL)
    list->tail = del->next;
  list->size -= 1;
  free (del);
  del = NULL;
}

void ConnectNodes (Node* left, Node* right) {
  // assert (!left);
  // assert (!right);
  // assert (left->prev == right);
  // assert (right->next == left);
  //assert (left == right);
    right->prev = left;
    left->next = right;
}

void OutputList (List* out, int key) {
  int i = out->size;
  if (i == 0) {
    printf("List is empty\n");
    return;
  }
  Node* tmp = out->tail;
  for (; i > 0; i--) {
    if (((tmp->val ^ key) == tmp->checksum) && (tmp->canary1 == CAN) && (tmp->canary2 == CAN)) {
      printf("%d\n", tmp->val);
    }
    else {
      printf("Unfortunately, your data lost :(\n");
      break;
    }
    tmp = tmp->next;
  }
}

List* InitList () {
  List* tmp = (List*) calloc (1, sizeof(List));
  tmp->size = 0;
  tmp->head = NULL;
  tmp->tail = NULL;
  return tmp;
}
