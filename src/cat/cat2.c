#include <stdio.h>

char buf[128*1024];

int
main()
{
  size_t count;

  while ((count = fread(buf, 1, sizeof(buf), stdin)) > 0)
    fwrite(buf, 1, count, stdout);
  return 0;
}
