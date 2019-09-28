#include <stdio.h>

int if_else_fn(int);

int main() {
  int in1 = 0; //Expected out1 = 0
  int out1 = if_else_fn(in1);

  int in2 = 1; // Expected out2 = 1
  int out2 = if_else_fn(in2);

  if (out1 != 0 || out2 != 1) {
    return -1;
  }
  return 0;
}

int if_else_fn(int in) {
  int x = in;
  int y = 0;
  if (x == 0) {
    y = 0; //Path ID = 1
    goto done;
  } else {
    y = y + 1; // Path ID = 2
  }

  x = x + 1; // Path ID = 2

done:
  return y;
}

/*
 * Transformed function:
int if_else_fn() {
  int x = 0;
  int y = 0;
  int path = 0; // Holds path id
  if (x == 0) {
    path = 1;
  } else {
    path = 2;
  }

  if (path == 1) {
    y = y + 1;
  }

  // If basic blocks doesn't have else condition it can be merged.
  // I guess LLVM optimization pipeline look for such opportunities.
  if (path == 2) {
    y = y +2;
  }

  if (path == 2) {
    x = x + 1;
  }
  // goto target "done" is not used in the function.
  return 0;
}
*/
