#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

int open_kkl(const char *dev);
int interface_send(int fd, uint8_t *send, uint8_t len);
uint8_t interface_recv(int fd, uint8_t *buf, uint8_t len);

#endif
