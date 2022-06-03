#include <time.h>
#ifndef TINYFS_H
#define TINYFS_H

#define BLOCKSIZE 256
#define DEFAULT_DISK_SIZE 10240
#define DEFUALT_DISK_NAME "tinyFSDisk"
#define MAGIC_NUM 0x5a
#define GET_BLOCK_LEN(TOTAL_SIZE) (TOTAL_SIZE / BLOCKSIZE)

#define SUPER_BLOCK_POS 0
#define SB_MAGIC_NUM_POS 0
#define SB_BLOCK_LEN_POS 1
#define SB_AVAILABLE_BLOCK_POS 2
#define SB_AVAILABLE_ENTRY_POS 3
#define SB_BIT_MAP_POS 4
#define OCCUPIED 1
#define FREE 0

#define ROOT_INODE_POS 1
#define MAX_NAME_LEN 15
#define TOTAL_ENTRY 16
#define RI_ENTRY_SIZE 16
#define RI_INODE_BLK_SIZE 1
#define RI_INODE_BLOCK_POS(ENTRY_INDEX) (ENTRY_INDEX * RI_ENTRY_SIZE)
#define RI_FILE_NAME_POS(ENTRY_INDEX) (ENTRY_INDEX * RI_ENTRY_SIZE + RI_INODE_BLK_SIZE)

#define INODE_DATA_BLK_POS 0
#define INODE_CREAT_TIME_POS 4
#define INODE_ACCESS_TIME_POS 20
#define INODE_POINTER_POS 30

#define DATA_NEXT_BLK_POS 0
#define DATA_START_POS 1
#define DATA_SIZE 255

typedef int fileDescriptor;

typedef struct Entry {
  int inode_blk_pos;
  fileDescriptor FD;
} Entry;

void _write_time(char *inode, int position, int inode_pos);

int _find_FD_by_inode_pos(int inode_pos);
/**
 * @brief makes an empty TinyFS file system of size nBytes on the file named filename. 
 * File will be foramtted as moutable, all data initalized to 0x0, magic numbers setted. 
 * 
 * @param filename filename for the TinyFS to be stored. 
 * @param nBytes size of the file system, must be a positive number divisible by 256. 
 * @return int return 0 on success, 
 * returns error code when size is valid or other error occurs while initializing file system. 
 */
int tfs_mkfs(char *filename, int nBytes);

/**
 * @brief mount a file system located in the file with filename. 
 * 
 * @param filename filename for the file of the TinyFS file system. 
 * @return int returns 0 on success, returns error code if there is no valid TinyFS file system in the file. 
 */
int tfs_mount(char *filename);

/**
 * @brief unmounts the current file system. 
 * 
 * @return int return 0 on success, returns error code if anything goes wrong when unmounting the current FS. 
 */
int tfs_unmount(void);

/**
 * @brief open a file for reading and writing on current mounted file system. 
 * create a dynamic resource table entry for the files. 
 * create a file in the file system if no file with name exists. 
 * 
 * @param name name for the file. 
 * @return fileDescriptor returns a positive number for file descriptor, returns error code when there is not enough space. 
 */
fileDescriptor tfs_open(char *name);

/**
 * @brief close the file and removes dynamic resource table entry. 
 * 
 * @param FD file descriptor. 
 * @return int return 0 on success, returns error code when FD is invalid. 
 */
int tfs_close(fileDescriptor FD);

/**
 * @brief write buffer to the disk with size of 'size'. File pointer setted to be 0 when done. 
 * 
 * @param FD file pointer. 
 * @param buffer content to write on disk. 
 * @param size size of the file. 
 * @return int returns 0 on sucess, returns error code when size is invalid or FD is invalid. 
 */
int tfs_write(fileDescriptor FD, char *buffer, int size);

/**
 * @brief delete a file and marks its blocks as free on disk
 * 
 * @param FD file descriptor given by tinyFS
 * @return int returns 0 on success, returns error code when fd is not valid. 
 */
int tfs_delete(fileDescriptor FD);

/**
 * @brief read one byte from the file and copied it to buffer, then increment its inode's offset on success. 
 * 
 * @param FD file descriptor given by tinyFS
 * @param buffer pointer to copy the data from tinyFS
 * @return int returns 0 on success, error code when pointer is at the end of file or FD invalid. 
 */
int tfs_readByte(fileDescriptor FD, char *buffer);

/**
 * @brief set the inode's offset
 * 
 * @param FD file descriptor given by tinyFS. 
 * @param offset offset of file pointer
 * @return int returns 0 on success, error code when offset out of bounds or FD is invalid. 
 */
int tfs_seek(fileDescriptor FD, int offset);

void tfs_display_framents();

void tfs_defragmentation();

/**
 * @brief prints the create time, last accessed time. 
 * 
 * @param FD file descriptor
 */
void tfs_stat(fileDescriptor FD);

/**
 * @brief rename the file referenced by FD to new_name
 * 
 * @param FD file descriptor
 * @param new_name new name for the file
 */
int tfs_rename(fileDescriptor FD, char *new_name);

/**
 * @brief print all files in root directory, 
 * display inode and filename of those files. 
 * 
 */
void tfs_readdir(void);

/**
 * @brief prints the time stamps for the file
 * 
 * @param FD 
 */
void tfs_stat(fileDescriptor FD);



#endif
