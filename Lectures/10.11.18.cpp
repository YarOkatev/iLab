#include <stdio.h>

float Norm () {
  return 0;
}

struct vec3 {
  float x;
  float y;
  float z;
  float Norm ();
  vec3 Normate ();
  struct vec3 Rotate (float ph);
};

typedef vec3 vec3;
// float Norm () {
//   return 0;
// }

int main (){
  vec3 x = {1,2,3};
  printf("%lf\n", x.Norm());
  vec3 y = x.Normate();
  vec3 z = x.Rotate (34);
}

// float vec3::Norm() {
//
// }
