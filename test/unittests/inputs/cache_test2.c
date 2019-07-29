#include <stdint.h>
#include <stdlib.h>
#include "Macros.h"

struct packet_state {
  uint64_t ps1;
  uint64_t ps2;
  uint64_t ps3;
  uint64_t ps4;
  uint64_t ps5;
  uint64_t ps6;
  uint64_t ps7;
  uint64_t ps8; // 1st Cache line
  uint64_t ps9;
  uint64_t pss1;
  uint64_t pss3;
  uint64_t pss4;
  uint64_t pss5;
  uint64_t pss6;
  uint64_t pss7;
  uint64_t pss8; // 2nd Cache line
};

// Case: When each struct takes more than 1 cacheline
// Expected: #cachelines = 3
void test_fn(struct packet_state * const p1_state, struct packet_state * const p2_state) TAS_ANALYZE_CACHE {
  // Access member state in 1st cache line.
  p1_state->ps3 = 4000;

  // Access meber state in 2nd cache line.
  p1_state->pss5 = 8000;

  // Access partial state of packet
  p2_state->pss7 = 10000;

  // Access p1_state, must be already in cache, hence not counted again.
  p1_state->ps1 = 1000;
}

int caller_fn() {
  struct packet_state * p1_state = (struct packet_state *) malloc(sizeof(struct packet_state));
  struct packet_state * p2_state = (struct packet_state *) malloc(sizeof(struct packet_state));

  test_fn(p1_state, p2_state);
  
  free(p1_state);
  free(p2_state);
  return 0;
}
