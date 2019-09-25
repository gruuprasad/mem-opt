#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define TAS_BATCH_START int TAS_batch_start __attribute__((annotate("batch_begin")));
#define TAS_BATCH_END int TAS_batch_end __attribute__((annotate("batch_end")));

int simple_fn(int a BATCH_ARG, int b BATCH_ARG) {
  int c;
  TAS_BATCH_START
  c = a + b;
  return c;
}

// t accumulates the value of iter, testing scenario.
int simple_fn_iter(int a BATCH_ARG, int b BATCH_ARG) {
  int iter_count = 0;
  int c = 0;
  TAS_BATCH_START
  iter_count++;
  c = a + b;
  return iter_count;
}
