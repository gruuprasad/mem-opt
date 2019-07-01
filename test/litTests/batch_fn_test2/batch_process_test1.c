#include <stdio.h>
#include "batch_process_test1.h"

int singlularFn(int c, int * b BATCH_ARG, int *a BATCH_ARG) TAS_MAKE_BATCH {
  *b = 10;
  if (*a > 10) {
    *a = *a + c + 10;
  }
  else {
    *a = *a + c + 20;
  }

  return 0;
}

int batchRefFn(int c, int ** b, int **a, int n) {
  for (int i = 0; i < n; ++i) {
    if (*a[i] > 10) {
      *a[i] = *a[i] + c + 10;
    }
    else {
      *a[i] = *a[i] + c + 20;
    }
  }

  return 0;
}
