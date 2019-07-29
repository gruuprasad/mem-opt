#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "Macros.h"

struct packet {
  uint64_t ip;
  uint64_t id;
  uint64_t src;
  uint64_t dest;
};

struct pl_memory {
  struct packet pkts[32];
};

// Expected: #cachelines = 6
void test_fn(struct pl_memory ** pl_state) TAS_ANALYZE_CACHE {
  /*
  printf("Size of struct packet = %lu\n", sizeof(struct packet));
  printf("Size of struct packet_state = %lu\n", sizeof(struct packet_state));
  printf("Size of struct metadata = %lu\n", sizeof(struct metadata));
  printf("Size of struct pl_memory = %lu\n", sizeof(struct pl_memory));
  */
  
  // New Cache line
  pl_state[0]->pkts[1].ip = 3;
  pl_state[0]->pkts[1].id = 2;
  pl_state[0]->pkts[1].src = 4;
  pl_state[0]->pkts[1].dest = 1;

  // New Cache line
  pl_state[1]->pkts[1].ip = 3;
  pl_state[1]->pkts[1].id = 2;
  pl_state[1]->pkts[1].src = 4;
  pl_state[1]->pkts[1].dest = 1;

  // New Cache line
  pl_state[1]->pkts[30].ip = 3;

  // New Cache line
  pl_state[0]->pkts[31].dest = 3;
}

int caller_fn() {
  struct pl_memory * pl_state1 = (struct pl_memory *) malloc(sizeof(struct pl_memory));
  struct pl_memory * pl_state2 = (struct pl_memory *) malloc(sizeof(struct pl_memory));
  struct pl_memory * states[2] = { pl_state1, pl_state2};
  test_fn(states);
  
  free(pl_state1);
  free(pl_state2);
  return 0;
}

int main() {
  caller_fn();
}
