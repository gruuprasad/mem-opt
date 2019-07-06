#include <stdio.h>
#include "batch_process_test1.h"

int singlularFn(int c, int * a BATCH_ARG EXPENSIVE, int * b BATCH_ARG EXPENSIVE) TAS_MAKE_BATCH {
  int x = 0;
  int z = 0;
  x = x + 30;
  z = z + x;
  int y = *a;
  if (y == 10) {
    printf("print a\n");
    //goto slowpath;
  }
  if (*b > 10) {
    printf("print b\n");
    *b = *b + c + 10;
    //goto unlock;
  }
  else {
    *b = *b + c + 20;
  }

  printf("=================\n");
  x = *a + 50;
  return 1;
}
