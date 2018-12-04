#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

int main () {
  int key = KeyGen ();
  List* first = InitList ();

  for (int i = 1; i <= 10; i++) {
    PushHead (first, i, key);
    PushTail (first, -i, key);
  }
  OutputList (first, key);
  Node* x = FindNode (first, 5);
  Replace (x, 99, key);
  InsertNode (x, 88, key, 1);
  OutputList (first, key);

  ListDelete (first);
  free (first);
}
