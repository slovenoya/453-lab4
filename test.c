#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyFS.h"
#include "libDisk.h"
int main() {
  int disk = tfs_mkfs(DEFUALT_DISK_NAME, DEFAULT_DISK_SIZE);
  void *read = malloc(256);
  printf("%d\n", disk);
  readBlock(disk, 0, read);
  return 0;
}
