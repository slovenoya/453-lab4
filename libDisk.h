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
#endif
