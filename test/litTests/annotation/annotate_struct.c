// RUN: clang -S -emit-llvm %s -o %t.ll

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXPENSIVE __attribute__((annotate("expensive")))
#define TAS_BATCH __attribute__((annotate("tas_batch")))

struct packet {
  int id;
  char * buffer EXPENSIVE;
} ;

char * packet_content = "Empty packet";

void annotate_struct() TAS_BATCH {
  for (int i = 0; i < 10; ++i) {
    struct packet a ;
    a.id = 1;
    a.buffer = malloc(sizeof(char) * 20);
    strcpy(a.buffer, packet_content);
    
    printf("packet_content = %s\n", a.buffer);
    free(a.buffer);
  }
}

int main() {
  annotate_struct();
  return 0;
}
