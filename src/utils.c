#include <stdint.h>
#include <stdio.h>

void pretty_print_msg(uint8_t *msg, uint8_t len, const char *type) {
  printf("%s: [ ", type);
  for (int i = 0; i < len; i++) {
    printf("%02x ", msg[i]);
  }
  printf("]\n");
}
