#include <stdlib.h>

#define TAS_MAKE_BATCH __attribute__((annotate("tas_batch_maker")))
#define BATCH_ARG __attribute__((annotate("batch_arg")))

// packet fits in a single cache line
struct packet {
  int ip;
  int id;
  int src;
  int dest;
};

// Test case 1
// desc: Detect tas function annotation
void test_fn(struct packet * const p1) TAS_MAKE_BATCH {
  // Access all the struct members present on the heap.
  p1->ip = 1;
  p1->id = 10;
  p1->src = 2000;
  p1->dest = 4000;
}

int test_fn2() TAS_MAKE_BATCH {
  return 0;
}

// Test case 2
int test_fn3(int * a, int * b1 BATCH_ARG) {
  return 0;
}

int test_fn4(int * a, int *b1 BATCH_ARG, int * b2 BATCH_ARG) {
  return 0;
}

int test_fn5(int * a, int *b1 BATCH_ARG, int c, int * b2 BATCH_ARG) {
  return 0;
}

// Calling function
int caller_fn() {

  struct packet * p1 = (struct packet *) malloc(sizeof(struct packet));

  test_fn(p1);

  free(p1);
  return 0;
}
