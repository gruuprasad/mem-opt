#include <stdio.h>
#include <stdlib.h>

int funcA() __attribute__((annotate("tas_batch"))) {
  int __attribute__((annotate("expensive"))) * a;
  int b = 10;
  int c = 20;
  a = malloc(sizeof(int));

  c = c + (*a);
  printf("c = %d\n", c);

  free(a);
  return c;
}

void funcWithPhi(int r, int y) __attribute__((annotate("tas_attr1"))) {
  int l = y || r;
}

int main() {
  printf("hello World!\n");
  return 0;
};
