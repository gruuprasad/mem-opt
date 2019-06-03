#include <stdio.h>

#define EXPENSIVE __attribute__((annotate("expensive")))
#define TAS_BATCH __attribute__((annotate("tas_batch")))

int a_src[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int b_src[10] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

int tas_batch_process_fn(int n) TAS_BATCH {

  int * a EXPENSIVE;
  int * b EXPENSIVE;
  int c = 10; // for random computation purpose

  for (int i = 0; i < n; ++i) {
    printf("starting loop\n");
    c = c + 10;
    c = c % 8;
    a = &a_src[0];
    printf("value of a[0] = %d\n", a[0]);
    c = c / 2;
    c = c % 10;
    b = &b_src[0];
    printf("value of b[0] = %d\n", b[0]);
  }

  return 0;
}

int main() {

  tas_batch_process_fn(32);
  return 0;
}
