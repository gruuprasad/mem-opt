#include <stdio.h>
#include "batch_process_test1.h"

int singlularFn(int c, int * a BATCH_ARG, int *b BATCH_ARG) TAS_MAKE_BATCH {
  int x;
  if (*a == 10) {
    //printf("goto slowpath\n");
    goto slowpath;
  }
  if (*b > 10) {
    //printf("goto unlock path\n");
    *b = *b + c + 10;
    goto unlock;
  }
  else {
    *b = *b + c + 20;
  }

  x = *a + 50;

unlock:
  x = x + 10;
  return 0;

slowpath:
  x = x + 30;
  return -1;
}
