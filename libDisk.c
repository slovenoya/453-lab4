#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include "libDisk.h"
#include "tinyFSErr.h"

int checkFd(int fd) {
  if (fd == -1) {
    fprintf(stderr, "Something wrong with openDisk() %s\n", strerror(errno));
    return DISK_OPEN_FAIL;
  }
  return fd;
}

/**
 * @brief open disk with given file name, the disk size equals nbytes. 
 * 
 * @param filename name for the file, if file doesn't exist then create one. 
 * @param nBytes how many bytes is the disk. 
 * @return int file descriptor of the disk. 
 */
int openDisk (char *filename, int nBytes) {
  int fd;
  if (nBytes < 0 || ((nBytes % BLOCKSIZE) != 0)) {
    fprintf(stderr, "Calling openDisk() with invalid block size: %d\n", nBytes);
    return DISK_INVALID_SIZE;
  } else if (nBytes == 0) {
    fd = open(filename, O_RDWR);
    return checkFd(fd);
  } else {
    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
    return checkFd(fd);
  }
}

/**
 * @brief return 0 on success, MISC_READ_FILE_FAIL on fail
 * 
 * @param disk fd for the disk
 * @param bNum number of blocks to be read
 * @param block pointer to buffer. 
 * @return int 
 */
int readBlock (int disk, int bNum, void *block) {
  int ret;
  ret = pread(disk, block, BLOCKSIZE, bNum * BLOCKSIZE);
  if (ret == -1) {
    fprintf(stderr, "Encoutering error in readBlock() %s\n", strerror(errno));
    return DISK_READ_FAIL;
  } else {
    return ret;
  }
}

/**
 * @brief write the buffer to the block. 
 * 
 * @param disk fd of the disk
 * @param bNum the block number to write on
 * @param block the pointer to what to write on disk. 
 * @return int 0 if success, MISC_WRITE_FILE_FAIL on fail
 */
int writeBlock (int disk, int bNum, void *block) {
  int ret;
  ret = pwrite(disk, block, BLOCKSIZE, BLOCKSIZE * bNum);
  if (ret == -1) {
    fprintf(stderr, "Encountering error in writeBlock() %s\n", strerror(errno));
    return DISK_WRITE_FAIL;
  } else {
    return ret;
  }
}

/**
 * @brief close the disk
 * 
 * @param disk fd number for the disk
 */
void closeDisk(int disk) {
  int ret;
  ret = close(disk);
  if (ret == -1) {
    fprintf(stderr, "Encountering error in closeBlock() %s\n", strerror(errno));
  } 
}
