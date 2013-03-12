#include <stdio.h>
#include <bigint.h>


int main(int argc, char**args) {
  int radix = 10;
  char output[1024] = {0};
  const char*a = args[1];
  const char*b = args[2];
  int m, n;
  sscanf(a, "%d", &m);
  sscanf(b, "%d", &n);
  bigint_add(a, b, output, radix);
  printf("%s\n", output);
  printf("%d\n", m+n);
  output[0] = 0;
  bigint_mul(a, b, output, radix);
  printf("%s\n", output);
  printf("%d\n", m*n);
}
