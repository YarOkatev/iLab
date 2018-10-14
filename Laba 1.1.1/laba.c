#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int  NPoints = 11, NWire = 3; //Number of points for each wire and number of wires
const double D = 0.356, DeltaD = 0.002;  // diameter in millimetres
const double L[NWire] = {0.5, 0.3, 0.2}, DeltaL = 0.005; // Lenght in metres

double Covariation (int Nstart, int Nfinish, double X[], double Y[]);
void CalculateData(int n, double U[], double I[], double p[], double deltaP[]);
int ReadData(double U[], double I[]);
int CheckData (double U[], double I[]);
double Dispersion (int Nstart, int Nfinish, double X[]);
int WriteData(double p[], double deltaP[]);

int main()
{
  double I[NPoints * NWire] = {}, U[NPoints * NWire] = {}, p[NWire] = {}, deltaP[NWire] = {};
  int RCheck = -1, DCheck = -1, WCheck = -1;
  RCheck = ReadData(U, I);
  if (RCheck != NPoints * NWire) {printf("Number of points mismatch \n"); return -1;};
  DCheck = CheckData(U, I);
  if (DCheck != 0) return -1;
  for (int n = 0; n <= NWire - 1; n++) CalculateData(n, U, I, p, deltaP);
  WCheck = WriteData(p, deltaP);
  if (WCheck != 0) return -1;
  return 0;
}

int ReadData(double U[], double I[])
{
  FILE* file = fopen ("input.in", "r");
  if (!file) {printf ("Can't open file \n"); return -1;}
  int line = 0;
  for (;;)
  {
    if (fscanf (file, "%lg %lg", &U[line], &I[line]) == EOF) break;
    line++;
  }
  fclose (file);
  return line;
}

int CheckData (double U[], double I[])
{
  int sumI = 0, sumU = 0;
  for (int i = 0; i < NWire * NPoints; i++) sumI += I[i];
  for (int i = 0; i < NWire * NPoints; i++) sumU += U[i];
  if ((abs(sumI) < 0.0000000001) || (abs(sumI) < 0.0000000001))
  {
    printf("Check your input data\n");
    return -1;
  }
  
  return 0;
}

void CalculateData (int n, double U[], double I[], double p[], double deltaP[])
{
  double cov = 0, dispI = 0, R = 0, sumI = 0, dispU = 0, deltaR = 0;
  cov =  Covariation ((n * NPoints), (NPoints * (n + 1)), U, I);
  dispI = Dispersion ((n * NPoints), (NPoints * (n + 1)), I);
  dispU = Dispersion ((n * NPoints), (NPoints * (n + 1)), U);
  R = cov / dispI;
  deltaR = sqrt((dispU / dispI - R * R) / (NPoints - 2));
  p[n] = R * 3.1415 * D * D / (4 * L[n]);
  deltaP[n] = p[n] * sqrt((deltaR / R) * (deltaR / R) + (2 * DeltaD / D) * (2 * DeltaD / D) + (DeltaL / L[n]) * (DeltaL / L[n]));
}

double Covariation (int Nstart, int Nfinish, double X[], double Y[])
{
  double sumX = 0, sumXY = 0, sumY = 0;
  int points = Nfinish - Nstart + 1;
  for (int i = Nstart; i < Nfinish; i++) sumX  += X[i];
  for (int i = Nstart; i < Nfinish; i++) sumY  += Y[i];
  for (int i = Nstart; i < Nfinish; i++) sumXY += Y[i] * X[i];
  return ((sumXY / points) - (sumX / points) * (sumY / points));
}

double Dispersion (int Nstart, int Nfinish, double X[])
{
  double sumdX = 0, sumX = 0;
  int points = Nfinish - Nstart + 1;
  for (int i = Nstart; i < Nfinish; i++) sumX  += X[i];
  for (int i = Nstart; i < Nfinish; i++) sumdX += (X[i] - (sumX / points)) * (X[i] - (sumX / points));
  return (sumdX / points);
}

int WriteData(double p[], double deltaP[])
{
  FILE* res = fopen ("result.txt", "w");
  if (!res) {printf ("Can't write to file \n"); return -1;}
  fprintf(res, "+------------------------------+------------------+------------------+------------------+\n");
  fprintf(res, "|                              |       50 см      |       30 см      |       20 см      |\n");
  fprintf(res, "+------------------------------+------------------+------------------+------------------+\n");
  fprintf(res, "|  Результаты измерений мОм*м  |   %4.3lg ± %.1lg    |    %.3lg ± %.1lg   |    %.3lg ± %.1lg   |\n", p[0], deltaP[0], p[1], deltaP[1], p[2], deltaP[2]);
  fprintf(res, "+------------------------------+------------------+------------------+------------------+\n");
  fprintf(res, "|     Ответ, Ом*м * 10^-6      |                       %.3lg ± %0.1lg                      |\n", (p[0] + p[1] + p[2]) / NWire, (deltaP[0] + deltaP[1] + deltaP[2]) / NWire);
  fprintf(res, "+------------------------------+--------------------------------------------------------+\n");
  fclose (res);
  return 0;
}
