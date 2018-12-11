#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int AMOUNT = 10000;

struct Contact {
  long long int num;
  char name[32];
};

int main () {
  FILE* book = fopen ("contacts.dat", "w");
  struct Contact phoneList;
  int space = 0, len = 0;
  srand (time (NULL));
  for (int i = 0; i < AMOUNT; i++) {
    phoneList.num = 79000000000 + rand () % 1000000000;
    phoneList.name[0] = (char) (rand () % 26 + 65);
    space = rand () % 8 + 5;
    len = rand () % 10 + 15;
    for (int j = 1; j < len; j++) {
      phoneList.name[j] = (char) (rand () % 26 + 97);
      if (j % space == 0) {
        phoneList.name[j] = ' ';
        j++;
        phoneList.name[j] = (char) (rand () % 26 + 65);
        space += 50;
      }
    }
    phoneList.name[len] = '\0';
    fprintf(book,"%lld:%s\n", phoneList.num, phoneList.name);
  }
  fclose (book);
  return 0;
}
