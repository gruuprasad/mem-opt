#include <stdio.h>
#include <stdlib.h>

int simple_fn(int a, int b);
void simple_fn_batch(int *, int *, int, int *); // Batch fn declaration.
int simple_fn_iter(int a, int b);
void simple_fn_batch(int *, int *, int, int *); // Batch fn declaration.
void simple_fn_iter_batch(int *, int *, int, int *); // Batch fn declaration.

int main() {
  int rc = 0;
  {
    // simple_fn
    int a = 2;
    int b = 3;
    int batch_size = 1;

    int ret = simple_fn(a, b); // ret = 5

    int ret_batch = 0;
    simple_fn_batch(&a, &b, batch_size, &ret_batch);

    /*
       printf("ret = %d\n", ret);
       printf("ret_batch = %d\n", ret_batch);
       */

    if (ret != ret_batch)
      rc--;
  }

  {
    // simple_fn_iter
    int a = 2;
    int b = 3;
    int batch_size = 5;

    int ret = simple_fn_iter(a, b); // ret = 1

    int ret_batch = 0; // Expected 5
    simple_fn_batch(&a, &b, batch_size, &ret_batch);

    if (batch_size != ret_batch)
      rc--;
  }

  return rc;
}
