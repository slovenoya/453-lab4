#include <stdio.h>
#include "tinyFSErr.h"

void PRINT_ERR(int ERRNO)
{

if (ERRNO == DISK_OPEN_FAIL) {
  fprintf(stderr, "disk open failed\n");
} else if (ERRNO == DISK_INVALID_SIZE) {
  fprintf(stderr, "disk open with invalid size\n");
} else if (ERRNO == DISK_READ_FAIL) {
  fprintf(stderr, "disk read failed\n");
} else if (ERRNO == DISK_WRITE_FAIL) {
  fprintf(stderr, "disk write failed\n");
} else if (ERRNO == TFS_FS_ALREADY_MOUNTED) {
  fprintf(stderr, "already mounted a file system\n");
} else if (ERRNO == TFS_INVALID_FS) {
  fprintf(stderr, "file given is not a valid file system\n");
} else if (ERRNO == TFS_NO_FS_TO_UNMOUNT) {
  fprintf(stderr, "no mounted file system to be unmount\n");
} else if (ERRNO == TFS_NO_AVAILABLE_ENTRY) {
  fprintf(stderr, "no available entry to use\n");
} else if (ERRNO == TFS_NO_AVAILABLE_BLOCK) {
  fprintf(stderr, "no available space to use\n");
} else if (ERRNO == TFS_EXCEED_NAME_LEN_MAX) {
  fprintf(stderr, "file name too long\n");
} else if (ERRNO == TFS_CLOSE_FILE_NOT_OPENED) {
  fprintf(stderr, "try to close a file that wasn't opened\n");
} else if (ERRNO == TFS_INVALID_FD) {
  fprintf(stderr, "invalid file descriptor\n");
} 
} 
