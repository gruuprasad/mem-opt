// packet fits in a single cache line
struct packet {
  int ip;
  int id;
  int src;
  int dest;
}; 

int process_packet(struct packet *);

int main() {
  struct packet fastpath_packet1 = { 40, 3, 20, 30 };
  int ret1 = process_packet(&fastpath_packet1);

  struct packet fastpath_packet2 = { 40, 3, 20, 30 };
  int ret2 = process_packet(&fastpath_packet2);

  struct packet unlock_packet = { 20, 1, 20, 30 };
  int ret3 = process_packet(&unlock_packet);

  struct packet slowpath_packet = { 30, 2, 20, 30 };
  int ret4 = process_packet(&slowpath_packet);

  if (ret1 != 50 && ret2 != 100 && ret3 != -1 && ret4 != -2)
    return -1;

  return 0;
}

// Function processing single packet
int process_packet(struct packet * in) {
  struct packet * p = in;
  int x = 0, y = 0, z = 0; // Dummy computation
  x = p->ip;
  if (p->id == 1)
    goto unlock;

  if (p->id == 2)
    goto slowpath;

  // Mimic some operations done by packet when in fast path.
  // contains if, if-else control flow.
  if (x == 0) {
    z = x + y + 50;
    goto slowpath;
  }

  if (x > 50) {
    z = 100;
  } else {
    z = 50;
  }

  return z;

slowpath:
  return -2;

unlock:
  return -1;
}
