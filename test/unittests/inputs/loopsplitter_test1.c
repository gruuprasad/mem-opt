#include <stdlib.h>

#define EXPENSIVE __attribute__((annotate("expensive")))

// Function with single loop
int fn(int *in, int N) {
  int * a EXPENSIVE = in;
  int b = 0;
  for (int i = 0; i < N; ++i) {
    *a = *a + i;
    b = b + i;
  }

  return b;
}

int fn_ref(int *in, int N) {
  int * a = in;
  int b = 0;
  for (int i = 0; i < N; ++i) {
    *a = *a + i;
    b = b + i;
  }

  return b;
}

int main() {
  int rc = 0;
  {
    int N = 2;
    int * in = (int *) malloc(sizeof(int) * N);
    int * ref_in = (int *) malloc(sizeof(int) * N);
    int ret = fn(in, N);
    if (ret != fn_ref(ref_in, N))
      rc--;

    free(in);
    free(ref_in);
  }

  return rc;
}
