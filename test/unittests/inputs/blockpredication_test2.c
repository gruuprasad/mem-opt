int fn_ref(int);
int fn(int);

int fn_ref(int x) {
  int y = 0;
  int z = 0;
  if (x == 0)
    y = 10;
  else if (x == 1) {
    y = 20;
  } else {
    y = 30;
  }

  if (y == 10) {
    z = y + 1;
  } else if (y == 20) {
    z = y + 2;
  }

  if (y == 30)
    z = z + 3;

  return z;
}

int fn(int x) {
  int y = 0;
  int z = 0;
  if (x == 0)
    y = 10;
  else if (x == 1) {
    y = 20;
  } else {
    y = 30;
  }

  if (y == 10) {
    z = y + 1;
  } else if (y == 20) {
    z = y + 2;
  }

  if (y == 30)
    z = z + 3;

  return z;
}

int main() {
  int rc = 0;
  if (fn(0) != fn_ref(0)) rc--;
  if (fn(1) != fn_ref(1)) rc--;
  if (fn(2) != fn_ref(2)) rc--;
  return rc;
}
