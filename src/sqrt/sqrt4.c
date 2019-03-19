typedef float SFtype;

#define FRACBITS                24
#define EXPBITS                 8
#define EXPBIAS                 127
#define EXPMAX                  255

#define W_TYPE                  unsigned long
#define I_TYPE                  long
#define W_TYPE_SIZE             32

#define CLS_NORMAL              0
#define CLS_ZERO                1
#define CLS_INF                 2
#define CLS_NAN                 3

union UNION
{
  SFtype flt;
  struct STRUCT_LAYOUT
  {
    unsigned frac:FRACBITS - 1;
    unsigned exp:EXPBITS;
    unsigned sign:1;
  } bits;
};

SFtype
__sqrtsf2(SFtype a)
{
  SFtype r;
  union UNION RAW_flo;
  I_TYPE shift;
  I_TYPE A_c;
  I_TYPE A_s;
  I_TYPE A_e;
  W_TYPE A_f;
  I_TYPE R_c;
  I_TYPE R_s;
  I_TYPE R_e;
  W_TYPE R_f;
  W_TYPE S_f;
  W_TYPE T_f;
  W_TYPE q;

  RAW_flo.flt = a;
  A_s = RAW_flo.bits.sign;
  A_e = RAW_flo.bits.exp;
  A_f = RAW_flo.bits.frac;
  switch (A_e) {
  default:
    A_c = CLS_NORMAL;
    A_f |= ((W_TYPE) 1 << (FRACBITS - 1));
    A_f <<= 3;
    A_e -= EXPBIAS;
    break;
  case 0:
    if (A_f == 0)
      A_c = CLS_ZERO;
    else {
      shift = __builtin_clzl(A_f);
      shift -= (W_TYPE_SIZE - FRACBITS);
      A_c = CLS_NORMAL;
      A_f <<= (shift + 3);
      A_e -= (EXPBIAS - 1 + shift);
    }
    break;
  case EXPMAX:
    if (A_f == 0)
      A_c = CLS_INF;
    else
      A_c = CLS_NAN;
    break;
  }
  switch (A_c) {
  case CLS_NAN:
    R_c = CLS_NAN;
    R_s = A_s;
    R_f = A_f;
    break;
  case CLS_INF:
    if (A_s) {
      R_c = CLS_NAN;
      R_s = 1;
      R_f = ((W_TYPE) 1 << (FRACBITS - 2));
    } else {
      R_c = CLS_INF;
      R_s = 0;
    }
    break;
  case CLS_ZERO:
    R_c = CLS_ZERO;
    R_s = A_s;
    break;
  case CLS_NORMAL:
    if (A_s) {
      R_c = CLS_NAN;
      R_s = 1;
      R_f = ((W_TYPE) 1 << (FRACBITS - 2));
      break;
    }

    R_c = CLS_NORMAL;
    R_s = 0;
    R_e = A_e >> 1;
    if (A_e & 1)
      A_f += A_f;
    S_f = 0;
    R_f = 0;
    q = ((W_TYPE) 1 << (3 + FRACBITS)) >> 1;
    while (q != ((W_TYPE) 1 << 2)) {
      T_f = S_f + q;
      if (T_f <= A_f) {
        S_f = T_f + q;
        A_f -= T_f;
        R_f += q;
      }
      A_f += A_f;
      q >>= 1;
    }
    if (A_f) {
      if (S_f < A_f)
        R_f |= ((W_TYPE) 1 << 2);
      R_f |= ((W_TYPE) 1 << 0);
    }

  }
  switch (R_c) {
  case CLS_NORMAL:
    R_e += EXPBIAS;
    if (R_e > 0) {
      if (R_f & 7) {
        if ((R_f & 15) != ((W_TYPE) 1 << 2))
          R_f += ((W_TYPE) 1 << 2);
      }
      if (R_f & ((W_TYPE) 1 << (3 + FRACBITS))) {
        R_f &= ~((W_TYPE) 1 << (3 + FRACBITS));
        R_e++;
      }
      R_f >>= 3;
      if (R_e >= EXPMAX) {
        R_c = CLS_INF;
        R_e = EXPMAX;
        R_f = 0;
      }
    } else {
      R_e = -R_e + 1;
      if (R_e <= (3 + FRACBITS)) {
        R_f = (R_f >> R_e);
        if (R_f & 7) {
          if ((R_f & 15) != ((W_TYPE) 1 << 2))
            R_f += ((W_TYPE) 1 << 2);
        }
        if (R_f & (((W_TYPE) 1 << (3 + FRACBITS)) >> 1)) {
          R_e = 1;
          R_f = 0;
        } else {
          R_e = 0;
          R_f >>= 3;
        }
      } else {
        R_e = 0;
        if (R_f != 0) {
          R_f = 1;
          if ((R_f & 7) && ((R_f & 15) != ((W_TYPE) 1 << 2)))
            R_f += ((W_TYPE) 1 << 2);
          R_f >>= 3;
        }
      }
    }
    break;
  case CLS_ZERO:
    R_e = 0;
    R_f = 0;
    break;
  case CLS_INF:
    R_e = EXPMAX;
    R_f = 0;
    break;
  case CLS_NAN:
    R_e = EXPMAX;
    R_f |= ((W_TYPE) 1 << (FRACBITS - 2));
    break;
  }
  RAW_flo.bits.frac = R_f;
  RAW_flo.bits.exp = R_e;
  RAW_flo.bits.sign = R_s;
  r = RAW_flo.flt;
  return r;
}

#include <stdio.h>
#include <string.h>
#include <math.h>

int
main()
{
  int i;
  unsigned long l;
  float f, r0, r1;

  i = 1;
  for (l = 0; i || l; l++) {
    i = 0;
    *((unsigned long *) &f) = l;
    if (!(l % 1000000))
      fprintf(stderr, "%10lu\n", l);
    r0 = sqrt(f);
    r1 = __sqrtsf2(f);
    if (memcmp(&r0, &r1, sizeof(r0))) {
      printf("%f %f %f %08x %08x %08x \n", f, r0, r1,
        *((unsigned *) &f), *((unsigned *) &r0), *((unsigned *) &r1));
      fflush(stdout);
    }
  }
  return 0;
}
