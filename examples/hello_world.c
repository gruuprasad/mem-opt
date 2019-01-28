#include <stdio.h>

int funcA() __attribute__((annotate("tas_attr1"))) {
  int __attribute__((annotate("tas_attr2"))) a = 10;
  int b = 20;
  int c;
  c = a + b;
  if (c > 10)
    c = 0;
  else
    c = 30;

  return c;
}

void funcWithPhi(int r, int y) __attribute__((annotate("tas_attr1"))) {
  int l = y || r;
}

int main() {
  printf("hello World!\n");
  return 0;
};
