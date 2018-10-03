#include <stdio.h>

const int  NPoints = 11, NWire = 3;
const double D = 0.356;  // diameter in millimetres
const double L[NWire] = {0.5, 0.3, 0.2}; // Lenght in metres

double Covariation (int n, double U[], double I[]);
void CalculateData(int n, double U[], double I[], double r[]);
int ReadData(double U[], double I[]);
double Dispersion (int n, double I[]);
int WriteData(double r[]);

int main()
{
  double I[NPoints * NWire]={}, U[NPoints * NWire]={}, r[NWire]={};
  int RCheck = ReadData(U, I);
  if (RCheck != NPoints * NWire) {printf("Data mismatch \n"); return 1;};
  for (int n = 0; n <= NWire - 1; n++) {CalculateData(n, U, I, r);};
  int WCheck = WriteData(r);
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

void CalculateData (int n, double U[], double I[], double r[])
{
  double cov = 0, dispI = 0;
  cov = Covariation (n, U, I);
  dispI = Dispersion (n, I);
  r[n] = (cov / dispI) * 3.1415 * D * D / (4 * L[n]);
}

double Covariation (int n, double U[], double I[])
{
  double sumU = 0, sumI = 0, sumUI = 0;
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumU += U[i];
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumI += I[i];
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumUI += U[i] * I[i];
  return ((sumUI / NPoints) - (sumI / NPoints) * (sumU / NPoints));
}

double Dispersion (int n, double I[])
{
  double sumdI = 0, sumI = 0;
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumI += I[i];
  for (int i = n * NPoints; i <= NPoints * (n + 1) - 1; i++) sumdI += (I[i] - (sumI / NPoints)) * (I[i] - (sumI / NPoints));
  return (sumdI / NPoints);
}

int WriteData(double r[])
{
  FILE* res = fopen ("result.txt", "w");
  if (!res) {printf ("Can't write to file \n"); return -1;}
  fprintf(res, "+----------------------+-----------+-----------+-----------+\n");
  fprintf(res, "|                      |   50 см   |   30 см   |   20 см   |\n");
  fprintf(res, "+----------------------+-----------+-----------+-----------+\n");
  fprintf(res, "| Результаты измерений |    %.3lg    |    %.3lg   |    %.3lg   |\n", r[0], r[1], r[2] );
  fprintf(res, "+----------------------+-----------+-----------+-----------+\n");
  fprintf(res, "|      Ответ, Ом*м     |                %.3lg               |\n", (r[0] + r[1] + r[2]) / NWire );
  fprintf(res, "+----------------------+-----------------------------------+\n");
  fclose (res);
  return 0;
}
