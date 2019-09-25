#define BATCH_ARG __attribute__((annotate("batch_arg")))

int simple_fn(int a BATCH_ARG, int b BATCH_ARG) {
  int c = 0;
  c = a + b;
  return c;
}
