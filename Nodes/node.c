#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

int main () {
  int key = KeyGen ();
  List* first = InitList ();
  Node* m = NULL;

  Node* n = InitNode(1, NULL, NULL, key, first);
  for (int i = 2; i <= 10; i++) {
    n = InitNode(i, n, NULL, key, first);
    ConnectNodes (n->prev, n);
    if (i % 5 == 0)
      m = n;
  }

  //ListDelete (first);
  PrintNode (m, key);
  OutputList (first, key);
  ListDelete (first);
  free (first);
}
