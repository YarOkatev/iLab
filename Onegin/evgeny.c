#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

const int DefSN = 1000;

typedef struct StringPlus
{
  char* str;
  int len;
} StrPlus;

char* FileRead ();
StrPlus* BetterData ();
void Sorting ();
int FileWrite ();
void Convert ();
int comp ();

int main ()
{
  int nSymb = 0, nStrings = 0;
  char* poem = NULL;
  StrPlus* text = NULL;
  poem = FileRead (&nSymb);
  text = BetterData (poem, nSymb, &nStrings);
  Sorting (text, nStrings);
  FileWrite (text, nStrings);
  free (poem);
  free (text);
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

StrPlus* BetterData (char* poem, int nSymb, int* nStrings)
{
  StrPlus* text = (StrPlus*) calloc (DefSN, sizeof(StrPlus));
  int k = 0, range = DefSN;
  text[k].str = poem;
  char* prev = poem - 1;
  for (int i = 0; i < nSymb; i++)
  {
    if (poem[i] == '\n')
    {
      text[k].len = poem + i - prev;
      k++;
      text[k].str = poem + i + 1;
      prev = poem + i;
    }
    if (k == range - 1)
    {
      text = (StrPlus*) realloc (text, (range + DefSN) * sizeof(StrPlus));
      range += DefSN;
    }
  }
  text = (StrPlus*) realloc (text, (k + 1) * sizeof(StrPlus));
  *nStrings = k;
  return text;
}

void Sorting (StrPlus* text, int nStrings)
{
  qsort (text, nStrings, sizeof(StrPlus), (int(*) (const void *, const void *)) comp);
}

int FileWrite (StrPlus* text, int nStrings)
{
  FILE* output = fopen ("out.txt", "w");
  if (!output)
  {
    printf ("Can't open file \n");
    return -1;
  }
  for (int i = 0; i < nStrings; i++)
  {
    fwrite (text[i].str, sizeof(char), text[i].len, output);
  }
  fclose (output);
  return 0;
}

int comp (const StrPlus* line1, const StrPlus* line2)
{
  int j = 0, i = 0;
  char s1 = 0, s2 = 0;
  for (; i < line1->len && j < line2->len;)
  {
    s1 = line1->str[i];
    s2 = line2->str[j];
    Convert (&s1, &s2);
    if (s1 == -1)
    {
      i++;
      continue;
    }
    if (s2 == -1)
    {
      j++;
      continue;
    }
    if (s1 > s2)
      return 1;
    if (s2 > s1)
      return -1;
    j++;
    i++;
  }
  if (s1 == -1 && s2 != -1)
    return 1;
  if (s2 == -1 && s1 != -1)
    return -1;
  return 0;
}

void Convert (char* s1, char* s2)
{
  if ((*s1 <= 64) || (*s1 >= 123) || ((*s1 >= 91) && (*s1 <= 96)))
    *s1 = -1;
  if ((*s2 <= 64) || (*s2 >= 123) || ((*s2 >= 91) && (*s2 <= 96)))
    *s2 = -1;
  *s1 = (*s1 <= 90 && *s1 >= 65) ? (*s1 + 32) : *s1;
  *s2 = (*s2 <= 90 && *s2 >= 65) ? (*s2 + 32) : *s2;
}
