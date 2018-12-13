#include "hashtable.h"

int main () {
  long long int (*MainHash) (long long int) = Hash_Mod;

  FILE* database = fopen ("contacts.dat", "+");
  List* *table = (List**) calloc (10000, sizeof(List*));
  int amount = 0;
  FileRead (database, table, &amount, MainHash);

  // FILE* stat = fopen ("mod.csv", "w");
  // for (int i = 0; i < amount; i++) {
  //   fprintf(stat, "%d;%d \n", i, table[i]->size);
  // }

  printf("amount %d\n", amount);
  //AddContact (table, &amount, MainHash);

  FileWrite (database, table, &amount);
  fclose (database);
  free (table);
  return 0;
}
