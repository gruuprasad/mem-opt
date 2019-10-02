// packet fits in a single cache line
struct packet {
  int ip;
  int id;
  int src;
  int dest;
}; 

int fn(struct packet *);
int fn_ref(struct packet *);

int main() {
  struct packet fastpath_packet1 = { 40, 3, 20, 30 };
  int ret1 = fn(&fastpath_packet1);
  int ref1 = fn_ref(&fastpath_packet1);

  struct packet fastpath_packet2 = { 40, 3, 20, 30 };
  int ret2 = fn(&fastpath_packet2);
  int ref2 = fn_ref(&fastpath_packet2);

  struct packet unlock_packet = { 20, 1, 20, 30 };
  int ret3 = fn(&unlock_packet);
  int ref3 = fn_ref(&unlock_packet);

  struct packet slowpath_packet = { 30, 2, 20, 30 };
  int ret4 = fn(&slowpath_packet);
  int ref4 = fn_ref(&slowpath_packet);

  if (ret1 != ref1 && ret2 != ref2 && ret3 != ref3 && ret4 != ref4)
    return -1;

  return 0;
}

// Function processing single packet
int fn(struct packet * in) {
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

// Function processing single packet
int fn_ref(struct packet * in) {
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
