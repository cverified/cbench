float
sqrt_approx(float x)
{
  union { float x; unsigned i; } u;

  u.x = x;
  u.i = (u.i >> 1) + 0x1fc00000;
  return u.x;
}

#include <stdio.h>
#include <math.h>

int
main()
{
  float x, y, z;

  for (x = 1.0; x <= 100.0; x += 1.0) {
    y = sqrt(x);
    z = sqrt_approx(x);
    printf("sqrt(%f) = %f = %f - %f (%f%%)\n", x, y, z, z - y, 100*(z - y)/y);
  }
  return 0;
}
