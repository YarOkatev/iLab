//
// Created by Ярослав Окатьев  on 2019-02-10.
//

#ifndef _HASH_C_INCLUDED_
#define _HASH_C_INCLUDED_

#include "hashtable.h"

List** tableInit () {
	List** table = (List**) calloc (TABLE_SIZE, sizeof(List*));
	for (int i = 0; i < TABLE_SIZE; i++) {
		table[i] = initList ();
		table[i]->key = i;
	}
	return table;
}

void tableDelete (List** table) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		listDelete (table[i]);
	}
	free (table);
}

void fileWrite (FILE* database, List** table) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		printListToFile (table[i], database);
	}
	fclose (database);
}

void addContact (List** table, long long int (*Hash) (long long int)) {
	printf("Enter phone number and name:\n");
	Contact newContact = {-1, ""};
	scanf ("%lld ", &newContact.num);
	fgets (newContact.name, MAXLEN - 1, stdin);
	newContact.name[strlen (newContact.name) - 1] = '\0';
	if (newContact.num == -1) {
		printf("Incorrect number\n");
		addContact (table, Hash);
		return;
	}
	createContact (newContact, table, Hash);
}

void deleteNumber (long long int num, List** table, long long int (*Hash) (long long int)) {
	Contact check = findContact (num, table, Hash);
	if (check.num < 0){
		printf("Contact does not exist\n");
		return;
	}
	long long int delKey = Hash (num);
	int count = findList (table, delKey);
	Node* tmp = table[count]->tail;
	for (; tmp != NULL;) {
		if (tmp->person.num == num) {
			deleteNode (tmp, table[count]);
			break;
		}
		tmp = tmp->next;
	}
}

void createContact (Contact newContact, List** table, long long int (*Hash) (long long int)) {
	Contact check = findContact (newContact.num, table, Hash);
	if (check.num > 0) {
		printf("Contact already exist\n");
		return;
	}
	long long int newKey = Hash (newContact.num);
	int count = findList (table, newKey);
	pushHead (table[count], newContact);
}

Contact findContact (long long int number, List** table, long long int (*Hash) (long long int)) {
	long long int key = Hash (number);
	Contact ret = {-1, ""};
	int count = findList (table, key);
	if (count < 0)
		return ret;
	else
		ret = findInList (count, table, number);
	return ret;
}

Contact findInList(int count, List** table, long long int num) {
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

int findList (List** table, long long int key) {
	int count = 0;
	for (;count < TABLE_SIZE; count++){
		if (key == table[count]->key)
			return count;
	}
	count = -1;
	return count;
}

void fileRead (FILE* database, List** table, long long int (*Hash) (long long int)) {
	Contact newContact = {-1, ""};
	while (fscanf (database, "%lld:", &newContact.num) != EOF) {
		fgets (newContact.name, MAXLEN - 1, database);
		newContact.name[strlen (newContact.name) - 1] = '\0';
		createContact (newContact, table, Hash);
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
	return hash % TABLE_SIZE;
}

long long int Hash_XOR (long long int num) {
	long long int hash = (num ^ XOR_KEY);
	return hash % TABLE_SIZE;
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
	hash = (num ^ halfnum);
	return hash % TABLE_SIZE;
}

long long int Hash_Mod (long long int num) {
	long long int hash = num % MOD_KEY;
	return hash % TABLE_SIZE;
}

#endif

