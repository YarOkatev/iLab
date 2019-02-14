//
// Created by Ярослав Окатьев  on 2019-02-10.
//

#ifndef HASHTABLE_HASHTABLE_H
#define HASHTABLE_HASHTABLE_H

#include "list.h"

const long long int XOR_KEY = 2718281828, MOD_KEY = 100;
const int TABLE_SIZE = 100;

long long int Hash_DigitSum (long long int num);
long long int Hash_XOR (long long int num);
long long int Hash_HalfXOR (long long int num);
long long int Hash_Mod (long long int num);
List** tableInit ();
void tableDelete (List** table);
void fileRead (FILE* database, List** table, long long int (*Hash) (long long int));
int findNumber (long long int number, List** table, long long int (*Hash) (long long int));
Contact findInList (int count, List** table, long long int num);
Contact findContact (long long int number, List** table, long long int (*Hash) (long long int));
int findList (List** table, long long int key);
void createContact (Contact newContact, List** table, long long int (*Hash) (long long int));
void deleteNumber (long long int num, List** table, long long int (*Hash) (long long int));
void addContact (List** table, long long int (*Hash) (long long int));
void fileWrite (FILE* database, List** table);

#include "hashtable.c"

#endif //HASHTABLE_HASHTABLE_H
