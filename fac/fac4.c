int
fac(int n)
{
  return n == 0 ? 1 : n*fac(n - 1);
}

int
main()
{
  return fac(5);
}
