#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BucketSize 8192
#define N 48

struct packet {
  int id;
  unsigned long size;
};

uint32_t hashes[BucketSize];
struct packet packet_buffer[N];
uint32_t expected[N];

// Key == Value
void fill_hashTable() {
  for (int i = 0; i < BucketSize; ++i) {
    hashes[i] = i;
  }
}

void fill_packet_buffer(struct packet ** pkts, int n) {
  for (int i = 0; i < n; ++i) {
    packet_buffer[i].id = i;
    packet_buffer[i].size = 32; // Fixed sized packets
  }
}

void test_loop_function() {
  int M = 10;
  int sum = 0;
  for (int i = 0; i < M; ++i) {
    for (int j = 0; j < M; ++j) {
      sum = sum + j;
    }
  }
}

void hash_lookup(struct packet * pkt) __attribute__((annotate("tas_batch"))){
  int n = N;
  for (int i = 0; i < n; ++i) {
    uint32_t h;
    h = hashes[pkt->id];
    printf("%d", h);
    printf("loop 2\n");
  }
  return;
}

/*
void hash_lookup_batched(struct packet ** pkts, uint16_t B) {
  uint32_t h[B];
  for (int i = 0; i < B; ++i) {
    h[i] = hashes[pkts[i]->id];
  }
}
*/

int main() {
  struct packet ** pkts_buf = malloc(N * sizeof(struct packet *));
  for (int i = 0; i < N; ++i)
    pkts_buf[i] = malloc(sizeof(struct packet));

  fill_hashTable();
  fill_packet_buffer(pkts_buf, N);

  for (int i = 0; i < N; ++i) {
    hash_lookup(pkts_buf[i]);
  }

  // Release memory
  for (int i = 0; i < N; ++i)
    free(pkts_buf[i]);
  free(pkts_buf);
  return EXIT_SUCCESS;
}
