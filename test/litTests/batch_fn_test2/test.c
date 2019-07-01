#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "batch_process_test1.h"

int batch_fn(int, int **, int **, int, int *);
int main() {
  int a1 = 10;
  int a2 = 20;

  int b1 = 30;
  int b2 = 40;

  int* a_list[2] = {&a1, &a2};
  int* b_list[2] = {&b1, &b2};
  int c = singlularFn(50, &a1, &b1);
  int d = singlularFn(50, &a2, &b2);

  int * retVals = malloc(sizeof(int) * 2);
  //printf("==============\n");
  //printf("Calling batch_fn\n");
  batch_fn(50, a_list, b_list, 2, retVals);

  assert (c == retVals[0]);
  assert (d == retVals[1]);

  return 0;
}
