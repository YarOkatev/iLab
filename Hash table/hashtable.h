#ifndef _HASH_H_INCLUDED_
#define _HASH_H_INCLUDED_

#include "list.h"

const long long int XOR_KEY = 2718281828, MOD_KEY = 100;
const int TABLE_SIZE = 100;

long long int Hash_DigitSum (long long int num);
long long int Hash_XOR (long long int num);
long long int Hash_HalfXOR (long long int num);
long long int Hash_Mod (long long int num);
List** TableInit ();
void TableDelete (List** table);
void FileRead (FILE* database, List** table, long long int (*Hash) (long long int));
int FindNum (long long int number, List** table, long long int (*Hash) (long long int));
Contact FindInList(int count, List** table, long long int num);
Contact FindContact (long long int number, List** table, long long int (*Hash) (long long int));
int FindList (List** table, long long int key);
void CreateContact (Contact newContact, List** table, long long int (*Hash) (long long int));
void DeleteNumber (long long int num, List** table, long long int (*Hash) (long long int));
void AddContact (List** table, long long int (*Hash) (long long int));
void FileWrite (FILE* database, List** table);

#include "hashtable.c"

#endif /* _HASH_H_INCLUDED_ */
