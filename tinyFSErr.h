#ifndef TINYFSERR_H
#define TINYFSERR_H

#define DISK_OPEN_FAIL -1
#define DISK_INVALID_SIZE -2
#define DISK_READ_FAIL -3
#define DISK_WRITE_FAIL -4
#define TFS_FS_ALREADY_MOUNTED -5
#define TFS_INVALID_FS -6
#define TFS_NO_FS_TO_UNMOUNT -7
#define TFS_NO_AVAILABLE_ENTRY -8
#define TFS_NO_AVAILABLE_BLOCK -9
#define TFS_EXCEED_NAME_LEN_MAX -10
#define TFS_CLOSE_FILE_NOT_OPENED -11
#define TFS_INVALID_FD -12

void PRINT_ERR(int errno);

#endif
