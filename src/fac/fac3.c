int
fac(int n)
{
  int f = 1;

  while (n)
    f *= n--;
  return f;
}

int
main()
{
  return fac(5);
}
