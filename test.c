#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tinyFS.h"
#include "libDisk.h"
int main() {
  tfs_mkfs(DEFUALT_DISK_NAME, DEFAULT_DISK_SIZE);
  return 0;
}
