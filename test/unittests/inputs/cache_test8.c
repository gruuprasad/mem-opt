#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct packet {
  uint64_t ip;
  uint64_t id;
  uint64_t src;
  uint64_t dest;
};

struct pl_memory {
  struct packet pkts[32];
};

// Expected: #cachelines = 2
void test_fn(struct packet ** pkts) {
  /*
  printf("Size of struct packet = %lu\n", sizeof(struct packet));
  printf("Size of struct packet_state = %lu\n", sizeof(struct packet_state));
  printf("Size of struct metadata = %lu\n", sizeof(struct metadata));
  printf("Size of struct pl_memory = %lu\n", sizeof(struct pl_memory));
  */

  int N = 2;
  struct packet * p;
  for (int i = 0; i < N; ++i) {
    p = pkts[i];
    p->dest = 1;
    p->src = 2;
    p->id = 3;
    p->ip = 4;
  }
}

int caller_fn() {
  struct packet * pkt1 = (struct packet *) malloc(sizeof(struct packet));
  struct packet * pkt2 = (struct packet *) malloc(sizeof(struct packet));
  struct packet * pkts[2] = { pkt1, pkt2};
  test_fn(pkts);
  
  free(pkt1);
  free(pkt2);
  return 0;
}

int main() {
  caller_fn();
}
