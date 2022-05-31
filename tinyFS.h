#ifndef TINYFS_H
#define TINYFS_H

#define BLOCKSIZE 256
#define DEFAULT_DISK_SIZE 10240
#define DEFUALT_DISK_NAME "tinyFSDisk"
#define MAGIC_NUM 0x5a
typedef int fileDescriptor;

typedef struct SuperBlock {
  void *inode;
  void *free_blocks;
  unsigned int magic_num;
  unsigned int size;
} SuperBlock;

typedef struct INode {
  unsigned int file_size;
  unsigned int offset;
} INode;

int tfs_mkfs(char *filename, int nBytes);
int tfs_mount(char *filename);
int tfs_unmount(void);
fileDescriptor tfs_open(char *name);
int tfs_close(fileDescriptor FD);
int tfs_write(fileDescriptor FD, char *buffer, int size);
int tfs_delete(fileDescriptor FD);
int tfs_readByte(fileDescriptor FD, char *buffer);
int tfs_seek(fileDescriptor FD, int offset);

#endif
