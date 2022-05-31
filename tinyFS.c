#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyFS.h"
#include "libDisk.h"
int tfs_mkfs(char *filename, int nBytes) {
  int disk = openDisk(filename, nBytes);
  SuperBlock super_block;
  super_block.magic_num = MAGIC_NUM;
  super_block.size = nBytes;
  writeBlock(disk, 0, &super_block);
  return disk;
} 
