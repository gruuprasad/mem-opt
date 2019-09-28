int main() {
  int x = 0;
  if (x == 0)
    goto exit;
  else
    x = 2;

  x = 1;

exit:
  x = -1;
  return 0;
}

int main_transform() {
  int x = 0;
  int path = 0;
  if (path == 0 && x == 0) {
    path = 1;
  } else if (path == 0) {
    x = 2;
  }

  if (path == 0) {
    x = 1;
  }

exit:
  if (path == 0 && path == 1) {
    x = -1;
  }
  return 0;
}
