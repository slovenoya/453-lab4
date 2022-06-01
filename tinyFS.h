#include <time.h>
#ifndef TINYFS_H
#define TINYFS_H
#define BLOCKSIZE 256
#define DEFAULT_DISK_SIZE 10240
#define DEFUALT_DISK_NAME "tinyFSDisk"
#define MAGIC_NUM 0x5a
#define SUPER_BLOCK_POS 0
typedef int fileDescriptor;

#define GET_BLOCK_LEN(TOTAL_SIZE) (TOTAL_SIZE / BLOCKSIZE)
#define SB_MAGIC_NUM_POS 0

#define ROOT_INODE_POS 1
#define RI_ENTRY(ENTRY_INDEX) (ENTRY_INDEX)


//helper functions
/**
 * @brief returns how many block is needed for given size. 
 * 
 * @param size size of the file. 
 * @return int number of blocks needed
 */
int _size_to_block(int size);

//functions related with super block
/**
 * @brief initialize the disk(file descriptor), set the nBytes to be 0x0
 * 
 */
void _init_disk(int disk, int nBytes);
int _get_magic_number(void);
int _get_root_inode_block_num(void);
void* _get_bit_map(void);
void _set_block_free(int block);
void _set_block_occupied(int block);
int _get_block_status(int block);
int _get_free_block(void);
int _get_FD(void);

//functions related with inode
void _init_inode(void);
void _set_ownershipt(int inode, int ownership);
int _get_ownership(int inode);
void _set_create_time(int inode, time_t time);
char* _get_create_time(int inode);

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

#endif
