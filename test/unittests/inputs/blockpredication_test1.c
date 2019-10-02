#include <stdio.h>

#define TAS_MAKE_BATCH __attribute__((annotate("tas_batch_maker")))
#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define EXPENSIVE __attribute__((annotate("expensive")))

int fn(int x) TAS_MAKE_BATCH {
  int y = 0;
  if (x == 0) {
    y = 1;
  } else {
    y = 3;
  }

  return y;
}

int fn_ref(int x) {
  int y = 0;
  if (x == 0) {
    y = 1;
  } else {
    y = 3;
  }

  return y;
}

int main() {
  int rc = 0;
  if (fn(0) != fn_ref(0)) rc--;
  if (fn(1) != fn_ref(1)) rc--;
  if (fn(2) != fn_ref(2)) rc--;
  return rc;
}
