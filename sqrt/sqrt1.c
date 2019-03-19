float
sqrt_newton(float x)
{
  float y, z;

  if (x <= 0)
    return 0;
  y = x >= 1 ? x : 1;
  do {
    z = y;
    y = (z + x/z)/2;
  } while (y < z);
  return y;
}

#include <stdio.h>
#include <math.h>
#include <float.h>

int
main()
{
  double x, s0;
  float s1;

  printf("%18.10f\n", FLT_EPSILON);
  for (x = 0.01; x < 100; x += 0.01) {
    s0 = sqrt(x);
    s1 = sqrt_newton(x);
    if (s0 != s1)
      printf("%5.2f %12.10Lf\n%18.10Lf\n%18.10f\n",
        x, (long double) s0, (long double) s1, s0 - s1);
  }
  return 0;
}
