#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 666666
double a[N];

#define THRESHOLD 0x2000
#define THREADS 16

pthread_mutex_t mutex;
int active = 1;

void quicksort(int, int, int);
struct quicksort_args {int m; int n;};

void *
run_quicksort(void *void_args)
{
  struct quicksort_args *args = void_args;

  quicksort(args->m, args->n, 1);
  return 0;
}

void
quicksort(int m, int n, int threaded)
{
  int i, j;
  double pivot, tmp;
  pthread_t thread;
  struct quicksort_args args;

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
    if (threaded) {
      if (n - m < THRESHOLD)
        threaded = 0;
      else {
        pthread_mutex_lock(&mutex);
        if (active < THREADS) {
          active++;
          pthread_mutex_unlock(&mutex);
          args.m = m;
          args.n = j;
          pthread_create(&thread, 0, &run_quicksort, &args);
          quicksort(i, n, 1);
          pthread_mutex_lock(&mutex);
          active--;
          pthread_mutex_unlock(&mutex);
          pthread_join(thread, 0);
          return;
        }
        pthread_mutex_unlock(&mutex);
      }
    }
    quicksort(m, j, threaded);
    quicksort(i, n, threaded);
  }
}

int
main()
{
  int i;

  pthread_mutex_init(&mutex, 0);
  for (i = 0; i < N; i++)
    a[i] = random();
  quicksort(0, N - 1, 1);
  for (i = 0; i < N; i++)
    printf("%f\n", a[i]);
  return 0;
}
