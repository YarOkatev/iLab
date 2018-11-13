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
int comp (const StrPlus *, const StrPlus *);

int main ()
{
  int nSymb = 0, nStrings = 0;
  char* poem = NULL;
  StrPlus* text = NULL;
  poem = FileRead (&nSymb);
  text = BetterData (poem, nSymb, &nStrings);
  Sorting (text, nStrings);
  FileWrite (text, poem, nStrings);
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
      //poem[i] = '\0';
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

int FileWrite (StrPlus* text, char* poem, int nStrings)
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
  int j = 0, i = 0, cmp = 0;
  char s1 = 0, s2 = 0;
  if (line1->str[0] == '\n')
    return 1;
  if (line2->str[0] == '\n')
    return -1;
  for (;;)
  {
    if (((int) line1->str[i] <= 64) || ((int) line1->str[i] >= 123) ||
                    (((int) line1->str[i] >= 91) && ((int) line1->str[i] <= 96)))
      { i++; continue; }
    if (((int) line2->str[j] <= 64) || ((int) line2->str[j] >= 123) ||
                    (((int) line2->str[j] >= 91) && ((int) line2->str[j] <= 96)))
      { j++; continue; }
    if (j == line2->len)
      break;
    if (j == line1->len)
      break;
    s1 = (line1->str[i] <= 90) ? (line1->str[i] + 32) : line1->str[i];
    s2 = (line2->str[j] <= 90) ? (line2->str[j] + 32) : line2->str[j];
    if (s1 > s2)
    {
      cmp = 1;
      break;
    }
    if (s2 > s1)
    {
      cmp = -1;
      break;
    }
    j++;
    i++;
    if (j == line2->len)
      break;
    if (j == line1->len)
      break;
  }
  return cmp;
}
