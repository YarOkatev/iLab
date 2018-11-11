#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void func (int** a)
{
  //*a = (int*) realloc (*a, 10000 * sizeof(int));
  *a = (int*) calloc (10000, sizeof(int));
  for (int i = 0; i < 10000; i++)
  {
    *a[i] = (i);
  }
  for (int i = 0; i < 10000; i++)
  {
    printf("%d\n", *a[i]);
  }
  //a[15] = '\0';
}

int main ()
{
  int* a = (int*) calloc (1, sizeof(int));
  //int* a = NULL;
  /*for (int i = 0; i < 10; i++)
  {
    a[i] = (char) (70 + i);
  }
 a = realloc (a, 15 * sizeof(char));
  for (int i = 10; i < 15; i++)
  {
    a[i] = i * i;
  } */
  func (&a);
  //printf("\n %s \n", a);
  for (int i = 0; i < 10000; i++)
  {
    printf("%d ", a[i]);
  }
  /*for (int i = 0; i < 15; i++)
  {
    printf("%c\n", a[i]);
  }*/
}
