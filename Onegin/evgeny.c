#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct StrP
{
  char* str;
  int len;
} StringPlus;

int FileRead ();

int main ()
{
  int nSymb = 0;
  StringPlus* text = NULL;
  int RCheck = -1;

  RCheck = FileRead (text, &nSymb);
  return 0;
}

int FileRead (StringPlus* text, int* nSymb)
{
  FILE* poem = fopen ("poem.txt", "r"); // file in win (1251) or cp866
  if (!poem)
  {
    printf ("Can't open file \n");
    return -1;
  }
  struct stat st = {};
  stat ("poem.txt", &st);
  *nSymb = (int)st.st_size;
  //printf ("%lld \n", sizeof(char));
  char* fcontent = (char*) calloc (*nSymb + 1, sizeof(char));
  fread (fcontent, sizeof(char), *nSymb, poem);
  fcontent[*nSymb] = '\0';
  printf("%s\n", fcontent);
  return 0;
}
