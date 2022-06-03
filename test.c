#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyFS.h"
#include "libDisk.h"
#include "tinyFSErr.h"
int main() {
  int mkfile = tfs_mkfs(DEFUALT_DISK_NAME, DEFAULT_DISK_SIZE);
  int mount = tfs_mount(DEFUALT_DISK_NAME);
  int unmount = tfs_unmount();
  mount = tfs_mount(DEFUALT_DISK_NAME);
  // unmount = tfs_unmount();
  // printf("mkf: %d, mount: %d, unmount: %d\n", mkfile, mount, unmount);
  // unmount = tfs_unmount();
  // printf("mkf: %d, mount: %d, unmount: %d\n", mkfile, mount, unmount);
  // mount = tfs_mount(DEFUALT_DISK_NAME);
  int fd = tfs_open("abcd");
  tfs_delete(fd);
  // int fd = tfs_open("abcd");
  // PRINT_ERR(fd);
  // fd = tfs_open("abcd");
  // PRINT_ERR(fd);
  // fd = tfs_open("abcde");
  // PRINT_ERR(fd);
  // fd = tfs_close(0);
  // PRINT_ERR(fd);
  // fd = tfs_open("abcd");
  // PRINT_ERR(fd);
  // fd = tfs_close(0);
  // PRINT_ERR(fd);
  // fd = tfs_close(1);
  // PRINT_ERR(fd);
  // fd = tfs_open("abcd");
  // PRINT_ERR(fd);
  // fd = tfs_close(1);
  // PRINT_ERR(fd);

  // fd = tfs_open("abcd");
  // printf("fd: %d\n", fd);
  // fd = tfs_open("abcde");
  // printf("fd: %d\n", fd);

  tfs_unmount();
  return 0;
}
