// packet fits in a single cache line
struct packet {
  int ip;
  int id;
  int src;
  int dest;
};

int process_packet(struct packet *);

// Function processing single packet
int process_packet(struct packet * in) {
  struct packet * p = in;
  int x = 0, y = 0, z = 0; // Dummy computation
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

  return 0;

slowpath:
  return -2;

unlock:
  return -1;
}
