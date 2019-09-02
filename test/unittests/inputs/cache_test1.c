#include <stdlib.h>
#include "Common/Macros.h"

// packet fits in a single cache line
struct packet {
  int ip;
  int id;
  int src;
  int dest;
};



// case: When struct fits in a single cacheline
// Expected: #cachelines = 1
void test_fn(struct packet * const p1, struct packet * const p2) TAS_ANALYZE_CACHE {
  // Access all the struct members present on the heap.
  p1->ip = 1;
  p1->id = 10;
  p1->src = 2000;
  p1->dest = 4000;
}

int caller_fn() {

  struct packet * p1 = (struct packet *) malloc(sizeof(struct packet));
  struct packet * p2 = (struct packet *) malloc(sizeof(struct packet));

  test_fn(p1, p2);

  free(p1);
  free(p2);
  return 0;
}
