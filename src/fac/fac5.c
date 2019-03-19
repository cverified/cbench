#include <stdio.h>
#include <stdlib.h>
#include "gmp.h"

int
main(int argc, char **argv)
{
  int n, i;
  mpz_t f;

  n = atoi(argv[1]);
  mpz_init(f);
  mpz_set_ui(f, 1);
  for (i = 1; i <= n ; i++)
    mpz_mul_ui(f, f, i);
  mpz_out_str(stdout, 10, f);
  putchar('\n');
  mpz_clear(f);
  return 0;
}
