int
main()
{
  int s, r, n = 5, u, v;

  for (u = r = 1; v = u, r < n; r++)
    for (s = 1; u += v, s++ < r; )
      ;
  return u;
}
