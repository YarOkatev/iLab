#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int ReadData();

int main ()
{
  char** text = NULL;
  char* line = NULL;
  int RCheck = -1;

  RCheck = ReadData (line, text);
  return 0;
}

int ReadData (char** text, char* line)
{
  FILE* poem = fopen ("ete.txt", "r");
  struct stat st = {};
  stat ("ete.txt", &st);
  printf ("%lld", st.st_size);
  line = realloc (line, st.st_size * sizeof(char));

  return 0;
}
