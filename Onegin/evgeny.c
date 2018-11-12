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
void Sorting ();
int FileWrite ();

int main ()
{
  int nSymb = 0, nStrings = 0;
  char* poem = NULL;
  StrP* text = NULL;
  poem = FileRead (&nSymb);
  text = BetterData (poem, nSymb, &nStrings);
  //Sorting (text, poem);
  FileWrite (text, poem, nStrings);

  // printf("%c \n", text[i].str[text[i].len - 10]);
  return 0;
}

char* FileRead (int* nSymb)
{
  FILE* filepoem = fopen ("poem.txt", "r");
  if (!filepoem)
  {
    printf ("Can't open file \n");
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

StrP* BetterData (char* poem, int nSymb, int* nStrings)
{
  StrP* text = (StrP*) calloc (DefSN, sizeof(StrP));
  int k = 0, range = DefSN;
  text[k].str = poem;
  char* prev = poem - 1;
  for (int i = 0; i < nSymb; i++)
  {
    if (poem[i] == '\n')
    {
      text[k].len = poem + i - prev;
      k++;
      //poem[i] = '\0';
      text[k].str = poem + i + 1;
      prev = poem + i;
    }
    if (k == range - 1)
    {
      text = (StrP*) realloc (text, (range + DefSN) * sizeof(StrP));
      range += DefSN;
    }
  }
  text = (StrP*) realloc (text, (k + 1) * sizeof(StrP));
  *nStrings = k + 1;
  return text;
}

// void Sorting (StrP* text, char* poem)
// {
//   qsort (text,
// }

int FileWrite (StrP* text, char* poem, int nStrings)
{
  FILE* output = fopen ("out.txt", "w");
  if (!output)
  {
    printf ("Can't open file \n");
    return -1;
  }
  for (int i = 0; i < nStrings; i++)
  {
    fwrite (text[i].str, sizeof(char), text[i].len, stdout);
  }
  fclose (output);
  return 0;
}
