#include <stdio.h>

#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define TAS_BATCH_START int TAS_batch_start __attribute__((annotate("batch_begin")));
#define TAS_BATCH_END int TAS_batch_end __attribute__((annotate("batch_end")));

struct packet {
  int field1;
  int field2;
};

struct flow_group {
  int p;
  int q;
};

struct flow_state {
  int a;
  int b;
  int c;
  int d;
  struct flow_group fg;
};

int struct_type_fn(struct packet a BATCH_ARG, int b BATCH_ARG) {
  int c, d;
  TAS_BATCH_START
  if (a.field1 > 10) {
    c = 10;
    b = 0;
  } else {
    c = a.field2;
  }

  d = c * 2;
  return 1;
}

void struct_type_multi_ret_fn(struct packet a BATCH_ARG, int b BATCH_ARG) {
  int c, d;
  TAS_BATCH_START
  if (a.field1 > 10) {
    c = 10;
    b = 0;
    return;
  } else {
    c = a.field2;
  }

  d = c * 2;
  return;
}

int struct_ptr_type_fn(struct packet * a BATCH_ARG, int b BATCH_ARG) {
  int c, d;
  TAS_BATCH_START
  if (a->field1 < 10) {
    c = 10;
    b = a->field1;
  } else {
    c = a->field2;
  }

  a->field1 = 1;
  a->field2 = 2;

  d = c * 2;
  return 1;
}

int struct_ptr_flow_state(struct flow_state * a BATCH_ARG) {
  struct flow_state * fs;
  int i, j, k;
  TAS_BATCH_START
  fs = a;
  i = 0; j = 0; k = 0;

  if (fs->a != 0 &&
      fs->b != 1 &&
      fs->c != 2 &&
      fs->d != 3 &&
      fs->fg.p != 4 &&
      fs->fg.q != 5)
    return -1;

  return 0;
}
