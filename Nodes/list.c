#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

#define _SAVE_MODE_

const int CAN = 31415;

int KeyGen () {
  int seed = time (NULL);
  srand (seed);
  int key = rand () % 1000000000;
  return key;
}

#ifdef _SAVE_MODE_
int CalcSum (Node* node, int key) {
int sum = (int) node->next ^ (int) node -> prev ^ node->val ^ key;
  return sum;
}
#endif

Node* InitNode(int val_, int key) {
  Node* ret = (Node*) calloc (1, sizeof(Node));
  ret->next = NULL;
  ret->prev = NULL;
  ret->val = val_;

#ifdef _SAVE_MODE_
  SecurefyNode (ret, key);
#endif

  return ret;
}

#ifdef _SAVE_MODE_
void SecurefyNode (Node* node, int key) {
  node->canary1 = CAN;
  node->checksum = CalcSum (node, key);
  node->canary2 = CAN;
}
#endif

Node* FindNode (List* list, int num) {
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

void DeleteNode (Node* del, List* list, int key) {
  if (!del) {
    ConnectNodes (del->prev, del->next, key);
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

void ConnectNodes (Node* left, Node* right, int key) {
  if (left == NULL) {
    right->prev = left;
#ifdef _SAVE_MODE_
    SecurefyNode (right, key);
#endif
    return;
  }
  if (right == NULL) {
    left->next = right;
#ifdef _SAVE_MODE_
    SecurefyNode (left, key);
#endif
    return;
  }
  right->prev = left;
  left->next = right;
#ifdef _SAVE_MODE_
  SecurefyNode (left, key);
  SecurefyNode (right, key);
#endif
}

void OutputList (List* out, int key) {

#ifdef _SAVE_MODE_
  if (out->canary1 != CAN || out->canary2 != CAN) {
    printf("Unfortunately, your data lost :(\n");
    return;
  }
#endif

  int i = out->size;
  if (i == 0) {
    printf("List is empty\n");
    return;
  }
  Node* tmp = out->tail;
  for (; i > 0; i--) {
    if (PrintNode (tmp, key) != 0)
      return;
    tmp = tmp->next;
  }
  printf("\n");
}

#ifdef _SAVE_MODE_
int Checksum (Node* tmp, int key) {
  return (CalcSum (tmp, key) == tmp->checksum) && (tmp->canary1 == CAN) && (tmp->canary2 == CAN) ? 1 : 0;
}
#endif

List* InitList () {
  List* tmp = (List*) calloc (1, sizeof(List));

#ifdef _SAVE_MODE_
  tmp->canary1 = CAN;
  tmp->canary2 = CAN;
#endif

  tmp->size = 0;
  tmp->head = NULL;
  tmp->tail = NULL;
  return tmp;
}

void ListDelete (List* del) {
  if (del->size == 0)
    return;
  Node* tmp = NULL;
  while (del->head != NULL) {
    tmp = del->head;
    del->head = del->head->prev;
    free (tmp);
  }
  del->tail = NULL;
  del->size = 0;
}

int PrintNode (Node* tmp, int key) {

#ifndef _SAVE_MODE_
    printf("%d ", tmp->val);
    return 0;
#endif

  if (Checksum (tmp, key) && (tmp != NULL)) {
    printf("%d ", tmp->val);
  }
  else {
    printf("List was damaged :(\n");
    return -1;
  }
  return 0;
}

Node* PushHead (List* list, int val, int key) {
  Node* old = list->head;
  list->head = InitNode (val, key);
  list->size += 1;
  ConnectNodes (old, list->head, key);
  if (list->tail == NULL)
    list->tail = list->head;
  return list->head;
}

Node* PushTail (List* list, int val, int key) {
  Node* old = list->tail;
  list->tail = InitNode (val, key);
  list->size += 1;
  ConnectNodes (list->tail, old, key);
  if (list->head == NULL)
    list->head = list->tail;
  return list->tail;
}

int PopHead (List* list, int key) {
  int value = list->head->val;
  Node* tmp = list->head->prev;
  list->head->prev->next = NULL;
  free (list->head);
  list->head = tmp;

#ifdef _SAVE_MODE_
  SecurefyNode (list->head, key);
#endif

  list->size -= 1;
  return value;
}

int PopTail (List* list, int key) {
  int value = list->tail->val;
  Node* tmp = list->tail->next;
  list->tail->next->prev = NULL;
  free (list->tail);
  list->tail = tmp;

#ifdef _SAVE_MODE_
  SecurefyNode (list->tail, key);
#endif

  list->size -= 1;
  return value;
}

Node* Replace (Node* node, int val_, int key) {
  node->val = val_;

#ifdef _SAVE_MODE_
  SecurefyNode (node, key);
#endif

  return node;
}

Node* InsertNode (List* list, Node* node, int val, int key, int location) { // before - 0, after - 1
  Node* tmp = InitNode (val, key);
  if (location == 0) {
    ConnectNodes (node->prev, tmp, key);
    ConnectNodes (tmp, node, key);
  }
  else {
    ConnectNodes (tmp, node->next, key);
    ConnectNodes (node, tmp, key);
  }
  list->size += 1;
  return tmp;
}

void IllustrateList (List* list, int key) {

#ifdef _SAVE_MODE_
    if (list->canary1 != CAN || list->canary2 != CAN) {
      printf("Unfortunately, your data lost :(\n");
      return;
    }
#endif

  FILE * output = fopen("list.dot", "w");
  fprintf(output, "digraph G{\nrankdir = LR;\n\tnode[shape = \"box\", color=\"black\", fontsize=14];\n\tedge[color=\"black\"];\n");
  if (list->canary1 == CAN)
    fprintf(output, "\tcanary1 [label = \"Canary1 = %d\"]\n", list->canary1);
  if (list->canary2 == CAN)
    fprintf(output, "\tcanary2 [label = \"Canary2 = %d\"]\n", list->canary2);
  if (list->size >= 0)
    fprintf(output, "size [label =\"Size = %d\"]\n", list->size);

  Node* cur = list->tail;
  int counter = 1;
  while (cur != NULL) {
    fprintf(output, "\telem_%d [shape = \"record\"  , label = \"<ptr>%d\\n", counter, counter);
    if(cur == list->head)
      fprintf(output, "HEAD\\n");
    if (cur == list->tail)
      fprintf(output, "TAIL\\n");
    fprintf(output, "%p | %d | {<prev> prev \n%p\\n", cur, cur->val, cur->prev);
    fprintf(output, "| <next> next\n %p}", cur->next);

#ifdef _SAVE_MODE_
    fprintf(output, " | checksum\n %d}", cur->checksum);
#endif

    fprintf(output," \"]");
    counter++;
    cur = cur->next;
  }

  fprintf(output, "\n");
  cur = list->tail;
  counter = 1;
  while (cur != NULL) {
    if (cur->prev != NULL)
      fprintf(output, "\telem_%d:<prev>->elem_%d:<ptr>\n", counter, counter - 1);
    if (cur->next != NULL)
      fprintf(output, "\telem_%d:<next>->elem_%d:<ptr>\n", counter, counter + 1);
    counter++;
    cur = cur->next;
  }
  fprintf(output, "}");
  fclose(output);
  system("dot -Tpng list.dot -o MyList.png");
}
