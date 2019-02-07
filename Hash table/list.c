#include "list.h"

Node* InitNode (Contact person_) {
  Node* ret = (Node*) calloc (1, sizeof(Node));
  ret->next = NULL;
  ret->prev = NULL;
  ret->person = person_;
  return ret;
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
  if (del == NULL) {
    printf("Deleting error\n");
    return;
  }
  if (list->size == 1) {
    free (del);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
  }
  ConnectNodes (del->prev, del->next);
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

void PrintList (List* out) {
  int i = out->size;
  if (i == 0) {
    printf("List is empty\n");
    return;
  }
  Node* tmp = out->tail;
  for (; i > 0; i--) {
    if (PrintNode (tmp) != 0)
      return;
    tmp = tmp->next;
  }
  printf("\n");
}

void PrintList2File (List* out, FILE* file) {
  int i = out->size;
  if (i == 0) {
    printf("List is empty\n");
    return;
  }
  Node* tmp = out->tail;
  for (; i > 0; i--) {
    PrintNode2File (tmp, file);
    tmp = tmp->next;
  }
}

List* InitList () {
  List* tmp = (List*) calloc (1, sizeof(List));
  tmp->key = 0;
  tmp->size = 0;
  tmp->head = NULL;
  tmp->tail = NULL;
  return tmp;
}

void ListDelete (List* del) {
  Node* tmp = NULL;
  while (del->head != NULL) {
    tmp = del->head;
    del->head = del->head->prev;
    free (tmp);
  }
  del->tail = NULL;
  free (del);
}

int PrintNode (Node* tmp) {
  printf(" Phone number: +%lld\n Name: %s\n", tmp->person.num, tmp->person.name);
  return 0;
}

int PrintNode2File (Node* tmp, FILE* file) {
  fprintf(file, "%lld:%s\n", tmp->person.num, tmp->person.name);
  return 0;
}

Node* PushHead (List* list, Contact person) {
  Node* old = list->head;
  list->head = InitNode (person);
  list->size += 1;
  ConnectNodes (old, list->head);
  if (list->tail == NULL)
    list->tail = list->head;
  return list->head;
}

Node* PushTail (List* list, Contact person) {
  Node* old = list->tail;
  list->tail = InitNode (person);
  list->size += 1;
  ConnectNodes (list->tail, old);
  if (list->head == NULL)
    list->head = list->tail;
  return list->tail;
}

Contact PopHead (List* list) {
  Contact value = list->head->person;
  Node* tmp = list->head->prev;
  list->head->prev->next = NULL;
  free (list->head);
  list->head = tmp;
  list->size -= 1;
  return value;
}

Contact PopTail (List* list) {
  Contact value = list->tail->person;
  Node* tmp = list->tail->next;
  list->tail->next->prev = NULL;
  free (list->tail);
  list->tail = tmp;
  list->size -= 1;
  return value;
}

Node* Replace (Node* node, Contact person_) {
  node->person = person_;
  return node;
}

Node* InsertNode (List* list, Node* node, Contact person, int location) { // before - 0, after - 1
  Node* tmp = InitNode (person);
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
