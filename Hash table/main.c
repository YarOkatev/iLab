#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

const long long int XOR_KEY = 2718281828, MOD_KEY = 20;

long long int Hash_DigitSum (long long int num);
long long int Hash_XOR (long long int num);
long long int Hash_HalfXOR (long long int num);
long long int Hash_Mod (long long int num);
void FileRead (List** table, int* amount, long long int (*Hash) (long long int));
int FindNum (long long int number, List** table, int amount, long long int (*Hash) (long long int));
Contact FindInList(int count, List** table, long long int num);
Contact FindContact (long long int number, List** table, int* amount, long long int (*Hash) (long long int));
int FindList (List** table, int amount, long long int key);
void AddContact (Contact newContact, List** table, int* amount, long long int (*Hash) (long long int));

long long int (*MainHash) (long long int) = Hash_XOR;

//----------------------------------------------------------------------------//

int main () {
  Contact newContact = {11111111111,"Alex"};
  List* *table = (List**) calloc (10000, sizeof(List*));
  int amount = 0;
  FileRead (table, &amount, MainHash);

  AddContact (newContact, table, &amount, MainHash);
  printf("amount %d\n", amount );

  for (int i = 0; i < 10; i++) {
    printf("\n%d\n", i);
    printf("KEY %lld\n", table[i]->key);
    printf("SIZE %d\n", table[i]->size);
    //OutputList (table[i]);
  }

  return 0;
}

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//


void AddContact (Contact newContact, List** table, int* amount, long long int (*Hash) (long long int)) {
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

void FileRead (List** table, int* amount, long long int (*Hash) (long long int)) {
  FILE* database = fopen ("contacts.dat", "r");
  Contact newContact = {-1, ""};
  for (int i = 0; fscanf (database, "%lld:", &newContact.num) != EOF; i++) {
    fgets (newContact.name, 31, database);
    newContact.name[strlen (newContact.name) - 1] = '\0';
    //printf("%lld %s\n",newContact.num,newContact.name );
    AddContact (newContact, table, amount, Hash);
  }
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
  long long int hash = num ^ XOR_KEY;
  return hash;
}

long long int Hash_HalfXOR (long long int num) {
  long long int hash = 0, halfnum = 0;
  int count = 0;
  for (; (num /= 10) != 0; count++);
  for (int i = count / 2 + 1; i >= 0; i--) {
    halfnum *= 10;
    halfnum += num % 10;
    num /= 10;
  }
  hash = num ^ halfnum;
  return hash;
}

long long int Hash_Mod (long long int num) {
  long long int hash = num % MOD_KEY;
  return hash;
}
