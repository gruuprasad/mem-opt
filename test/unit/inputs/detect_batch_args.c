/*
 * ${LLVM_PATH}/clang -S -emit-llvm -O1 -Xclang -disable-llvm-passes -c batch_process_test1.c
 */
#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define TAS_MAKE_BATCH __attribute__((annotate("tas_batch_maker")))

int test_fn(int c, int * b BATCH_ARG, int *a BATCH_ARG, int n) TAS_MAKE_BATCH {
  if (c > 10)
    return *a + 10;
  else
    return *b + 10;
}
