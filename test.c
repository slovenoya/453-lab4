#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyFS.h"
#include "libDisk.h"
int main() {
  int mkfile = tfs_mkfs(DEFUALT_DISK_NAME, DEFAULT_DISK_SIZE);
  int mount = tfs_mount(DEFUALT_DISK_NAME);
  // int unmount = tfs_unmount();
  // printf("mkf: %d, mount: %d, unmount: %d\n", mkfile, mount, unmount);
  // mount = tfs_mount(DEFUALT_DISK_NAME);
  // unmount = tfs_unmount();
  // printf("mkf: %d, mount: %d, unmount: %d\n", mkfile, mount, unmount);
  // unmount = tfs_unmount();
  // printf("mkf: %d, mount: %d, unmount: %d\n", mkfile, mount, unmount);
  // mount = tfs_mount(DEFUALT_DISK_NAME);

  int fd = tfs_open("abcd");
  printf("fd: %d\n", fd);
  fd = tfs_open("abcd");
  printf("fd: %d\n", fd);
  // char *a = calloc(1, 20);
  // int c;
  // a[0] = 1;
  // a[1] = 1;
  // a[2] = 1;
  // a[3] = 1;
  // a[5] = 1;
  // memcpy(&c, a, sizeof(int));
  // printf("%x\n", c); 
  // memcpy(&c, a+1, sizeof(int));
  // printf("%x\n", c); 
  // free(a);
  return 0;
}
