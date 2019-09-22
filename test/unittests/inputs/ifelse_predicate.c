int if_else_fn() {
  int x = 0;
  int y = 0;
  if (x == 0) {
    y = y + 1; //Path ID = 1
    goto done;
  } else {
    y = y + 2; // Path ID = 2
  }

  x = x + 1; // Path ID = 2

done:
  return 0;
}

/*
 * Transformed function:
int if_else_fn_predicated() {
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
