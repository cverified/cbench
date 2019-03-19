#include <stdio.h>

/* after all the macro violence, PRIM(fac) will be left in the output;
   one solution is to define these as ... existing nowhere */

extern int fac();
extern int (*PRIM())();

#define NOTHING
#define PRIM(f) f##_prim

/* NOTHING between PRIM and (f) prevens expansion of PRIM */
#define REC PRIM NOTHING

/* this simply forces the re-scanning of x (for macros) many times */
#define EVAL1(x) x
#define EVAL2(x) EVAL1(EVAL1(EVAL1(EVAL1(x))))
#define EVAL3(x) EVAL2(EVAL2(EVAL2(EVAL2(x))))

#define fac_prim(n) (n)<=0? 1 : (n)*(REC(fac)(n-1))
#define fac(n) EVAL3(PRIM(fac)(n))

int main()
{
    printf("%ld\n", fac(20L));
}
