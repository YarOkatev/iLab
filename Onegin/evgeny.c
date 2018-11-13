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
  FileWrite (text, nStrings);
  //SortingEnd ()
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

// void SortingEnd (StringPlus* text, int nStrings)
// {
//   qsort (text, nStrings, sizeof(StrPlus), (int(*) (const void *, const void *)) comp2);
// }

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
  printf("\n");
  if (line1->str[0] == '\n')
    return 1;
  if (line2->str[0] == '\n')
    return -1;
  for (;;)
  {
    //printf("* %c %c *     ", line1->str[i], line2->str[j]);
    if ((line1->str[i] <= 64) || (line1->str[i] >= 123) ||
                    ((line1->str[i] >= 91) && (line1->str[i] <= 96)))
    {
      i++;
      if (i == line1->len)
        break;
      continue;
    }
    if ((line2->str[j] <= 64) || (line2->str[j] >= 123) ||
                    ((line2->str[j] >= 91) && (line2->str[j] <= 96)))
    {
      j++;
      if (j == line2->len)
        break;
      continue;
    }
    s1 = (line1->str[i] <= 90) ? (line1->str[i] + 32) : line1->str[i];
    s2 = (line2->str[j] <= 90) ? (line2->str[j] + 32) : line2->str[j];
    //printf("| %c %c |\n", s1, s2);
    if (s1 > s2)
      return 1;
    if (s2 > s1)
      return -1;
    j++;
    i++;
    if (j == line2->len)
      break;
    if (j == line1->len)
      break;
  }
  // printf("| %c %c |\n", s1, s2);
  // if (s2 == 0 && s1 == 0)
  //   return 1;
  // if (s1 == 0 && s2 != 0)
  //   return -1;
  return 0;
}

// int comp2 (const StrPlus* line1, const StrPlus* line2)
// {
//   int j = 0, i = 0, cmp = 0;
//   char s1 = 0, s2 = 0;
//   if (line1->str[0] == '\n')
//     return 1;
//   if (line2->str[0] == '\n')
//     return -1;
// }
