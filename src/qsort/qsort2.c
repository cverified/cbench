int
random(int i, int j)
{
  return i + ((j - i) >> 1);
}

void
exchange(double *_x, double *_y)
{
  double z;

  z = *_x;
  *_x = *_y;
  *_y = z;
}

void
partition(double *a, int m, int n, int *_i, int *_j)
{
  double x;
  int f, i, j;

  f = random(m, n);
  x = a[f];
  i = m;
  j = n;
up:
  for (i = i; i <= n; i++)
    if (x < a[i])
      goto down;
  i = n;
down:
  for (j = j; j >= m; j--)
    if (a[j] < x)
      goto change;
  j = m;
change:
  if (i < j) {
    exchange(&a[i], &a[j]);
    i = i + 1;
    j = j - 1;
    goto up;
  } else
  if (i < f) {
    exchange(&a[i], &a[f]);
    i = i + 1;
  } else
  if (f < j) {
    exchange(&a[f], &a[j]);
    j = j - 1;
  }
  *_i = i;
  *_j = j;
}

void
quicksort(double *a, int m, int n)
{
  int i, j;

  if (m < n) {
    partition(a, m, n, &i, &j);
    quicksort(a, m, j);
    quicksort(a, i, n);
  }
}

int
main()
{
  return 0;
}
