#define SWAP(a, b) do { tmp = *a; *a = *b; *b = tmp; } while(0)

void
quicksort(double *base_ptr, int total_elems)
{
  double tmp, *lo, *hi, *left_ptr, *right_ptr, *mid;

  if (total_elems == 0)
    return;
  lo = base_ptr;
  hi = lo + total_elems - 1;
  while (lo < hi) {
    mid = lo + ((hi - lo) >> 1);
    if (*mid < *lo)
      SWAP(mid, lo);
    if (*hi < *mid) {
      SWAP(mid, hi);
      if (*mid < *lo)
        SWAP(mid, lo);
    }
    left_ptr = lo + 1;
    right_ptr = hi - 1;
    do {
      while (*left_ptr < *mid)
        left_ptr++;
      while (*mid < *right_ptr)
        right_ptr--;
      if (left_ptr < right_ptr) {
        SWAP(left_ptr, right_ptr);
        if (mid == left_ptr)
          mid = right_ptr;
        else if (mid == right_ptr)
          mid = left_ptr;
        left_ptr++;
        right_ptr--;
      } else if (left_ptr == right_ptr) {
        left_ptr++;
        right_ptr--;
        break;
      }
    } while (left_ptr <= right_ptr);
    if (right_ptr - lo > hi - left_ptr) {
      quicksort(left_ptr, hi - left_ptr + 1);
      hi = right_ptr;
    } else {
      quicksort(lo, right_ptr - lo + 1);
      lo = left_ptr;
    }
  }
}

int
main()
{
  return 0;
}
