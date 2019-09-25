#include <stdio.h>

int simple_fn(int a, int b);
void simple_fn_batch(int *, int *, int, int *); // Batch fn declaration.

int main() {
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

  if (ret == ret_batch)
    return 0;
  return -1;
}
