#include <stdio.h>
#include <math.h>

const int  NPoints = 11, NWire = 3;
const double D = 0.356, DeltaD = 0.002;  // diameter in millimetres
const double L[NWire] = {0.5, 0.3, 0.2}, DeltaL = 0.005; // Lenght in metres

double CovariationUI (int n, double U[], double I[], double sumI);
void CalculateData(int n, double U[], double I[], double p[], double deltaP[]);
int ReadData(double U[], double I[]);
double DispersionI (int n, double I[], double sumI);
int WriteData(double p[], double deltaP[]);

int main()
{
  double I[NPoints * NWire] = {}, U[NPoints * NWire] = {}, p[NWire] = {}, deltaP[NWire] = {};
  int RCheck = ReadData(U, I);
  if (RCheck != NPoints * NWire) {printf("Data mismatch \n"); return 1;};
  for (int n = 0; n <= NWire - 1; n++) CalculateData(n, U, I, p, deltaP);
  int WCheck = WriteData(p, deltaP);
  if (WCheck != 0) return 1;
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

void CalculateData (int n, double U[], double I[], double p[], double deltaP[])
{
  double cov = 0, dispI = 0, r = 0, sumI = 0;
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumI += I[i];
  cov = CovariationUI (n, U, I, sumI);
  dispI = DispersionI (n, I, sumI);
  r = cov / dispI;
  p[n] = r * 3.1415 * D * D / (4 * L[n]);
  deltaP[n] = p[n] * sqrt((2 * DeltaD / D) * (2 * DeltaD / D) + (DeltaL / L[n]) * (DeltaL / L[n]));
}

double CovariationUI (int n, double U[], double I[], double sumI)
{
  double sumU = 0, sumUI = 0;
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumU  += U[i];
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumUI += U[i] * I[i];
  return ((sumUI / NPoints) - (sumI / NPoints) * (sumU / NPoints));
}

double DispersionI (int n, double I[], double sumI)
{
  double sumdI = 0;
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumdI += (I[i] - (sumI / NPoints)) * (I[i] - (sumI / NPoints));
  return (sumdI / NPoints);
}

int WriteData(double p[], double deltaP[])
{
  FILE* res = fopen ("result.txt", "w");
  if (!res) {printf ("Can't write to file \n"); return -1;}
  fprintf(res, "+----------------------+-----------+-----------+-----------+\n");
  fprintf(res, "|                      |       50 см       |       30 см       |       20 см       |\n");
  fprintf(res, "+----------------------+-----------+-----------+-----------+\n");
  fprintf(res, "| Результаты измерений |    %3.3lg ± %.1lg    |    %.3lg ± %.1lg   |    %.3lg ± %.1lg   |\n", p[0], deltaP[0], p[1], deltaP[1], p[2], deltaP[2]);
  fprintf(res, "+----------------------+-----------+-----------+-----------+\n");
  fprintf(res, "|      Ответ, Ом*м     |                %.3lg ± %0.2lg              |\n", (p[0] + p[1] + p[2]) / NWire, (deltaP[0] + deltaP[1] + deltaP[2]) / NWire);
  fprintf(res, "+----------------------+-----------------------------------+\n");
  fclose (res);
  return 0;
}
