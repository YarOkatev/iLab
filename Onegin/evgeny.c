#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct StrP
{
  char** str;
  int len;
} StringPlus;

int FileRead ();
int FileInfo ();

int main ()
{
  FILE* filepoem = fopen ("poem.txt", "r");
  int nSymb = 0;
  //int RCheck = -1;
  FileInfo (filepoem, &nSymb);
  char* poem = (char*) calloc (nSymb + 1, sizeof(char));
  FileRead (filepoem, poem, nSymb);
  
  fclose (filepoem);
  printf("+++++++++++++++++++++++++%s\n", poem);
  return 0;
}

int FileInfo (FILE* filepoem, int* nSymb)
{
  if (!filepoem)
  {
    printf ("Can't open file \n");
    return -1;
  }
  struct stat st = {};
  stat ("poem.txt", &st);
  *nSymb = (int) st.st_size;
  return 0;
}

int FileRead (FILE* filepoem, char* poem, int nSymb)
{
  if (!filepoem)
  {
    printf ("Can't open file \n");
    return -1;
  }
  fread (poem, sizeof(char), nSymb, filepoem);
  poem[nSymb] = '\0';
  printf("%s\n", poem);
  return 0;
}
