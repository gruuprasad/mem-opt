#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define TAS_BATCH_START int TAS_batch_start __attribute__((annotate("batch_begin")));
#define TAS_BATCH_END int TAS_batch_end __attribute__((annotate("batch_end")));

struct packet {
  int field1;
  int field2;
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
