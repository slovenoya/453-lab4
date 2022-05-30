#include <stdio.h>
#include <stdlib.h>
#include "tinyFS.h"
#include "libDisk.h"
int tfs_mkfs(char *filename, int nBytes) {
  SuperBlock super_block;
  int disk = openDisk(filename, nBytes);
  super_block.magic_num = MAGIC_NUM;

}
