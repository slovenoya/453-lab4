#include <stdio.h>
#include <string.h>
#include "libDisk.h"
int main() {
  //----------libDisk----------//
  //wrong block size tests
  printf("first, open disk with wrong block size: \n");
  openDisk("file.txt", -1);
  openDisk("file.txt", 21);
  //correct block size tests
  int dummyDisk = 33;
  int disk = openDisk("file.txt", 256);
  printf("disk number: %d \n", disk);
  
  char str[256];
  readBlock(dummyDisk, 0, str);
  
  int read = readBlock(disk, 0, str);
  printf("read return: %d, read bytes: %s\n", read, str);
  strncpy(str, "hello, world", 256);
  writeBlock(dummyDisk, 0, str);
  
  int write = writeBlock(disk, 0, str);
  printf("write return: %d, write bytes: %s\n", write, str);
  
  read = readBlock(disk, 0, str);
  printf("read return: %d, read bytes: %s\n", read, str);
  
  //display wrong closure. 
  closeDisk(dummyDisk);
  closeDisk(disk);
  //----------TinyFS----------//
  return 0;
}
