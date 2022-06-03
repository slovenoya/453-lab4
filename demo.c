#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tinyFS.h"
#include "libDisk.h"
#include "tinyFSErr.h"
int main() {
  char read;
  printf("create a file system with default disk name and default size, no error message\n");
  PRINT_ERR(tfs_mkfs(DEFUALT_DISK_NAME, DEFAULT_DISK_SIZE));

  printf("\nmount file system with invalid file, error message\n");
  PRINT_ERR(tfs_mount("README"));

  printf("\nmount the file system with default name, no error message\n");
  PRINT_ERR(tfs_mount(DEFUALT_DISK_NAME));
  tfs_display_framents();

  printf("\nopen file on the disk, no error\n");
  int fd = tfs_open("file1.txt");
  PRINT_ERR(fd);
  tfs_stat(fd);
  sleep(1);
  tfs_display_framents();

  printf("\nopen another file on the disk, no error\n");
  int fd1 = tfs_open("file2.txt");
  PRINT_ERR(fd1);
  tfs_stat(fd1);
  sleep(1);
  tfs_display_framents();

  printf("\nopen another file on the disk, no error\n");
  int fd2 = tfs_open("file3.txt");
  PRINT_ERR(fd2);
  tfs_stat(fd2);
  sleep(1);
  tfs_display_framents();

  printf("\nopen another file on the disk, no error\n");
  int fd3 = tfs_open("file4.txt");
  PRINT_ERR(fd3);
  tfs_stat(fd3);
  tfs_display_framents();

  printf("\nnote their create time and access time are all changed\n");

  printf("\nprint all directories: \n");
  tfs_readdir();
  printf("\nrename file1 to file1(1)\n");
  tfs_rename(fd, "file1(1)");

  printf("print directories\n");
  tfs_readdir();

  printf("\nwrite %s to file1, no errors\n", "abcd");
  PRINT_ERR(tfs_write(fd, "abcd", 4));

  printf("\nread from disk, no errors\n");
  PRINT_ERR(tfs_readByte(fd, &read));
  printf("print the first byte we read: ");
  printf("%c\n",read);

  printf("\nread another byte, no errors\n");
  PRINT_ERR(tfs_readByte(fd, &read));
  printf("print the second byte we read: ");
  printf("%c\n",read);

  printf("\nseek position 0, no errors\n");
  PRINT_ERR(tfs_seek(fd, 0));
  printf("read a byte from position 0\n");
  PRINT_ERR(tfs_readByte(fd, &read));
  printf("print the byte we read: ");
  printf("%c\n",read);

  printf("\nread another byte \n");
  PRINT_ERR(tfs_readByte(fd, &read));
  printf("print the byte: ");
  printf("%c\n",read);

  printf("\ndelete file2\n");
  PRINT_ERR(tfs_delete(fd1));
  tfs_readdir();
  tfs_display_framents();

  printf("\ndelete file3\n");
  PRINT_ERR(tfs_delete(fd2));
  tfs_readdir();
  tfs_display_framents();

  printf("\ndelete file4\n");
  PRINT_ERR(tfs_delete(fd3));
  tfs_readdir();
  tfs_display_framents();

  printf("\ndelete file4 again, error occurs: \n");
  PRINT_ERR(tfs_delete(fd3));

  printf("\ntry to mount current file system again, error occurs: \n");
  PRINT_ERR(tfs_mount(DEFUALT_DISK_NAME));

  printf("\nunmount current file system\n");
  PRINT_ERR(tfs_unmount());

  printf("\nunmount again, error occurs:\n");
  PRINT_ERR(tfs_unmount());

  printf("\nmount the file system back\n");
  PRINT_ERR(tfs_mount(DEFUALT_DISK_NAME));

  printf("\nprint directories in the disk\n");
  tfs_readdir();

  printf("\nopen existed file on disk\n");
  fd = tfs_open("file1(1)");
  PRINT_ERR(fd);

  printf("\nread byte from the file");
  PRINT_ERR(tfs_readByte(fd, &read));
  printf("\nprint the byte: ");
  printf("%c\n", read);

  printf("\nread another byte from the file");
  PRINT_ERR(tfs_readByte(fd, &read));
  printf("\nprint the byte: ");
  printf("%c\n", read);

  printf("\nread another byte from the file");
  PRINT_ERR(tfs_readByte(fd, &read));
  printf("\nprint the byte: ");
  printf("%c\n", read);

  printf("\nread another byte from the file");
  PRINT_ERR(tfs_readByte(fd, &read));
  printf("\nprint the byte: ");
  printf("%c\n", read);

  printf("\n open file names 1.txt 2.txt 3.txt 4.txt");
  tfs_open("1.txt");
  int fd_2 = tfs_open("2.txt");
  tfs_open("3.txt");
  tfs_open("4.txt");
  tfs_readdir();
  tfs_display_framents();

  printf("\ndelete 2.txt");
  tfs_delete(fd_2);
  tfs_display_framents();

  printf("\ndefragmentation\n");
  tfs_defragmentation();
  tfs_display_framents();

  printf("\nunmount the file system");
  PRINT_ERR(tfs_unmount());
  return 0;
}
