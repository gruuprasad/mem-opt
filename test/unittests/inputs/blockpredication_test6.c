#include <stdio.h>

#define TAS_MAKE_BATCH __attribute__((annotate("tas_batch_maker")))
#define BATCH_ARG __attribute__((annotate("batch_arg")))
#define EXPENSIVE __attribute__((annotate("expensive")))

struct packet {
  int id;
  int ip;
};

int fn(struct packet *);

int set_y(int in) {
  int y = in;
  return y;

}

int fn_ref(struct packet * in) {
  struct packet * p = in;
  int x = 0;
  int y = 0;
  int z = 0;
  if (p->id == 1) {
    x =10; 
  } else if (p->id == 2) {
    x =20; 
  } else if (p->id == 3) {
    x =30; 
  } else {
    x =40; 
    goto exit_;
  }

  if (x == 10)
    goto id_1;

  if (x == 20)
    return x;

  x++;

  if (p->ip == 100) {
    y = set_y(100);
    goto id_1;
  }

  if (x <= 20)
    x++;

  if (p->ip == 200) {
    y = set_y(200);
  } else {
    y = set_y(300);
    goto exit_;
  }

  if (z == 0)
    z++;

  if (y < 100)
    z = 1000;
  else
    z = 2000;

  if (y % 30 < 5) {
    x = x + 1;
  }

  if (x < 10)
    z = 10;
  else if (x == 10)
    z = 20;
  else
    z = 30;

  if (z == 30)
    goto id_1;

  x = x + y;

id_1:
  x++;
  z = 0;
  return x;
 
exit_:
  return -1;
}

int fn(struct packet * in) {
  struct packet * p = in;
  int x = 0;
  int y = 0;
  int z = 0;
  if (p->id == 1) {
    x =10; 
  } else if (p->id == 2) {
    x =20; 
  } else if (p->id == 3) {
    x =30; 
  } else {
    x =40; 
    goto exit_;
  }

  if (x == 10)
    goto id_1;

  if (x == 20)
    return x;

  x++;

  if (p->ip == 100) {
    y = set_y(100);
    goto id_1;
  }

  if (x <= 20)
    x++;

  if (p->ip == 200) {
    y = set_y(200);
  } else {
    y = set_y(300);
    goto exit_;
  }

  if (z == 0)
    z++;

  if (y < 100)
    z = 1000;
  else
    z = 2000;

  if (y % 30 < 5) {
    x = x + 1;
  }

  if (x < 10)
    z = 10;
  else if (x == 10)
    z = 20;
  else
    z = 30;

  if (z == 30)
    goto id_1;

  x = x + y;

id_1:
  x++;
  z = 0;
  return x;
 
exit_:
  return -1;
}

int main() {
  int rc = 0; // Absolute value gives how many failed.

  {
    struct packet p1 = { 1, 100 };
    int ret1 = fn(&p1);
    int ref1 = fn_ref(&p1);
    if (ret1 != ref1) {
      rc--;
    }
  }

  {
    struct packet p1 = { 2, 100 };
    int ret1 = fn(&p1);
    int ref1 = fn_ref(&p1);
    if (ret1 != ref1) {
      rc--;
    }
  }

  {
    struct packet p1 = { 3, 100 };
    int ret1 = fn(&p1);
    int ref1 = fn_ref(&p1);
    if (ret1 != ref1) {
      rc--;
    }
  }

  {
    struct packet p1 = { 4, 100 };
    int ret1 = fn(&p1);
    int ref1 = fn_ref(&p1);
    if (ret1 != ref1) {
      rc--;
    }
  }

return rc;
}
