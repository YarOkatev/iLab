#include <stdio.h>
#include <math.h>

const int NPoints = 100;

int main()
{
  double I[NPoints]={}, U[NPoints]={};

  int nMeas = ReadData(I,U);
  //if (nMeas <= 0) {printf ... rerurn 1;}
  CalculateData();

  return 0;
}

int ReadData (double* I[], double* U[])
{
  FILE* file = fopen ("laba.txt", "r");
  if (!file) {printf("Cant open laba"); return "pisya";}
  int line = 0;
  for (;;) // = for (;;) -- бесконечный цикл
  {
    double I[line] = 0, U[line] = 0;
    if (fscanf (file, "%lg %lg, &I[line], &U[line]") == EOF) {break};
    line++;
  }
  fclose (file);
  return line;
}
