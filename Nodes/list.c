#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

const int MODE = 1; // 1 - fast, 0 - slow

const int CAN = 31415;

int KeyGen () {
  int seed = time (NULL);
  srand (seed);
  int key = rand () % 1000000000;
  return key;
}

Node* InitNode(int val_, int key) {
  Node* ret = (Node*) calloc (1, sizeof(Node));
  ret->next = NULL;
  ret->prev = NULL;
  ret->val = val_;
  if (MODE == 0)
    SecurefyNode (ret, key);
  return ret;
}

void SecurefyNode (Node* node, int key) {
  node->canary1 = CAN;
  node->checksum = node->val ^ key;
  node->canary2 = CAN;
}

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
  if (left == NULL) {
    right->prev = left;
    return;
  }
  if (right == NULL) {
    left->next = right;
    return;
  }
  right->prev = left;
  left->next = right;
}

void OutputList (List* out, int key) {
  if (MODE == 0) {
    if (out->canary1 != CAN || out->canary2 != CAN) {
      printf("Unfortunately, your data lost :(\n");
      return;
    }
  }
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

int Checksum (Node* tmp, int key) {
  return ((tmp->val ^ key) == tmp->checksum) && (tmp->canary1 == CAN) && (tmp->canary2 == CAN) ? 1 : 0;
}

List* InitList () {
  List* tmp = (List*) calloc (1, sizeof(List));
  if (MODE == 0) {
      tmp->canary1 = CAN;
      tmp->canary2 = CAN;
  }
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
  if ((MODE == 1) && (tmp != NULL)) {
    printf("%d ", tmp->val);
    return 0;
  }
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
  ConnectNodes (old, list->head);
  if (list->tail == NULL)
    list->tail = list->head;
  return list->head;
}

Node* PushTail (List* list, int val, int key) {
  Node* old = list->tail;
  list->tail = InitNode (val, key);
  list->size += 1;
  ConnectNodes (list->tail, old);
  if (list->head == NULL)
    list->head = list->tail;
  return list->tail;
}

int PopHead (List* list) {
  int value = list->head->val;
  Node* tmp = list->head->prev;
  list->head->prev->next = NULL;
  free (list->head);
  list->head = tmp;
  list->size -= 1;
  return value;
}

int PopTail (List* list) {
  int value = list->tail->val;
  Node* tmp = list->tail->next;
  list->tail->next->prev = NULL;
  free (list->tail);
  list->tail = tmp;
  list->size -= 1;
  return value;
}

Node* Replace (Node* node, int val_, int key) {
  node->val = val_;
  SecurefyNode (node, key);
  return node;
}

Node* InsertNode (List* list, Node* node, int val, int key, int location) { // before - 0, after - 1
  Node* tmp = InitNode (val, key);
  if (location == 0) {
    ConnectNodes (node->prev, tmp);
    ConnectNodes (tmp, node);
  }
  else {
    ConnectNodes (tmp, node->next);
    ConnectNodes (node, tmp);
  }
  list->size += 1;
  return tmp;
}

void IllustrateList (List* list, int key) {
  if (MODE == 0) {
    if (list->canary1 != CAN || list->canary2 != CAN) {
      printf("Unfortunately, your data lost :(\n");
      return;
    }
  }
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
    fprintf(output, "| <next> next\n %p} | checksum\n %d} \"]", cur->next, cur->checksum);
    counter++;
    cur = cur->next;
  }

  fprintf(output, "\n");
  cur = list->tail;
  counter = 1;
  while (cur != NULL) {
    if (!Checksum (cur, key)) {
      printf("List was damaged :(\n");
      return;
    }
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
