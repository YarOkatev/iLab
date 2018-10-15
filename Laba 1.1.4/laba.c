#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int NPoints = 200;

int CheckDataZeros (int n20[]);
int ReadData (int n20[]);
void CalculateData (int n20[]);
void TwentyToFourty (int n20[], int n40[]);
double AverageValue (int n[], int amnt);
double Dispersion (int Nstart, int Nfinish, int X[]);

int main ()
{
  int n20[NPoints] = {};
  int RCheck = -1, DCheck = -1, WCheck = -1;
  RCheck = ReadData (n20);
  if (RCheck != 0) return -1;
  DCheck = CheckDataZeros (n20);
  if (DCheck != 0) return -1;
  CalculateData (n20);
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


void CalculateData (int n20[])
{
  int n40[NPoints / 2] = {};
  double avgN10 = 0, avgN40 = 0, deltaMeas10 = 0, deltaMeas40 = 0;
  TwentyToFourty (n20, n40);
  avgN10 = AverageValue (n20, NPoints) / 2;
  avgN40 = AverageValue (n40, (int)(NPoints / 2 + 0.5));
  deltaMeas10 = sqrt(Dispersion (0, NPoints - 1, n20) / 2);
  deltaMeas40 = sqrt(Dispersion (0, (int)(NPoints / 2 + 0.5) - 1, n40));

  printf(" %lg %lg\n",  deltaMeas10, deltaMeas40);
}

void TwentyToFourty (int n20[], int n40[])
{
  int i = 0;
  for (;;)
  {
    n40[i / 2] = n20[i] + n20[i + 1];
    if (i == NPoints - 2) break;
    i += 2;
  }
}

double AverageValue (int n[], int amnt)
{
  double avg = 0;
  for (int i = 0; i <= amnt - 1; i++)
  {
    avg = (avg * (double)i + n[i]) / (i + 1);
  }
  return avg;
}

double Dispersion (int Nstart, int Nfinish, int X[])
{
  double sumdX = 0, sumX = 0;
  int points = Nfinish - Nstart + 1;
  for (int i = Nstart; i <= Nfinish; i++) sumX  += X[i];
  for (int i = Nstart; i <= Nfinish; i++) sumdX += (X[i] - (sumX / points)) * (X[i] - (sumX / points));
  return (sumdX / points);
}
