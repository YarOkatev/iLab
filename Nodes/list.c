#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

const int CAN = 31415;

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
    ConnectNodes (del->prev, del->next);
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
    right->prev = left;
    left->next = right;
}

void OutputList (List* out, int key) {
  printf("sfgsd\n");
  if (out->canary1 != CAN || out->canary2 != CAN) {
    printf("Unfortunately, your data lost :(\n");
    return;
  }
  int i = out->size;
  if (i == 0) {
    printf("List is empty\n");
    return;
  }
  Node* tmp = out->tail;
  for (; i > 0; i--) {
    if (((tmp->val ^ key) == tmp->checksum) && (tmp->canary1 == CAN) && (tmp->canary2 == CAN) && (tmp != NULL)) {
      printf("%d ", tmp->val);
    }
    else {
      printf("List was damaged :(\n");
      break;
    }
    tmp = tmp->next;
  }
  printf("\n");
}

List* InitList () {
  List* tmp = (List*) calloc (1, sizeof(List));
  tmp->canary1 = CAN;
  tmp->size = 0;
  tmp->head = NULL;
  tmp->tail = NULL;
  tmp->canary2 = CAN;
  return tmp;
}
