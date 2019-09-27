#include <stdio.h>
#include <stdlib.h>

int multiblock_fn(int a, int b);
int multiblock_fn_batch(int *, int *, int, int *);

int main() {
  int rc = 0;

  {
    // multiblock_fn
    int batch_size = 3;
    int a[3] = { 5, 15, 7 };
    int b[3] = { 3, 2, 10 };

    int ret[batch_size];
    for (int i = 0; i < batch_size; ++i)
      ret[i] = multiblock_fn(a[i], b[i]);

    int ret_batch[3];
    multiblock_fn_batch(a, b, batch_size, ret_batch);

    for (int i = 0; i < batch_size; ++i) {
      if (ret[i] != ret_batch[i])
        rc--; break;
    }
  }

  return rc;
}
