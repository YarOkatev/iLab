#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct StrP
{
  char* str;
  int len;
} StringPlus;

int FileRead ();
void DataBeatify ();
int FileCount ();

int main ()
{
  int nSymb = 0;
  StringPlus* text = NULL;
  char* fcontent = NULL;
  //int RCheck = -1;
  FILE* poem;

  //FileCount (poem, &nSymb);

  FileRead (poem, fcontent, &nSymb);
  printf("\n second %d \n%s\n", nSymb, fcontent);
  DataBeatify (fcontent, nSymb, text);
  return 0;
}

int FileRead (FILE* poem, char fcontent, int* nSymb)
{
  /* FILE* poem = fopen ("poem.txt", "r"); // file in win (1251) or cp866
  if (!poem)
  {
    printf ("Can't open file \n");
    return -1;
  }
  struct stat st = {};
  stat ("poem.txt", &st);
  *nSymb = (int)st.st_size; */
  //fcontent = (char*) realloc (fcontent, (*nSymb + 1) * sizeof(char));
  fread (fcontent, sizeof(char), *nSymb, poem);
  fcontent[*nSymb] = '\0';
  printf("\nfirst\n%s\n", fcontent);
  return 0;
}

void DataBeatify (char* fcontent, int nSymb, StringPlus* text)
{

}

int FileCount (FILE* poem, int* nSymb)
{
  poem = fopen ("poem.txt", "r"); // file in win (1251) or cp866
  if (!poem)
  {
    printf ("Can't open file \n");
    return -1;
  }
  struct stat st = {};
  stat ("poem.txt", &st);
  *nSymb = (int)st.st_size;
}
