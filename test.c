#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyFS.h"
#include "libDisk.h"
int main() {
  int disk = tfs_mkfs(DEFUALT_DISK_NAME, DEFAULT_DISK_SIZE);
  void *read = malloc(256);
  readBlock(disk, 0, read);
  SuperBlock super_block;
  memcpy(&super_block, read, sizeof(SuperBlock));
  printf("super block: %d\n", super_block.magic_num);
  return 0;
}
