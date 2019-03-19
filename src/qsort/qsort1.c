#include <stdio.h>

#define N 666666
double a[N];

void
quicksort(int m, int n)
{
  int i, j;
  double pivot, tmp;

  if (m < n) {
    pivot = a[n];
    i = m; j = n;
    while (i <= j) {
      while (a[i] < pivot) i++;
      while (a[j] > pivot) j--;
      if (i <= j) {
        tmp = a[i]; a[i] = a[j]; a[j] = tmp;
        i++; j--;
      }
    }
    quicksort(m, j);
    quicksort(i, n);
  }
}

int
main()
{
  int i;

  for (i = 0; i < N; i++)
    a[i] = i;
  quicksort(0, N - 1);
  for (i = 0; i < N; i++)
    printf("%f\n", a[i]);
  return 0;
}
