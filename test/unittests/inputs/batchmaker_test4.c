#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define TAS_BATCH_START int TAS_batch_start __attribute__((annotate("batch_begin")));
#define TAS_BATCH_END int TAS_batch_end __attribute__((annotate("batch_end")));

int multiblock_fn(int a BATCH_ARG, int b BATCH_ARG) {
  int c, d;
  TAS_BATCH_START
  if (a > 10) {
    c = 10;
    b = 0;
  } else {
    c = a;
  }

  d = c * 2;
  return b;
}
