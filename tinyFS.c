#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tinyFS.h"
#include "libDisk.h"
#include "tinyFSErr.h"
#define CALLOC_ERR -1

int fd_FS = -1; // file descriptor for current mounted file system. 

int _size_to_block(int size) {return (size / BLOCKSIZE + 1);}

void _init_disk(int disk, int nBytes) {
  int i;
  char zero = 0x0;
  for (i = 0; i < nBytes; i++)
    pwrite(fd_FS, &zero, 1, i);
}

void* call_calloc(int size) {
  void* ret;
  ret = calloc(1, size);
  if (ret == NULL) {
    fprintf(stderr, "something goes wrong on calloc\n");
    exit(CALLOC_ERR);
  } else {
    return ret;
  }
}

int tfs_mkfs(char *filename, int nBytes) {
  int block_len;
  int fd_FS = openDisk(filename, nBytes);
  if (fd_FS < 0) return fd_FS;
  _init_disk(fd_FS, nBytes);
  block_len = GET_BLOCK_LEN(nBytes);

  //initialize super block and write it on disk
  char *super_block = call_calloc(BLOCKSIZE);
  int block_status[block_len];
  super_block[SB_MAGIC_NUM_POS] = MAGIC_NUM;
  memcpy(super_block + sizeof(MAGIC_NUM), block_status, sizeof(int) * block_len);
  writeBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  free(super_block);

  //initialize root node and write it on disk
  char *root_node = call_calloc(BLOCKSIZE);
  writeBlock(fd_FS, ROOT_INODE_POS, root_node);
  free(root_node);

  //close the disk after initializing and set fd for current file system to -1
  closeDisk(fd_FS);
  return fd_FS;
} 

int tfs_mount(char *filename) {

  return 0;
}

int tfs_unmount(void) {
  return 0;
}

fileDescriptor tfs_open(char *name) {
  return 0;
}

int tfs_close(fileDescriptor FD) {
  return 0;
}

int tfs_write(fileDescriptor FD, char *buffer, int size) {
  return 0;
}

int tfs_delete(fileDescriptor FD) {
  return 0;
}

int tfs_readByte(fileDescriptor FD, char *buffer) {
  return 0;
}

int tfs_seek(fileDescriptor FD, int offset) {
  return 0;
}
