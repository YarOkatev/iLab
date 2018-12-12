#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

const long long int XOR_KEY = 2718281828, MOD_KEY = 2;

long long int Hash_DigitSum (long long int num);
long long int Hash_XOR (long long int num);
long long int Hash_HalfXOR (long long int num);
long long int Hash_Mod (long long int num);
void FileRead (FILE* database, List** table, int* amount, long long int (*Hash) (long long int));
int FindNum (long long int number, List** table, int amount, long long int (*Hash) (long long int));
Contact FindInList(int count, List** table, long long int num);
Contact FindContact (long long int number, List** table, int* amount, long long int (*Hash) (long long int));
int FindList (List** table, int amount, long long int key);
void CreateContact (Contact newContact, List** table, int* amount, long long int (*Hash) (long long int));
void DeleteNumber (long long int num, List** table, int* amount, long long int (*Hash) (long long int));
void AddContact (List** table, int* amount, long long int (*Hash) (long long int));
void FileWrite (FILE* database, List** table, int* amount);

long long int (*MainHash) (long long int) = Hash_Mod;

//----------------------------------------------------------------------------//

int main () {
  FILE* database = fopen ("contacts.dat", "r+");
  List* *table = (List**) calloc (10000, sizeof(List*));
  int amount = 0;
  FileRead (database, table, &amount, MainHash);

  printf("amount %d\n", amount);

  // for (int i = 0; i < amount; i++) {
  //   //printf("\n%d\n", i);
  //   //printf("KEY %lld\n", table[i]->key);
  //   //fprintf(stdout, "SIZE %d\n", table[i]->size);
  //   //PrintList (table[i]);
  // }

  FileWrite (database, table, &amount);
  free (table);
  return 0;
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//

void FileWrite (FILE* database, List** table, int* amount) {
  for (int i = 0; i < *amount; i++) {
    PrintList2File (table[i], database);
  }
  fclose (database);
}

void AddContact (List** table, int* amount, long long int (*Hash) (long long int)) {
  printf("Enter phone number and name:\n");
  Contact newContact = {-1, ""};
  scanf ("%lld ", &newContact.num);
  fgets (newContact.name, 31, stdin);
  newContact.name[strlen (newContact.name) - 1] = '\0';
  CreateContact (newContact, table, amount, Hash);
}

void DeleteNumber (long long int num, List** table, int* amount, long long int (*Hash) (long long int)) {
  Contact check = FindContact (num, table, amount, Hash);
  if (check.num < 0){
    printf("Contact does not exist\n");
    return;
  }
  long long int delKey = Hash (num);
  int count = FindList (table, *amount, delKey);
  Node* tmp = table[count]->tail;
  for (; tmp != NULL;) {
    if (tmp->person.num == num) {
      DeleteNode (tmp, table[count]);
      break;
    }
    tmp = tmp->next;
  }
  *amount -= 1;
  }

void CreateContact (Contact newContact, List** table, int* amount, long long int (*Hash) (long long int)) {
  Contact check = FindContact (newContact.num, table, amount, Hash);
  if (check.num > 0){
    printf("Contact already exist\n");
    return;
  }
  long long int newKey = Hash (newContact.num);
  int count = FindList (table, *amount, newKey);
  if (count >= 0) {
    PushHead (table[count], newContact);
  }
  else {
    table[*amount] = InitList ();
    table[*amount]->key = newKey;
    PushHead (table[*amount], newContact);
    *amount += 1;
  }
}

Contact FindContact (long long int number, List** table, int* amount, long long int (*Hash) (long long int)) {
  long long int key = Hash (number);
  Contact ret = {-1, ""};
  int count = FindList (table, *amount, key);
  if (count < 0)
    return ret;
  else
    ret = FindInList (count, table, number);
  return ret;
}

Contact FindInList(int count, List** table, long long int num) {
  Contact ret = {-1 , ""};
  Node* tmp = table[count]->tail;
  for (; tmp != NULL;) {
    if (tmp->person.num == num) {
      ret = tmp->person;
      break;
    }
    tmp = tmp->next;
  }
  return ret;
}

int FindList (List** table, int amount, long long int key) {
  int count = 0;
  for (;count < amount; count++){
    if (key == table[count]->key)
      return count;
  }
  count = -1;
  return count;
}

void FileRead (FILE* database, List** table, int* amount, long long int (*Hash) (long long int)) {
  Contact newContact = {-1, ""};
  for (int i = 0; fscanf (database, "%lld:", &newContact.num) != EOF; i++) {
    fgets (newContact.name, 31, database);
    newContact.name[strlen (newContact.name) - 1] = '\0';
    CreateContact (newContact, table, amount, Hash);
  }
  fclose (database);
}

long long int Hash_DigitSum (long long int num) {
  long long int hash = 0;
  for (;;) {
    hash += num % 10;
    if ((num /= 10) == 0)
      break;
  }
  return hash;
}

long long int Hash_XOR (long long int num) {
  long long int hash = (num ^ XOR_KEY) >> 24;
  return hash;
}

long long int Hash_HalfXOR (long long int num) {
  long long int hash = 0, halfnum = 0, tmp = num;
  int count = 0;
  for (; (tmp /= 10) != 0; count++);
  for (int i = 0; i < count / 2; i++) {
    halfnum += num % 10;
    halfnum *= 10;
    num /= 10;
  }
  hash = (num ^ halfnum) >> 14;
  return hash;
}

long long int Hash_Mod (long long int num) {
  long long int hash = num % MOD_KEY;
  return hash;
}
