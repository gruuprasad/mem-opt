int fn(int);
int fn_ref(int);


int fn_ref(int in) {
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

int fn(int in) {
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

int main() {
  int rc = 0;
  if (fn(0) != fn_ref(0)) rc--;
  if (fn(1) != fn_ref(1)) rc--;
  if (fn(2) != fn_ref(2)) rc--;
  if (fn(3) != fn_ref(3)) rc--;
  return rc;
}
