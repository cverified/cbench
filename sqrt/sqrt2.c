#define BITS(x) (*((unsigned *) &x))
#define FLOAT(x) (*((float *) &x))

float
sqrt_approx(float x)
{
  float one = 1;
  unsigned i;

  i = (BITS(x) >> 1) + BITS(one) - (BITS(one) >> 1);
  return FLOAT(i);
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
