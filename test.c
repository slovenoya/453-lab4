#include "tinyFS.h"
int main() {
  tfs_mkfs("file.txt", DEFAULT_DISK_SIZE);
  return 0;
}
