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
