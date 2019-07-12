#include <stdint.h>
#include <stdlib.h>

struct property {
  uint64_t uid; // 0
  uint64_t pid; // 8
};

struct metadata {
  uint64_t md1; // 0
  uint64_t md2; // 8
  uint64_t md3; // 16
  uint64_t md4; // 24
  struct property mdp1; // 32
};

struct packet_state {
  uint64_t ps1_cl1; // 0
  uint64_t ps2_cl1; // 8
  uint64_t ps3_cl1; // 16
  uint64_t ps4_cl1; // 24
  struct metadata mdd1_cl1; // 32
  uint64_t ps5_cl2; // 80
  uint64_t ps6_cl2; // 88
  uint64_t ps7_cl2; // 96
  uint64_t ps8_cl2; // 104
  uint64_t ps9_cl2; // 112
  uint64_t pss1_cl2; // 120
  uint64_t pss3_cl2; // 128
  uint64_t pss4_cl2; // 136
  uint64_t pss5_cl3; // 144
  uint64_t pss6_cl3; // 152
  uint64_t pss7_cl3; // 160
  uint64_t pss8_cl3; // 168
  struct metadata mdd2_cl3; // 176
};

// Case: Nested struct
// Expected: #cachelines = 6
void test_fn(struct packet_state * const p1_state, struct packet_state * const p2_state) {
  // Access member state in 1st cache line.
  p1_state->ps3_cl1 = 4000;    // p1 - cl 0

  // Access meber state in 2nd cache line.
  p1_state->pss5_cl3 = 8000;  // p1 - cl 1

  // Access p1_state, must be already in cache, hence not counted again.
  p1_state->ps1_cl1 = 1000;   // p1 - cl 0
  p1_state->mdd1_cl1.md3 = 300; // p1 - cl 0
  p1_state->mdd2_cl3.md3 = 300; // p1 - cl 2
  p1_state->mdd1_cl1.mdp1.uid = 1000;
  p1_state->mdd1_cl1.mdp1.pid = 1000;
  p1_state->mdd2_cl3.mdp1.uid = 2000;
  p1_state->mdd2_cl3.mdp1.pid = 2000;

  // Access partial state of packet
  p2_state->pss7_cl3 = 10000; // p2 - cl 1
  p2_state->mdd1_cl1.md1 = 500; // p2 - cl 0
  p2_state->pss7_cl3 = 600; // p2 - cl 2
  p2_state->mdd1_cl1.mdp1.pid = 5000;
}

int caller_fn() {
  struct packet_state * p1_state = (struct packet_state *) malloc(sizeof(struct packet_state));
  struct packet_state * p2_state = (struct packet_state *) malloc(sizeof(struct packet_state));

  test_fn(p1_state, p2_state);
  
  free(p1_state);
  free(p2_state);
  return 0;
}
