#include <stdio.h>
#include <assert.h>
#include "batch_process_test1.h"

int batch_fn(int, int **, int **, int);
int main() {
  int a1 = 10;
  int a2 = 20;

  int b1 = 30;
  int b2 = 40;

  int * aa1 = &a1;
  int * bb1 = &b1;

  int* a_list[2] = {&a1, &a2};
  int* b_list[2] = {&b1, &b2};
  int c = singlularFn(50, &a1, &b1);
  int d = 0;
  d = batch_fn(50, &aa1, &bb1, 1);

  assert (c == d);

  return 0;
}
