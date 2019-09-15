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
  printf("packet ip = %d\n", p->ip);
  printf("packet id = %d\n", p->id);
  if (p->id == 1)
    goto unlock;

  if (p->id == 2)
    goto slowpath;

  printf("fastpath");
  return 0;

slowpath:
  printf("slowpath");
  return -2;

unlock:
  printf("unlock");
  return -1;
}

/*
// Stage 1:
// Change argument to accept list of values
// Index variable is introduced, but still single packet processing version
int process_packet_batch(struct packet ** in) {
  int i = 0;
  struct packet * p = in[i];
  printf("packet ip = %d\n", p->ip);
  printf("packet id = %d\n", p->id);
  if (p->id == 1)
    goto unlock;

  if (p->id == 2)
    goto slowpath;

  printf("fastpath");
  return 0;

slowpath:
  printf("slowpath");
  return -2;

unlock:
  printf("unlock");
  return -1;
}

// Stage 2
// Make single point exit of the function
// Store intermediate return variable in local variable.
int process_packet_batch(struct packet ** in) {
  int i = 0; // Batch-related
  int ret = 0; // Batch-related
  int x = 0;
  struct packet * p = in[i];
  printf("packet ip = %d\n", p->ip);
  printf("packet id = %d\n", p->id);
  if (p->id == 1) {
    x = x + 10;
    goto unlock;
  }

  if (p->id == 2) {
    x = x + 20;
    goto slowpath;
  }

  printf("fastpath");
  ret = 0;
  goto exit;

unlock:
  printf("unlock");
  ret = -1;
  goto exit;

slowpath:
  printf("slowpath");
  ret = -2;
  goto exit;

exit:
  printf("x = %d", x);
  return ret;
}

// Stage 3
// Linearize the control flow
int process_packet_batch(struct packet ** in) {
  int x = 0; // Dummy variable to represent arbitrary computation
  struct packet * p;
  int i = 0; // Batch-related
  int ret[1]; // Batch-related
  int packet_path_cond[1]; // 0 - NA, unlock - 1, slowpath - 2, exit - 3
  ret[0] = 0;
  packet_path_cond[0] = 0;
  p = in[i];
  printf("packet ip = %d\n", p->ip);
  printf("packet id = %d\n", p->id);

  if (packet_path_cond[i] == 0 && p->id == 1) {
    x = x + 10;
    packet_path_cond[i] = 1;
  }
  
  if (packet_path_cond[i] == 0 && p->id == 2) {
    x = x + 20;
    packet_path_cond[i] = 2;
  }

  if (packet_path_cond[i] == 0) {
    printf("fastpath");
    packet_path_cond[i] = 3;
    ret[i] = 0;
  }

unlock:
  if (packet_path_cond[i] == 1) {
   printf("unlock");
   ret[i] = -1;
  }

slowpath:
  if (packet_path_cond[i] == 2) {
    printf("slowpath");
    ret[i] = -2;
  }

exit:
  printf("x = %d", x);
  return ret;
}
*/
void process_packet_batch(struct packet **, int, int *);

// Calling function
int main() {
  struct packet * p1 = (struct packet *) malloc(sizeof(struct packet));
  p1->ip = 1000;
  p1->id = 1;
  p1->src = 200;
  p1->dest = 300;

  process_packet(p1);
  int * ret = NULL;
  process_packet_batch(&p1, 1, ret);

  free(p1);
  return 0;
}
