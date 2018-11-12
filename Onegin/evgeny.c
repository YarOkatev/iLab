#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

const int DefSN = 1000;

typedef struct StringPlus
{
  char* str;
  int len;
} StrP;

char* FileRead ();
StrP* BetterData ();

int main ()
{
  int nSymb = 0;
  char* poem = NULL;
  StrP* text = NULL;
  poem = FileRead (&nSymb);
  text = BetterData (poem, nSymb);
  
  for (int i = 0; i <= 15; i++)
  {
    printf("%d= %s\n",text[i].len, text[i].str);
  }
  return 0;
}

char* FileRead (int* nSymb)
{
  FILE* filepoem = fopen ("poem.txt", "r");
  if (!filepoem)
  {
    printf ("Can't open file \n");
    fclose (filepoem);
    return NULL;
  }
  struct stat st = {};
  stat ("poem.txt", &st);
  *nSymb = (int) st.st_size;
  char* poem = (char*) calloc (*nSymb + 1, sizeof(char));
  fread (poem, sizeof(char), *nSymb, filepoem);
  fclose (filepoem);
  return poem;
}

StrP* BetterData (char* poem, int nSymb)
{
  StrP* text = (StrP*) calloc (DefSN, sizeof(StrP));
  int k = 0;
  text[k].str = poem;
  char* prev = poem - 1;
  for (int i = 0; i < nSymb; i++)
  {
    if (poem[i] == '\n')
    {
      text[k].len = poem + i - prev;
      k++;
      poem[i] = '\0';
      text[k].str = poem + i + 1;
      prev = poem + i;
    }
  }
  return text;
}
