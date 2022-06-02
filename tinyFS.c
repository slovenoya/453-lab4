#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <limits.h>
#include "tinyFS.h"
#include "libDisk.h"
#include "tinyFSErr.h"
#define CALLOC_ERR -1
#define OVERHEAD 2
int fd_FS = -1; // file descriptor for current mounted file system. 
Entry **entry_table;

void _init_disk(int nBytes) {
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

void _free_entry_table(Entry **entry_table) {
  int i;
  for (i = 0; i < TOTAL_ENTRY; i++) {
    if (entry_table[i] != NULL) {
      free(&entry_table[i]);
    }
  }
  free(entry_table);
}

int tfs_mkfs(char *filename, int nBytes) {
  int open_ret;
  int write_ret;
  int tot_blk = GET_BLOCK_LEN(nBytes);
  char super_block[BLOCKSIZE] = {0};

  //open disk for the file system. 
  open_ret = openDisk(filename, nBytes);
  if (open_ret < 0) {return open_ret;}
  fd_FS = open_ret;
  _init_disk(nBytes);

  //initialize super block
  super_block[SB_MAGIC_NUM_POS] = MAGIC_NUM;
  super_block[SB_BLOCK_LEN_POS] = tot_blk;
  super_block[SB_AVAILABLE_BLOCK_POS] = tot_blk- OVERHEAD;
  super_block[SB_AVAILABLE_ENTRY_POS] = TOTAL_ENTRY;
  super_block[SB_BIT_MAP_POS + SUPER_BLOCK_POS] = OCCUPIED;
  super_block[SB_BIT_MAP_POS + ROOT_INODE_POS] = OCCUPIED;

  //write the super block to disk
  write_ret = writeBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if (write_ret < 0) {return write_ret;}
  closeDisk(fd_FS);
  fd_FS = -1;
  return 0;
} 

int tfs_mount(char *filename) {
  int temp_fd;
  int read_ret;
  int blk_len;
  char super_block[BLOCKSIZE];

  //open disk
  if (fd_FS != -1) {return TFS_FS_ALREADY_MOUNTED;}
  temp_fd = openDisk(filename, 0);
  if (temp_fd < 0) {return temp_fd;}
  
  //check magic number
  read_ret = readBlock(temp_fd, SUPER_BLOCK_POS, super_block);
  if (read_ret < 0) {return read_ret;}
  if (super_block[SB_MAGIC_NUM_POS] != MAGIC_NUM) {return TFS_INVALID_FS;}
  blk_len = super_block[SB_BLOCK_LEN_POS];

  entry_table = call_calloc(sizeof(void*) * blk_len);
  fd_FS = temp_fd;
  return 0;
}

int tfs_unmount(void) {
  if (fd_FS == -1) {return TFS_NO_FS_TO_UNMOUNT;}
  _free_entry_table(entry_table);
  closeDisk(fd_FS);
  fd_FS = -1;
  return 0;
}

/**
 * @brief check if file with name has already been opened
 * 
 * @param name name for the file
 * @return int return 0 if no such file, negative if encounters error, return FD of the inode if it exists. 
 */
int _file_exist(char *name) {
  int i;
  int inode_blk;
  int read_ret;
  char root_node[BLOCKSIZE];
  char inode[BLOCKSIZE];
  char filename[MAX_NAME_LEN + 1];
  read_ret = readBlock(fd_FS, ROOT_INODE_POS, root_node);
  for (i = 0; i < TOTAL_ENTRY; i++) {
    memcpy(filename, &root_node[RI_FILE_NAME_POS(i)], MAX_NAME_LEN);
    if (strcmp(filename, name) == 0) {
      inode_blk = root_node[RI_INODE_BLOCK_POS(i)];
      read_ret = readBlock(fd_FS, inode_blk, inode);
      if (read_ret < 0) return read_ret;
      return inode[INODE_DATA_BLK_POS];
    }
  }
  return 0;
}

fileDescriptor tfs_open(char *name) {
  fileDescriptor FD;
  char root_node[BLOCKSIZE];
  char super_block[BLOCKSIZE];
  int read_ret;
  int available_entry;
  int available_block;

  //check file name length first
  if (strlen(name) > MAX_NAME_LEN) return TFS_EXCEED_NAME_LEN_MAX;

  //read root node
  read_ret = readBlock(fd_FS, ROOT_INODE_POS, root_node);
  if (read_ret < 0) return read_ret;

  //check if the file has already been opened
  FD = _file_exist(name);
  if (FD > 0) return FD;

  //file hasn't been opened yet
  read_ret = readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if (read_ret < 0) return read_ret;
  //check if there is enough space. 
  available_block = super_block[SB_AVAILABLE_BLOCK_POS];
  available_entry = super_block[SB_AVAILABLE_ENTRY_POS];
  if (available_entry == 0) return TFS_NO_AVAILABLE_ENTRY;
  if (available_block < 2) return TFS_NO_AVAILABLE_BLOCK;
  //
  return FD;

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
