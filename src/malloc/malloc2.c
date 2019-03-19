#include <stdio.h>

typedef struct { char contents[42]; } T;


#define HEAPSIZE 1000

union cell { T allocated; union cell *next_free; } heap[HEAPSIZE];
union cell *limit = heap, *first_free = 0;

T *
T_alloc()
{
  union cell *ptr = first_free;

  if (ptr) {
    first_free = first_free->next_free;
    return &(ptr->allocated);
  }
  if (limit >= heap + HEAPSIZE)
    return 0;
  ptr = limit++;
  return &(ptr->allocated);
}

void
T_free(T *T_ptr)
{
  union cell *ptr = (union cell *) T_ptr;

  ptr->next_free = first_free;
  first_free = ptr;
}


void
vstrcpy(char *dest, char *src)
{
  while (*src)
    *dest++ = *src++;
}

int
main()
{
  T *p;
  p = T_alloc();
  vstrcpy(p->contents, "foo");
  printf("%s\n", p->contents);
  T_free(p);
  return 0;
}
