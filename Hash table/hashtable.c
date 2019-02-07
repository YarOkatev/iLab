#include "hashtable.h"

List** TableInit () {
    List** table = (List**) calloc (TABLE_SIZE, sizeof(List*));
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = InitList ();
        table[i]->key = i;
    }
    return table;
}

void TableDelete (List** table) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        ListDelete (table[i]);
    }
    free (table);
}

void FileWrite (FILE* database, List** table) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    PrintList2File (table[i], database);
  }
  fclose (database);
}

void AddContact (List** table, long long int (*Hash) (long long int)) {
  printf("Enter phone number and name:\n");
  Contact newContact = {-1, ""};
  scanf ("%lld ", &newContact.num);
  fgets (newContact.name, MAXLEN - 1, stdin);
  newContact.name[strlen (newContact.name) - 1] = '\0';
  if (newContact.num == -1) {
    printf("Incorrect number\n");
    AddContact (table, Hash);
    return;
  }
  CreateContact (newContact, table, Hash);
}

void DeleteNumber (long long int num, List** table, long long int (*Hash) (long long int)) {
  Contact check = FindContact (num, table, Hash);
  if (check.num < 0){
    printf("Contact does not exist\n");
    return;
  }
  long long int delKey = Hash (num);
  int count = FindList (table, delKey);
  Node* tmp = table[count]->tail;
  for (; tmp != NULL;) {
    if (tmp->person.num == num) {
      DeleteNode (tmp, table[count]);
      break;
    }
    tmp = tmp->next;
  }
  }

void CreateContact (Contact newContact, List** table, long long int (*Hash) (long long int)) {
  Contact check = FindContact (newContact.num, table, Hash);
  if (check.num > 0) {
    printf("Contact already exist\n");
    return;
  }
  long long int newKey = Hash (newContact.num);
  int count = FindList (table, newKey);
  PushHead (table[count], newContact);
}

Contact FindContact (long long int number, List** table, long long int (*Hash) (long long int)) {
  long long int key = Hash (number);
  Contact ret = {-1, ""};
  int count = FindList (table, key);
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

int FindList (List** table, long long int key) {
  int count = 0;
  for (;count < TABLE_SIZE; count++){
    if (key == table[count]->key)
      return count;
  }
  count = -1;
  return count;
}

void FileRead (FILE* database, List** table, long long int (*Hash) (long long int)) {
  Contact newContact = {-1, ""};
  while (fscanf (database, "%lld:", &newContact.num) != EOF) {
    fgets (newContact.name, MAXLEN - 1, database);
    newContact.name[strlen (newContact.name) - 1] = '\0';
    CreateContact (newContact, table, Hash);
  }
  fclose (database);
  //rewind (database);
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
