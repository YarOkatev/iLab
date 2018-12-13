#ifndef _HASH_H_INCLUDED_
#define _HASH_H_INCLUDED_

#include "list.h"

const long long int XOR_KEY = 2718281828, MOD_KEY = 100;

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

#include "hashtable.c"

#endif /* _HASH_H_INCLUDED_ */
