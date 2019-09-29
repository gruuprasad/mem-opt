#include <stdio.h>

struct packet {
  int id;
  int ip;
};

int multi_conditionals_fn(struct packet *);

int set_y(int in) {
  int y = in;
  return y;
}

int multi_conditionals_fn_ref(struct packet * in) {
  struct packet * p = in;
  int x = 0;
  int y = 0;
  if (p->id == 1) {
    x =10; 
  } else if (p->id == 2) {
    x =20; 
  } else if (p->id == 3) {
    x =30; 
  } else {
    x =40; 
  }

  if (x == 10)
    goto id_1;

  if (x == 20)
    return x;

  x++;

  if (p->ip == 100) {
    y = set_y(100);
  }

  if (x <= 20)
    x++;

  if (p->ip == 200) {
    y = set_y(200);
  } else {
    y = set_y(300);
  }

  x = x + y;

id_1:
  x++;

  return x; // x value tells which set of blocks got executed.
}

int multi_conditionals_fn(struct packet * in) {
  struct packet * p = in;
  int x = 0;
  int y = 0;
  if (p->id == 1) {
    x =10; 
  } else if (p->id == 2) {
    x =20; 
  } else if (p->id == 3) {
    x =30; 
  } else {
    x =40; 
  }

  if (x == 10)
    goto id_1;

  if (x == 20)
    return x;

  x++;

  if (p->ip == 100) {
    y = set_y(100);
  }

  if (x <= 20)
    x++;

  if (p->ip == 200) {
    y = set_y(200);
  } else {
    y = set_y(300);
  }

  x = x + y;

id_1:
  x++;

  return x; // x value tells which set of blocks got executed.
}

int main() {
  int rc = 0; // Absolute value gives how many failed.

  {
    struct packet p1 = { 1, 100 };
    int ret1 = multi_conditionals_fn(&p1);
    int ref1 = multi_conditionals_fn_ref(&p1);
    if (ret1 != ref1) {
      printf("1:ret1 = %d\n", ret1);
      rc--;
    }
  }

  {
    struct packet p1 = { 2, 100 };
    int ret1 = multi_conditionals_fn(&p1);
    int ref1 = multi_conditionals_fn_ref(&p1);
    if (ret1 != ref1) {
      printf("2:ret1 = %d\n", ret1);
      rc--;
    }
  }

  {
    struct packet p1 = { 3, 100 };
    int ret1 = multi_conditionals_fn(&p1);
    int ref1 = multi_conditionals_fn_ref(&p1);
    if (ret1 != ref1) {
      printf("3:ret1 = %d\n", ret1);
      rc--;
    }
  }

  {
    struct packet p1 = { 4, 100 };
    int ret1 = multi_conditionals_fn(&p1);
    int ref1 = multi_conditionals_fn_ref(&p1);
    if (ret1 != ref1) {
      printf("4:ret1 = %d\n", ret1);
      rc--;
    }
  }

  printf("rc = %d\n", rc);

return rc;
}
