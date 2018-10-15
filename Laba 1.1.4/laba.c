#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int NPoints = 200;

int CheckDataZeros (int n20[]);
int ReadData (int n20[]);
void CalculateData (int n20[], double* avg10, double* avg40, double* delta10, double* delta40);
void TwentyToFourty (int n20[], int n40[]);
double AverageValue (int n[], int amnt);
double Dispersion (int Nstart, int Nfinish, int X[]);
void MinMax (int n[], int* min, int* max, int amnt);
void ShareCount ();
int WriteData (double avg10, double avg40, double delta10, double delta40);

int main ()
{
  double avg10 = 0, avg40 = 0, delta10 = 0, delta40 = 0;
  int* n20 = (int*) calloc(NPoints, sizeof(int));
  int RCheck = -1, DCheck = -1, WCheck = -1;
  RCheck = ReadData (n20);
  if (RCheck != 0) return -1;
  DCheck = CheckDataZeros (n20);
  if (DCheck != 0) return -1;
  CalculateData (n20, &avg10, &avg40, &delta10, &delta40);
  WCheck = WriteData (avg10, avg40, delta10, delta40);
  if (WCheck != 0) return -1;
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

void CalculateData (int n20[], double* avg10, double* avg40, double* delta10, double* delta40)
{
  int* n40 = (int*) calloc (NPoints / 2, sizeof(int));
  int min40 = 0, max40 = 0;
  TwentyToFourty (n20, n40);
  *avg10 = AverageValue (n20, NPoints) / 2;
  *avg40 = AverageValue (n40, (int)(NPoints / 2 + 0.5));
  *delta10 = sqrt(Dispersion (0, NPoints - 1, n20) / (2 * NPoints));
  *delta40 = sqrt(Dispersion (0, (int)(NPoints / 2 + 0.5) - 1, n40) / (NPoints / 2));
  MinMax (n40, &min40, &max40, (int)(NPoints / 2 + 0.5));
  double* share40 = (double*) calloc (max40 - min40 + 1, sizeof(double));
  ShareCount (n40, share40, min40, max40);
  //Plot ();
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

void ShareCount (int n[], double share[], int min, int max)
{
  for (int i = 0; i <= max - min; i++)
  {
    for (int j = 0; j <= NPoints / 2 - 1; j++)
    {
      if (n[j] == i + min) share[i] += (double) 1 / (NPoints / 2);
    }
  }
}

void MinMax (int n[], int* min, int* max, int amnt)
{
  *min = n[1];
  *max = n[1];
  for (int i = 0; i <= amnt - 1; i++)
  {
    if (n[i] < *min) *min = n[i];
    if (n[i] > *max) *max = n[i];
  }
}

void Plot ()
{

}

int WriteData (double avg10, double avg40, double delta10, double delta40)
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
