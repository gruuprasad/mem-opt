#include <stdio.h>

struct packet {
  int ip;
  int id;
};

int multi_conditionals_fn(struct packet *);

int set_y(int in) {
  int y = in;
  return y;
}

// Strategy to test: Define variables in each block uniquely.
// So checking the variable value tells which block got executed
// correctly during runtime after transformation.
int multi_conditionals_fn(struct packet * in) {
  struct packet * p = in;
  int x = 0;
  if (p->id == 1) {
    x =10; 
    printf("x = %d\n", x);
  } else if (p->id == 2) {
    x =20; 
    printf("x = %d\n", x);
  } else if (p->id == 3) {
    x =30; 
    printf("x = %d\n", x);
  } else {
    x =40; 
    printf("x = %d\n", x);
  }

  if (x == 10)
    goto id_1;

  if (x == 20)
    return x;

  /*
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
  */

id_1:
  x++;

  return x; // x value tells which set of blocks got executed.
}

int main() {
  int rc = 0;

  struct packet p1 = { 1, 100 };
  int ret1 = multi_conditionals_fn(&p1);

  printf("ret1 = %d\n", ret1);

  if (ret1 != 10)
    rc--;

return rc;
}
