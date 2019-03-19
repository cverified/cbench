#define HEAPSIZE 1000

unsigned char heap[HEAPSIZE];
unsigned char *limit = heap;

unsigned char *
malloc(int n)
{
  unsigned char *ptr;

  if (n <= 0 || n > heap + HEAPSIZE - limit)
    return 0;
  ptr = limit;
  limit += n;
  return ptr;
}

int
main()
{
  return 0;
}
