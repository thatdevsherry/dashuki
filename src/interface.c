#include <asm/termbits.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int open_kkl(const char *dev) {
  int fd = open(dev, O_RDWR | O_NOCTTY);
  if (fd < 0)
    return -1;

  struct termios2 tty;
  ioctl(fd, TCGETS2, &tty);

  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;

  tty.c_cflag |= (CLOCAL | CREAD);

  tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG);
  tty.c_iflag &= ~(IXON | IXOFF | IXANY | IGNBRK | BRKINT | PARMRK | ISTRIP |
                   INLCR | IGNCR | ICRNL);
  tty.c_oflag &= ~(OPOST | ONLCR);

  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 10;

  tty.c_cflag &= ~CBAUD;
  tty.c_cflag |= BOTHER;
  tty.c_ispeed = 7812;
  tty.c_ospeed = 7812;

  int ret = ioctl(fd, TCSETS2, &tty);

  if (ret != 0) {
    printf("Error %i from ioctl: %s\n", ret, strerror(ret));
  }

  return fd;
}

int interface_send(int fd, uint8_t *send, uint8_t len) {
  return write(fd, send, len);
}

uint8_t interface_recv(int fd, uint8_t *buf, uint8_t len) {
  int n = read(fd, buf, len);
  return n;
}
