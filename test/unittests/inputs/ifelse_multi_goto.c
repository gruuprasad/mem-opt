#include <stdlib.h>
#include <stdio.h>

#define TAS_MAKE_BATCH __attribute__((annotate("tas_batch_maker")))
#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define EXPENSIVE __attribute__((annotate("expensive")))

// packet fits in a single cache line
struct packet {
  int ip;
  int id;
  int src;
  int dest;
};

// Function processing single packet
int process_packet(struct packet * in BATCH_ARG) TAS_MAKE_BATCH {
  struct packet * p = in;
  int x = 0, y = 0, z = 0; // Dummy computation
  printf("packet ip = %d\n", p->ip);
  printf("packet id = %d\n", p->id);
  if (p->id == 1)
    goto unlock;

  if (p->id == 2)
    goto slowpath;

  // Mimic some operations done by packet when in fast path.
  // contains if, if-else control flow.
  if (x == 0) {
    x = x + 100;
    y = y + 200;
    z = x + y + 50;
    goto slowpath;
  }

  if (x > 50) {
    x = x - 50;
    y = y - 100;
  } else {
    z = x + y + 50;
  }

  printf("fastpath");
  return 0;

slowpath:
  printf("slowpath");
  return -2;

unlock:
  printf("unlock");
  return -1;
}

// Calling function
int caller_fn() {
  struct packet * p1 = (struct packet *) malloc(sizeof(struct packet));
  p1->ip = 1000;
  p1->id = 1;
  p1->src = 200;
  p1->dest = 300;

  process_packet(p1);

  free(p1);
  return 0;
}
