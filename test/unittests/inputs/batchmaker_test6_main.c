#include <stdio.h>
#include <stdlib.h>

struct packet {
  int field1;
  int field2;
};


int struct_type_fn(struct packet a, int b);
void struct_type_fn_batch(struct packet *, int *, int, int *);
void struct_type_multi_ret_fn(struct packet a, int b);
void struct_type_multi_ret_fn_batch(struct packet *, int *, int);
int struct_ptr_type_fn(struct packet *, int);
void struct_ptr_type_fn_batch(struct packet **, int *, int, int *);

int main() {
  int rc = 0;

  {
    // struct_type_fn
    int batch_size = 3;
    struct packet a[3] = { {1, 2}, {3, 4}, {5, 6} };
    int b[3] = { 3, 2, 10 };
    int ret[3] = { 0, 0, 0};
    int ret_batch[3] = { 0, 0, 0 };

    for (int i = 0; i < batch_size; ++i)
      ret[i] = struct_type_fn(a[i], b[i]);

    struct_type_fn_batch(a, b, batch_size, ret_batch);

    for (int i = 0; i < batch_size; ++i) {
      if (ret[i] != ret_batch[i]) {
        rc--; break;
      }
    }
  }

  {
    // struct_type_multi_ret_fn
    int batch_size = 3;
    struct packet a[3] = { {1, 2}, {3, 4}, {5, 6} };
    int b[3] = { 3, 2, 10 };

    for (int i = 0; i < batch_size; ++i)
      struct_type_multi_ret_fn(a[i], b[i]);

    struct_type_multi_ret_fn_batch(a, b, batch_size);
  }

  {
    // struct_ptr_type_fn
    // fn sets field1 = 1 and field2 = 2 for each packet.
    int batch_size = 3;
    struct packet * aptr[3];
    for (int i = 0; i < batch_size; ++i) {
      aptr[i] = (struct packet *) malloc(sizeof(struct packet));
      aptr[i]->field1 = 0;
      aptr[i]->field2 = 0;
    }
    int b[3] = { 3, 2, 10 };
    int ret[3] = { 0, 0, 0};
    int ret_batch[3] = { 0, 0, 0 };

    for (int i = 0; i < batch_size; ++i)
      ret[i] = struct_ptr_type_fn(aptr[i], b[i]);

    struct_ptr_type_fn_batch(aptr, b, batch_size, ret_batch);

    for (int i = 0; i < batch_size; ++i) {
      if (aptr[i]->field1 != 1 && aptr[i]->field2 != 2) {
        rc--; break;
      }
    }
    for (int i = 0; i < batch_size; ++i) {
      if (ret[i] != ret_batch[i]) {
        rc--; break;
      }
    }
  }

  return rc;
}
