#include <stdio.h>
#include <stdlib.h>

void funcA() {
  int __attribute__((annotate("expensive"))) a;
  a = 10;
  printf("c = %d\n", a);
}

int main() {
  funcA();
  return 0;
};
