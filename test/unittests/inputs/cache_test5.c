#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "Macros.h"

struct packet {
  int ip;
  int id;
  int src;
  int dest;
};

struct metadata {
  uint64_t md1;
  uint64_t md2;
  uint64_t md3;
  uint64_t md4;
};

struct packet_state {
  uint64_t ps1_cl1;
  uint64_t ps2_cl1;
  uint64_t ps3_cl1;
  uint64_t ps4_cl1;
  struct metadata mdd1_cl1; // 1st cache line end
  uint64_t ps5_cl2;
  uint64_t ps6_cl2;
  uint64_t ps7_cl2;
  uint64_t ps8_cl2;
  uint64_t ps9_cl2;
  uint64_t pss1_cl2;
  uint64_t pss3_cl2;
  uint64_t pss4_cl2; // 2nd cache line end
  uint64_t pss5_cl3;
  uint64_t pss6_cl3;
  uint64_t pss7_cl3;
  uint64_t pss8_cl3;
  struct metadata mdd2_cl3; // 3rd cache line
};

struct pl_memory {
  struct packet pkts[32];
  struct packet_state pkt_states[32];
};

// Expected: #cachelines = 6
void test_fn(struct pl_memory * pl_state) TAS_ANALYZE_CACHE {
  /*
  printf("Size of struct packet = %lu\n", sizeof(struct packet));
  printf("Size of struct packet_state = %lu\n", sizeof(struct packet_state));
  printf("Size of struct metadata = %lu\n", sizeof(struct metadata));
  printf("Size of struct pl_memory = %lu\n", sizeof(struct pl_memory));
  */
  
  pl_state->pkts[1].ip = 3;
  pl_state->pkts[1].id = 2;
  pl_state->pkts[1].src = 4;
  pl_state->pkts[1].dest = 1;
  pl_state->pkts[15].dest = 15;

  pl_state->pkt_states[0].ps1_cl1 = 100;
  pl_state->pkt_states[2].ps1_cl1 = 100;
  pl_state->pkt_states[2].ps2_cl1 = 100;
  pl_state->pkt_states[2].mdd1_cl1.md2 = 300;
}

int caller_fn() {

  struct pl_memory * pl_state = (struct pl_memory *) malloc(sizeof(struct pl_memory));
  test_fn(pl_state);
  
  free(pl_state);
  return 0;
}

int main() {
  caller_fn();
}
