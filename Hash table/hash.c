#include "stdio.h"
#include "stdlib.h"
#include "math.h"

const int KEY = 27181828;

int Hash (char* a);
int func1 (int x, int y, int z, int q, int t);
int func2 (int x, int y, int z, int q, int t);
int func3 (int x, int y, int z, int q, int t);
int func4 (int x, int y, int z, int q, int t);

int main () {
  char* e = (char*) calloc (256, sizeof(char));
  for (int i = 0; i < 2; i++) {

    Hash (e);
  }
}

int Hash (char* a) {
  unsigned int* hash = (unsigned int*) calloc (4, sizeof(unsigned int));
  char* shash = (char*) calloc (33, sizeof(char));
  int* b = (int*) a;
  for (int i = 0; i < 64; i++) {
    hash[0] += func1 (b[i], b[(i + 1) % 64], b[(i + 2) % 64], hash[3], hash[2]) + KEY * sin (i + 1);
    hash[1] += func2 (b[i], b[(i + 2) % 64], b[(i + 1) % 64], hash[0], hash[1]) + KEY * sin (2 * i);
    hash[2] += func3 (b[i], b[(i + 1) % 64], b[(i + 2) % 64], hash[3], hash[1]) + KEY * sin (3 * i);
    hash[3] += func4 (b[i], b[(i + 2) % 64], b[(i + 2) % 64], hash[0], hash[2]) + KEY * sin (4 * i);
  }
  sprintf (shash, "%X%X%X%X", hash[0], hash[1], hash[2], hash[3]);
  printf("%s %s\n", shash, a);
  free (hash);
  free (shash);
  return 0;
}

int func1 (int x, int y, int z, int q, int t) {
  int ret = ((KEY + x) & q) | (~y & z & q);
  return ret;
}

int func2 (int x, int y, int z, int q, int t) {
  int ret = (KEY | ~x | y) ^ (~KEY | z | q | ~t);
  return ret;
}

int func3 (int x, int y, int z, int q, int t) {
  int ret = (x ^ (KEY + ~y)) ^ z ^ ((~q | KEY) & t);
  return ret;
}

int func4 (int x, int y, int z, int q, int t) {
  int ret = ((KEY + q) & x & y & t) | ((~q | (y ^ ~x ^ z)) ^ t);
  return ret;
}
