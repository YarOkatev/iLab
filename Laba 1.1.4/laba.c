#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int NPoints = 200;

int CheckDataZeros (int n20[]);
int ReadData (int n20[]);

int main ()
{
  int n20[NPoints] = {};
  int RCheck = -1, DCheck = -1, WCheck = -1;
  RCheck = ReadData (n20);
  if (RCheck != 0) return -1;
  DCheck = CheckDataZeros (n20);
  if (DCheck != 0) return -1;
  return 0;
}

int ReadData (int n20[])
{
  FILE* file = fopen ("input.in", "r");
  if (!file) {printf ("Can't open file \n"); return -1;}
  int point = 0;
  for (;;)
  {
    if (fscanf (file, "%d", &n20[point]) == EOF) break;
    point++;
  }
  fclose (file);
  if (point != NPoints)
  {
    printf("Number of measurements mismatch\n");
    return -1;
  }
  return 0;
}

int CheckDataZeros (int n20[])
{
  int sumN = 0;
  for (int i = 0; i <= NPoints - 1; i++) sumN += n20[i];
  if (abs(sumN) == 0)
  {
    printf("Check your input data\n");
    return -1;
  }
  return 0;
}
