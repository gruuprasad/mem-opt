#include <stdio.h>
#include <stdlib.h>

void no_return_fn(int a, int b);
void no_return_fn_batch(int *, int *, int);
void multi_no_return_fn(int, int);
void multi_no_return_fn_batch(int *, int *, int);

int main() {
  int rc = 0;

  {
    // no_return_fn
    int batch_size = 3;
    int a[3] = { 5, 15, 7 };
    int b[3] = { 3, 2, 10 };

    for (int i = 0; i < batch_size; ++i)
      no_return_fn(a[i], b[i]);

    no_return_fn_batch(a, b, batch_size);

  }

  {
    // multi_no_return_fn
    int batch_size = 3;
    int a[3] = { 5, 15, 7 };
    int b[3] = { 3, 2, 10 };

    for (int i = 0; i < batch_size; ++i)
      multi_no_return_fn(a[i], b[i]);

    multi_no_return_fn_batch(a, b, batch_size);

  }
  return rc;
}
