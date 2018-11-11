#include <stdio.h>
#include <stdlib.h>

int main ()
{
  double* d = (double*) calloc (1, sizeof(double));
  //int* i = (int*) calloc (8, sizeof(int));
  scanf("%lg", &d[1]);
//  printf("%.8X ", (int)(d[1]));
  //char* i = (char*) calloc (8, 1);
  char* i = (char*) realloc (d, 8 * sizeof (char));
  for (int k = 0; k < 8; k++)
  {
    printf("%.8X ", *(i + k));
  }
}
