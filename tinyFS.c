#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
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
  super_block[SB_TOTAL_SIZE_POS] = block_len;
  memcpy((super_block + SB_BIT_MAP_POS), block_status, sizeof(int) * block_len);
  writeBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  free(super_block);

  //close the disk after initializing and set fd for current file system to -1
  closeDisk(fd_FS);
  fd_FS = -1;
  return 0;
} 

int _get_total_size(char *super_block) {
  int total_size;
  memcpy(&total_size, (super_block + SB_TOTAL_SIZE_POS), sizeof(int));
  return total_size;
}

char *_get_bit_map(int block_len) {
  char *super_block = call_calloc(BLOCKSIZE);
  char *bit_map = call_calloc(block_len * sizeof(int));
  readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  memcpy(bit_map, &(super_block[SB_BIT_MAP_POS]), block_len * sizeof(char));
  free(super_block);
  return bit_map;
}

void _set_block_free(int block) {
  void *super_block = call_calloc(BLOCKSIZE);
  free(super_block);
}

void _set_block_occupied(int block) {
  void *super_block = call_calloc(BLOCKSIZE);
  free(super_block);
}

int tfs_mount(char *filename) {
  char* super_block;
  int magic_num;
  int total_size;

  if (fd_FS != -1) {return MOUNTING_MORE_THAN_ONE_FS;}
  fd_FS = openDisk(filename, BLOCKSIZE);
  if (fd_FS < 0) return fd_FS;

  super_block = call_calloc(BLOCKSIZE);
  readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  magic_num = super_block[SB_MAGIC_NUM_POS];
  if (magic_num != MAGIC_NUM) return INVALID_FS_MOUNTING_FILE;
  total_size = super_block[SB_TOTAL_SIZE_POS];
  closeDisk(fd_FS);
  free(super_block);

  fd_FS = openDisk(filename, total_size);
  if (fd_FS < 0) return fd_FS;
  return 0;
}

int tfs_unmount(void) {
  closeDisk(fd_FS);
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
