#include <stdio.h>
#include <stdlib.h>

#define EXPENSIVE __attribute__((annotate("expensive")))
#define TAS_BATCH __attribute__((annotate("tas_batch")))

void annotate_ptr() TAS_BATCH {
  for (int i = 0; i < 10; ++i) {
    int EXPENSIVE * a;
    a = malloc(sizeof(int));
    printf("c = %d\n", *a);
    free(a);
  }
}

int main() {
  annotate_ptr();
  return 0;
};
