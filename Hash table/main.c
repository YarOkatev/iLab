#include <stdio.h>
#include <stdlib.h>
#include "list.h"

long long int Hash_DigitSum (long long int num);
long long int Hash_XOR (long long int num);
long long int Hash_HalfXOR (long long int num);
long long int Hash_Mod (long long int num);

const long long int XOR_KEY = 2718281828, MOD_KEY = 33;

int main () {
  long long int num = 89195136321, hash = 0;
  hash = Hash_DigitSum (num);
  printf("%lld\n", hash);
  hash = Hash_XOR (num);
  printf("%lld\n", hash);
  hash = Hash_HalfXOR (num);
  printf("%lld\n", hash);
  hash = Hash_Mod (num);
  printf("%lld\n", hash);
  return 0;
}

long long int Hash_DigitSum (long long int num) {
  long long int hash = 0;
  for (;;) {
    hash += num % 10;
    if ((num /= 10) == 0)
      break;
  }
  return hash;
}

long long int Hash_XOR (long long int num) {
  long long int hash = num ^ XOR_KEY;
  return hash;
}

long long int Hash_HalfXOR (long long int num) {
  long long int hash = 0, halfnum = 0;
  int count = 0;
  for (; (num /= 10) == 0; count++);
  for (int i = count / 2; i >= 0; i--) {
    halfnum *= 10;
    halfnum += num % 10;
    num /= 10;
  }
  hash = num ^ halfnum;
  return hash;
}

long long int Hash_Mod (long long int num) {
  long long int hash = num / MOD_KEY;
  return hash;
}
