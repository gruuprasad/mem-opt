#include <stdio.h>

void funcA(int * a) {
  printf("size of a is %lu\n", sizeof(a));
}

int main() {
  int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  funcA(a);

  return 0;
}
