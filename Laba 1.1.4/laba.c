#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int DefaultNP = 220;

int CheckDataZeros ();
int ReadData ();
void CalculateData ();
void TwentyToFourty ();
double AverageValue ();
double Dispersion ();
void MinMax ();
void ShareCount ();
int WriteData ();

int main ()
{
  double avg10 = 0, avg40 = 0, delta10 = 0, delta40 = 0;
  int* n20 = (int*) calloc(DefaultNP, sizeof(int));
  int NPoints = 0, DCheck = -1, WCheck = -1;
  NPoints = ReadData (n20);
  if (NPoints <= 0) return -1;
  DCheck = CheckDataZeros (NPoints, n20);
  if (DCheck != 0) return -1;
  CalculateData (NPoints, n20, &avg10, &avg40, &delta10, &delta40);
  WCheck = WriteData (NPoints, avg10, avg40, delta10, delta40);
  if (WCheck != 0) return -1;
  return 0;
}

int ReadData (int n20[])
{
  int arrSize = DefaultNP;
  FILE* file = fopen ("input.in", "r");
  if (!file) {printf ("Can't open file \n"); return -1;}
  int point = 0;
  for (;;)
  {
    if (fscanf (file, "%d", &n20[point]) == EOF) break;
    point++;
    if (point > arrSize - 1)
    {
      n20 = (int*) realloc(n20, (point + 100) * sizeof(int));
      arrSize = point + 100;
    }
  }
  fclose (file);
  n20 = (int*) realloc(n20, (point) * sizeof(int));
  if (point <= 10)
  {
    printf("Number of measurements is incorrect\n");
    return -1;
  }
  return point;
}

int CheckDataZeros (int NPoints, int n20[])
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

void CalculateData (int NPoints, int n20[], double* avg10, double* avg40, double* delta10, double* delta40)
{
  int* n40 = (int*)calloc((int)(NPoints / 2), sizeof(int));
  int min40 = 0, max40 = 0;
  TwentyToFourty (NPoints, n20, n40);
  *avg10 = AverageValue (n20, NPoints) / 2;
  *avg40 = AverageValue (n40, (int)(NPoints / 2));
  *delta10 = sqrt(Dispersion(0, NPoints - 1, n20) / (2 * NPoints));
  *delta40 = sqrt(Dispersion(0, (int)(NPoints / 2) - 1, n40) / (NPoints / 2));
  MinMax (n40, &min40, &max40, (int)(NPoints / 2));
  double* share40 = (double*)calloc(max40 - min40 + 1, sizeof(double));
  ShareCount (NPoints, n40, share40, min40, max40);
  //Plot ();
}

void TwentyToFourty (int NPoints, int n20[], int n40[])
{
  int i = 0;
  for (;;)
  {
    if (i >= NPoints) break;
    n40[i / 2] = n20[i] + n20[i + 1];
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

void ShareCount (int NPoints, int n[], double share[], int min, int max)
{
  for (int i = 0; i <= max - min; i++)
  {
    for (int j = 0; j <= (int)(NPoints / 2) - 1; j++)
    {
      if (n[j] == i + min) share[i] += (double) 1 / (NPoints / 2);
    }
  }
}

void MinMax (int n[], int* min, int* max, int amnt)
{
  *min = n[0];
  *max = n[0];
  for (int i = 0; i <= amnt - 1; i++)
  {
    if (n[i] < *min) *min = n[i];
    if (n[i] > *max) *max = n[i];
  }
}

void Plot ()
{

}

int WriteData (int NPoints, double avg10, double avg40, double delta10, double delta40)
{
  FILE* res = fopen ("result.txt", "w");
  if (!res) {printf ("Can't write to file \n"); return -1;}
  fprintf(res, "+--------------------------------+------------------+------------------+\n");
  fprintf(res, "|                                |      10 сек      |      40 сек      |\n");
  fprintf(res, "+--------------------------------+------------------+------------------+\n");
  fprintf(res, "|   Среднее число срабатываний   |   %5.3lg ± %.1lg    |   %5.3lg ± %.1lg    |\n", avg10, delta10, avg40, delta40);
  fprintf(res, "+--------------------------------+------------------+------------------+\n");
  fprintf(res, "| Погрешность как корень ср. зн. |        %2.1lg       |        %2.1lg       |\n", sqrt(avg10 / (NPoints * 2)), sqrt(avg40 / (NPoints / 2)));
  fprintf(res, "+--------------------------------+-------------------------------------+\n");
  fclose (res);
  return 0;
}
