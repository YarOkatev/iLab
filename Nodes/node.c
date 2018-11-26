#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int CAN = 31415;

struct _node {
  int canary1;
  int val;
  int checksum;
  struct _node* prev;
  struct _node* next;
  int canary2;
};

typedef struct _node node;

node* CreateNode (int val_, node* prev_, node* next_, int key);
void ConnectNodes (node* left, node* right);
int KeyGen ();
void DeleteNode (node* del);
void OutputList (node* start, int key);

int main () {
  int key = KeyGen ();
  node* first = CreateNode (rand () % 1000, NULL, NULL, key);
  node* n = first;
  for (int i = 0; i < 10; i++) {
    n = CreateNode (rand () % 1000, n, NULL, key);
    ConnectNodes (n->prev, n);
  }
  //
  // char* a = (char*) calloc (1, sizeof(char));
  // for (int i = 0; i < 1000; i++)
  //   a[i] = 1;
  // printf("\n");

  OutputList (first->next, key);
}

int KeyGen () {
  int seed = time (NULL);
  srand (seed);
  int key = rand () % 1000000000;
  return key;
}

node* CreateNode (int val_, node* prev_, node* next_, int key) {
  // assert (prev == next);
  // assert (!prev);
  // assert (!next);
  node* ret = (node*) calloc (1, sizeof(node));
  ret->canary1 = CAN;
  ret->val = val_;
  ret->checksum = val_ ^ key;
  ret->prev = prev_;
  ret->next = next_;
  ret->canary2 = CAN;
  return ret;
}

void DeleteNode (node* del) {
  assert (!del);
  del->prev->next = del->next;
  del->next->prev = del->prev;
}

void ConnectNodes (node* left, node* right) {
  // assert (!left);
  // assert (!right);
  // assert (left->prev == right);
  // assert (right->next == left);
  //assert (left == right);
  right->prev = left;
  left->next = right;
}

void OutputList (node* start, int key) {
  if (start) {
    if (((start->val ^ key) == start->checksum) && (start->canary1 == CAN) && (start->canary2 == CAN)) {
      printf("%d\n", start->val);
      OutputList (start->next, key);
    }
    else
    printf("Unfortunately, your data lost :(\n");
  }
}
