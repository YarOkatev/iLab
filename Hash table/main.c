#include "hashtable.h"

int main () {
  FILE* database = fopen ("contacts.dat", "r+");
  List* *table = (List**) calloc (10000, sizeof(List*));
  long long int (*MainHash) (long long int) = Hash_Mod;
  int amount = 0;
  FileRead (database, table, &amount, MainHash);

  printf("amount %d\n", amount);
  AddContact (table, &amount, MainHash);

  FileWrite (database, table, &amount);
  fclose (database);
  free (table);
  return 0;
}
