#include <stdio.h>
#include "batch_process_test1.h"


// New prototye int tas_single_element_process_fn(int *a[], int N);

/* After transformation
int singlularFn(int *a[], int n) {
  for (int i = 0; i < n; ++i) {
    if (*a[i] > 10)
      *a[i] = *a[i] + 10;
    else
      *a[i] = *a[i] + 20;
  }

  return 0;
}
*/

int RefBatchFn(int d, int ** b, int ** a) {
  printf("%d\n", d);
  int c = 10;

  if (*a[0] > 10)
    *a[0] = *a[0] + 10;
  else
    *a[1] = *a[1] + 20;

  for (int i = 0; i < 2; ++i) {
    printf("%d\n", *a[i]);
  }
  for (int i = 0; i < 2; ++i) {
    printf("%d\n", *b[i]);
  }

  d += 10;

  return 0;
}

int batchFn1(int c, int ** b, int ** a) {
  c = c + 10;
  *b[0] = 10;
  if (*a[0] > 10)
    *a[0] = *a[0] + 10;
  else
    *a[0] = *a[0] + 20;
  c += 20;

  return 0;
}

int singlularFn(int c, int * b BATCH_ARG, int *a BATCH_ARG) TAS_MAKE_BATCH {
  *b = 10;
  if (*a > 10) {
    *a = *a + 10;
    c = c + 10;
  }
  else {
    *a = *a + 20;
    c = c + 20;
  }

  return c;
}

int test() {
  int a1 = 10;
  int a2 = 20;

  int b1 = 30;
  int b2 = 40;

  int* a_list[2] = {&a1, &a2};
  int* b_list[2] = {&b1, &b2};
  RefBatchFn(50, a_list, b_list);

  return 0;
}
