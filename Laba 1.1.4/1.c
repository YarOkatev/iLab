#include <stdio.h>
#include <math.h>
#include <stdlib.h>
void mmm(int a[])
{
  int i = 0;
  for (;;)
  {
    printf("%d\n", a[i]);
    i++;
    if (i == 3) break;
  }

  a = (int*)realloc(a, 5*sizeof(int));
  a[3] = 228;
  i = 0;
  for (;;)
  {
    printf("s %d\n", a[i]);
    i++;
    if (i == 4) break;
}
    a = (int*)realloc(a, 4*sizeof(int));
}

int main()
{
  int* a = (int*)calloc(3, sizeof(int));
  int i = 0;
  for (;;)
  {
    a[i] = i*i;
    i++;
    if (i == 3) break;
  }

  i = 0;
  for (;;)
  {
    printf("%d\n", a[i]);
    i++;
    if (i == 3) break;
  }
mmm(a);

    i = 0;
    for (;;)
    {
      printf("ss %d\n", a[i]);
      i++;
      if (i == 4) break;
    }
  }
