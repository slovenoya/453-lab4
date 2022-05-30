#ifndef LIBDISK_H
#define LIBDISK_H
/* This function opens a regular UNIX file 
and designates the first nBytes of it as space 
for the emulated disk. 
nBytes should be a number that is evenly divisible 
by the block size. 
If nBytes > 0 and there is already a file by 
the given filename, that disk is resized to nBytes, 
and that file’s contents may be overwritten. 
If nBytes is 0, an existing disk is opened, 
and should not be overwritten. 
There is no requirement to maintain integrity 
of any content beyond nBytes. Errors must be 
returned for any other failures, as defined by 
your own error code system.  */
int openDisk (char *filename, int nBytes);
int readBlock (int disk, int bNum, void *block);
int writeBlock (int disk, int bNum, void *block);
void closeDisk(int disk);
#define BLOCKSIZE 256
#define INVALID_BLOCK_SIZE -1
#define MISC_OPEN_FILE_FAIL -2
#define MISC_READ_FILE_FAIL -3
#define MISC_WRITE_FILE_FAIL -4
#define MISC_CLOSE_FILE_FAIL -5
#endif