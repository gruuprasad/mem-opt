#ifndef BATCH_PROCESS_TEST_H
#define BATCH_PROCESS_TEST_H

#define EXPENSIVE __attribute__((annotate("expensive")))
#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define TAS_BATCH __attribute__((annotate("tas_batch")))
#define TAS_MAKE_BATCH __attribute__((annotate("tas_batch_maker")))

int singlularFn(int c, int * b BATCH_ARG, int *a BATCH_ARG) TAS_MAKE_BATCH;
#endif
