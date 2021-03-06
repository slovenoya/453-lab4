#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <assert.h>
#include <limits.h>
#include "tinyFS.h"
#include "libDisk.h"
#include "tinyFSErr.h"
#define CALLOC_ERR -1
#define OVERHEAD 2
int fd_FS = -1; // file descriptor for current mounted file system. 
Entry **opened_file;

void _init_disk(int nBytes) {
  int i;
  char zero = 0x0;
  for (i = 0; i < nBytes; i++)
    pwrite(fd_FS, &zero, 1, i);
}

void* call_calloc(int size) {
  void* ret;
  ret = calloc(1, size);
  if (ret == NULL) {
    fprintf(stderr, "something goes wrong on calloc\n");
    exit(CALLOC_ERR);
  } else {
    return ret;
  }
}

/**
 * @brief smallest available fd
 * 
 * @return int -1 if no fd available, 0 ... TOTAL_ENTRY on success
 */
int _get_FD() {
  int i;
  for (i = 0; i < TOTAL_ENTRY; i++) {
    if (opened_file[i] == NULL) {
      return i;
    }
  }
  return -1;
}

void _free_entries(Entry **opened_file) {
  int i;
  for (i = 0; i < TOTAL_ENTRY; i++) {
    if (opened_file[i] != NULL) {
      free(opened_file[i]);
    }
  }
  free(opened_file);
}

int tfs_mkfs(char *filename, int nBytes) {
  int open_ret;
  int write_ret;
  int tot_blk = GET_BLOCK_LEN(nBytes);
  char super_block[BLOCKSIZE] = {0};

  //open disk for the file system. 
  open_ret = openDisk(filename, nBytes);
  if (open_ret < 0) {return open_ret;}
  fd_FS = open_ret;
  _init_disk(nBytes);

  //initialize super block
  super_block[SB_MAGIC_NUM_POS] = MAGIC_NUM;
  super_block[SB_BLOCK_LEN_POS] = tot_blk;
  super_block[SB_AVAILABLE_BLOCK_POS] = tot_blk- OVERHEAD;
  super_block[SB_AVAILABLE_ENTRY_POS] = TOTAL_ENTRY;
  super_block[SB_BIT_MAP_POS + SUPER_BLOCK_POS] = OCCUPIED;
  super_block[SB_BIT_MAP_POS + ROOT_INODE_POS] = OCCUPIED;

  //write the super block to disk
  write_ret = writeBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if (write_ret < 0) {return write_ret;}
  closeDisk(fd_FS);
  fd_FS = -1;
  return 0;
} 

int tfs_mount(char *filename) {
  int temp_fd;
  int read_ret;
  int blk_len;
  char super_block[BLOCKSIZE];

  //open disk
  if (fd_FS != -1) {return TFS_FS_ALREADY_MOUNTED;}
  temp_fd = openDisk(filename, 0);
  if (temp_fd < 0) {return temp_fd;}
  
  //check magic number
  read_ret = readBlock(temp_fd, SUPER_BLOCK_POS, super_block);
  if (read_ret < 0) {return read_ret;}
  if (super_block[SB_MAGIC_NUM_POS] != MAGIC_NUM) {return TFS_INVALID_FS;}
  blk_len = super_block[SB_BLOCK_LEN_POS];

  opened_file = call_calloc(sizeof(void*) * blk_len);
  fd_FS = temp_fd;
  return 0;
}

int tfs_unmount(void) {
  if (fd_FS == -1) {return TFS_NO_FS_TO_UNMOUNT;}
  _free_entries(opened_file);
  closeDisk(fd_FS);
  fd_FS = -1;
  return 0;
}

/**
 * @brief check if file with name has already been opened
 * 
 * @param name name for the file
 * @return int FD when file exist and opened, -1 when exist but not opened, -2 if not exist
 */
int _file_exist(char *root_node, char *name) {
  int i;
  int inode_blk;
  int FD;
  char filename[MAX_NAME_LEN + 1] = {0};
  for (i = 0; i < TOTAL_ENTRY; i++) {
    memcpy(filename, &root_node[RI_FILE_NAME_POS(i)], MAX_NAME_LEN);
    if (strcmp(filename, name) == 0) {
      inode_blk = root_node[RI_INODE_BLOCK_POS(i)];
      FD = _find_FD_by_inode_pos(inode_blk);
      //exist on disk but not opened
      if (FD == -1) {
        return -1;
      }
      return FD;
    }
  }
  //do not exist on disk
  return -2;
}

int _find_inode_pos_by_name(char* root_inode, char *name) {
  int i;
  char filename[MAX_NAME_LEN + 1] = {0};
  for (i = 0; i < TOTAL_ENTRY; i++) {
    memcpy(filename, &root_inode[RI_FILE_NAME_POS(i)], MAX_NAME_LEN);
    if (strcmp(filename, name) == 0) {
      return root_inode[RI_INODE_BLOCK_POS(i)];
    }
  }
  return -1;
}

/**
 * @brief find the FD on inode_pos
 * 
 * @param inode_pos the position of inode
 * @return int return -1 if not exist in opened file, return FD if exists. 
 */
int _find_FD_by_inode_pos(int inode_pos) {
  int i;
  Entry *entry;
  for (i = 0; i < TOTAL_ENTRY; i++) {
    entry = opened_file[i];
    if (entry == NULL) return -1;
    if (entry -> inode_blk_pos == inode_pos)
      return entry -> FD;
  }
  return -1;
}

/**
 * @brief find a free block by super block's bitmap, then mark the free block occupied
 * 
 * @param super_block super block
 * @return int index of the free block on success, -1 on fail. 
 */
int _find_free_block(char *super_block) {
  int i;
  int blk_len;
  blk_len = super_block[SB_BLOCK_LEN_POS];
  for (i = 0; i < blk_len; i++) {
    if (super_block[SB_BIT_MAP_POS + i] == FREE) {
      super_block[SB_BIT_MAP_POS + i] = OCCUPIED;
      return i;
    }
  }
  return -1;
}

fileDescriptor tfs_open(char *name) {
  fileDescriptor FD;
  Entry *entry;
  char root_node[BLOCKSIZE];  //root node for entries
  char super_block[BLOCKSIZE];//super block
  char inode[BLOCKSIZE] = {0};
  int disk_ret;
  int available_entry;//number of free entries
  int available_block;//number of free blocks
  int data_blk_pos; //index for data block
  int inode_blk_pos;//index for inode block

  //check file name length first
  if (strlen(name) > MAX_NAME_LEN) return TFS_EXCEED_NAME_LEN_MAX;

  //read root node
  disk_ret = readBlock(fd_FS, ROOT_INODE_POS, root_node);
  if (disk_ret < 0) return disk_ret;
  //read super block
  disk_ret = readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if (disk_ret < 0) return disk_ret;

  //record available spaces for block and entry
  available_block = super_block[SB_AVAILABLE_BLOCK_POS];
  available_entry = super_block[SB_AVAILABLE_ENTRY_POS];

  //check if the file has already been created
  FD = _file_exist(root_node, name);
  if (FD >= 0) { //file exist in disk and opened. 
    return FD;
  } else if (FD == -1){ //file exist in disk but not opened. 
    entry = call_calloc(sizeof(void*));
    FD = _get_FD();
    entry -> FD = FD;
    entry -> inode_blk_pos = _find_inode_pos_by_name(root_node, name);
    super_block[SB_AVAILABLE_ENTRY_POS] = super_block[SB_AVAILABLE_ENTRY_POS] - 1;
    opened_file[FD] = entry;
    tfs_seek(FD, 0);
    return FD;
  }

  //file hasn't been created yet
  if (available_entry == 0) return TFS_NO_AVAILABLE_ENTRY;
  if (available_block < 2) return TFS_NO_AVAILABLE_BLOCK;
  //proceed with enough space. 
  entry = call_calloc(sizeof(void*));
  inode_blk_pos = _find_free_block(super_block);
  data_blk_pos = _find_free_block(super_block);
  entry -> inode_blk_pos = inode_blk_pos;
  super_block[SB_AVAILABLE_BLOCK_POS] = super_block[SB_AVAILABLE_BLOCK_POS] - 1;
  super_block[SB_AVAILABLE_ENTRY_POS] = super_block[SB_AVAILABLE_ENTRY_POS] - 1;
  FD = _get_FD();
  entry -> FD = FD;                                  
  opened_file[FD] = entry;                            //put entry into open file table. 
  inode[INODE_DATA_BLK_POS] = data_blk_pos;           //set data block position in root node
  root_node[RI_INODE_BLOCK_POS(FD)] = inode_blk_pos;  //set ionde block position in root node
  memcpy(&(root_node[RI_FILE_NAME_POS(FD)]), name, strlen(name)); //set file name in root node

  //write the blocks
  disk_ret = writeBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if (disk_ret < 0) return disk_ret;
  disk_ret = writeBlock(fd_FS, ROOT_INODE_POS, root_node);
  if (disk_ret < 0) return disk_ret;
  disk_ret = writeBlock(fd_FS, inode_blk_pos, inode);
  if (disk_ret < 0) return disk_ret;
  _write_time(inode, INODE_CREAT_TIME_POS, inode_blk_pos);
  _write_time(inode, INODE_ACCESS_TIME_POS, inode_blk_pos);
  return FD;
}

int tfs_close(fileDescriptor FD) {
  int disk_ret;
  char root_node[BLOCKSIZE];
  char super_block[BLOCKSIZE];
  Entry *entry = opened_file[FD];
  //check if FD is valid
  if (FD > TOTAL_ENTRY - 1) return TFS_INVALID_FD;
  if (entry == NULL) return TFS_CLOSE_FILE_NOT_OPENED;

  //read disk
  disk_ret = readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if(disk_ret < 0) {return disk_ret;}
  disk_ret = readBlock(fd_FS, ROOT_INODE_POS, root_node);
  if(disk_ret < 0) {return disk_ret;}

  free(entry);
  opened_file[FD] = NULL;
  super_block[SB_AVAILABLE_ENTRY_POS] = super_block[SB_AVAILABLE_ENTRY_POS] - 1;
  return 0;
}

int tfs_write(fileDescriptor FD, char *buffer, int size) {
  int blk_needed;
  int disk_ret;
  int blk_to_write;
  int i;
  int current_data_blk;
  int next_data_blk;
  int write_size;
  char super_block[BLOCKSIZE];
  char data_node[BLOCKSIZE] = {0};
  char inode[BLOCKSIZE];
  blk_needed = size / DATA_SIZE;
  blk_to_write = blk_needed + 1;

  //check if FD is opened
  if (opened_file[FD] == NULL) return TFS_INVALID_FD;

  //read super block and root inode
  disk_ret = readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if(disk_ret < 0) return disk_ret;

    //read super block and root inode
  disk_ret = readBlock(fd_FS, opened_file[FD] -> inode_blk_pos, inode);
  if(disk_ret < 0) return disk_ret;

  //figure out if there is enough room for the write
  if(super_block[SB_AVAILABLE_BLOCK_POS] < blk_needed) return TFS_NO_AVAILABLE_BLOCK;

  //there is enough room, write into blocks that is available
  for (i = 0; i < blk_to_write; i++) {
    //determine the current block position in disk
    if (i == 0) current_data_blk = inode[INODE_DATA_BLK_POS]; 
    else current_data_blk = _find_free_block(super_block);
    assert(current_data_blk >= 0);
    //determine the size to write
    if (i == blk_to_write - 1) {next_data_blk = 0; write_size = size % DATA_SIZE;}
    else {next_data_blk = _find_free_block(super_block); write_size = DATA_SIZE;}
    assert(next_data_blk >= 0);
    //write the bytes and get the next data block
    if (size < DATA_SIZE) write_size = size;
    data_node[DATA_NEXT_BLK_POS] = next_data_blk;
    memcpy(&data_node[DATA_START_POS], buffer, write_size);
    //write datablock to disk
    disk_ret = writeBlock(fd_FS, current_data_blk, data_node);
    if (disk_ret < 0) return disk_ret;
    //modify super available blocks in super block
    super_block[SB_AVAILABLE_BLOCK_POS] = super_block[SB_AVAILABLE_BLOCK_POS] - 1;
  }

  //update super block and root node to disk
  disk_ret = writeBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if (disk_ret < 0) return disk_ret;
  _write_time(inode, INODE_ACCESS_TIME_POS, opened_file[FD] -> inode_blk_pos);
  return 0;
}

int tfs_delete(fileDescriptor FD) {
  int next_node_pos = 0;
  int disk_ret;
  int cur_node_pos;
  int inode_pos;
  char i;
  char cur_node[BLOCKSIZE];
  char super_block[BLOCKSIZE];
  char root_node[BLOCKSIZE];
  char empty_blk[BLOCKSIZE] = {0};

  if (opened_file[FD] == NULL) return TFS_INVALID_FD;

  cur_node_pos = opened_file[FD] -> inode_blk_pos;
  next_node_pos = cur_node_pos;

  disk_ret = readBlock(fd_FS, next_node_pos, cur_node);
  if (disk_ret < 0) return disk_ret;
  disk_ret = readBlock(fd_FS, next_node_pos, root_node);
  if (disk_ret < 0) return disk_ret;

  cur_node_pos = next_node_pos;
  //read from disk
  disk_ret = readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if (disk_ret < 0) return disk_ret;
  disk_ret = readBlock(fd_FS, ROOT_INODE_POS, root_node);
  if (disk_ret < 0) return disk_ret;

  //delete data from disk
  next_node_pos = cur_node[DATA_NEXT_BLK_POS];
  writeBlock(fd_FS, cur_node_pos, empty_blk);

  //update bitmap
  super_block[SB_BIT_MAP_POS + cur_node_pos] = FREE;
  //update available block position
  super_block[SB_AVAILABLE_BLOCK_POS] = super_block[SB_AVAILABLE_BLOCK_POS] + 1;
  
  //update root node
  for (i = 0; i< RI_ENTRY_SIZE; i++) {
    root_node[RI_INODE_BLOCK_POS(FD) + i] = 0;
  }
  disk_ret = writeBlock(fd_FS, ROOT_INODE_POS, root_node);
  if (disk_ret < 0) return disk_ret;
  
  //delete inode
  inode_pos = opened_file[FD] -> inode_blk_pos;
  disk_ret = writeBlock(fd_FS, inode_pos, empty_blk);
  if (disk_ret < 0) return disk_ret;

  //update bitmap
  super_block[SB_BIT_MAP_POS + inode_pos] = FREE;
  super_block[SB_BIT_MAP_POS + inode_pos + 1] = FREE;
  disk_ret = writeBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  if (disk_ret < 0) return disk_ret;

  //delete file from opened file table
  free(opened_file[FD]);
  opened_file[FD] = NULL;

  return 0;
}

int tfs_readByte(fileDescriptor FD, char *buffer) {
  int disk_ret;
  int offset;
  int data_blk;
  char read;
  char data[BLOCKSIZE];
  char inode[BLOCKSIZE];

  //return errno if FD not valid
  if (opened_file[FD] == NULL) return TFS_INVALID_FD;

  //read inode from disk
  disk_ret = readBlock(fd_FS, opened_file[FD] -> inode_blk_pos, inode);
  if (disk_ret < 0) return disk_ret;

  offset = inode[INODE_POINTER_POS];
  inode[INODE_POINTER_POS] = offset + 1;
  data_blk = inode[INODE_DATA_BLK_POS];

  //read data
  disk_ret = readBlock(fd_FS, data_blk, data);
  if (disk_ret < 0) return disk_ret;

  read = data[DATA_START_POS + offset];
  memcpy(buffer, &read, sizeof(char));

  //write inode
  disk_ret = writeBlock(fd_FS, opened_file[FD] -> inode_blk_pos, inode);
  if (disk_ret < 0) return disk_ret;

  return 0;
}

int tfs_seek(fileDescriptor FD, int offset) {
  int disk_ret;
  char inode[BLOCKSIZE];

  if (opened_file[FD] == NULL) return TFS_INVALID_FD;

  disk_ret = readBlock(fd_FS, opened_file[FD] -> inode_blk_pos, inode);
  if (disk_ret < 0) return disk_ret;
  inode[INODE_POINTER_POS] = offset;
  disk_ret = writeBlock(fd_FS, opened_file[FD] -> inode_blk_pos, inode);
  if (disk_ret < 0) return disk_ret;
  _write_time(inode, INODE_ACCESS_TIME_POS, opened_file[FD] -> inode_blk_pos);
  return 0;
}

void tfs_defragmentation() {
  char super_block[BLOCKSIZE];
  int blk_occupied;
  int total_blk;
  int i;
  readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  total_blk = super_block[SB_BLOCK_LEN_POS];
  blk_occupied = total_blk - super_block[SB_AVAILABLE_BLOCK_POS];
  blk_occupied = blk_occupied - 2;
  for (i = 0; i < blk_occupied*2;i++) {
    super_block[SB_BIT_MAP_POS + total_blk - i] = OCCUPIED;
    super_block[SB_BIT_MAP_POS + i + 2] = FREE;
  }
  writeBlock(fd_FS, SUPER_BLOCK_POS, super_block);
}

int tfs_rename(fileDescriptor FD, char *new_name) {
  int disk_ret;
  char root_node[BLOCKSIZE];

  if (strlen(new_name) > MAX_NAME_LEN) return TFS_EXCEED_NAME_LEN_MAX;
  if (opened_file[FD] == NULL) return TFS_INVALID_FD;

  disk_ret = readBlock(fd_FS, ROOT_INODE_POS, root_node);
  if (disk_ret < 0) return disk_ret;

  memcpy(&root_node[RI_FILE_NAME_POS(FD)], new_name, RI_ENTRY_SIZE - 1);

  disk_ret = writeBlock(fd_FS, ROOT_INODE_POS, root_node);
  if (disk_ret < 0) return disk_ret;

  return 0;
}

void tfs_readdir() {
  int i;
  char root_node[BLOCKSIZE];
  char entry[RI_ENTRY_SIZE];

  readBlock(fd_FS, ROOT_INODE_POS, root_node);

  for (i = 0 ; i < TOTAL_ENTRY; i++) {
    if (root_node[RI_INODE_BLOCK_POS(i)] != 0){
      memcpy(entry, &root_node[RI_INODE_BLOCK_POS(i)], RI_ENTRY_SIZE);
      printf("inode: %d, filename: %s\n", (int)entry[0], &entry[1]);
    }
  }
}

void _write_time(char *inode, int position, int inode_pos) {
  time_t now;
  time(&now);
  localtime(&now); 
  memcpy(&inode[position], &now, sizeof(time_t));
  writeBlock(fd_FS, inode_pos, inode);
}

time_t _get_time(char *inode, int position) {
  time_t time;
  memcpy(&time, &inode[position], sizeof(time_t));
  return time;
}

void tfs_stat(fileDescriptor FD) {
  time_t access;
  time_t create;
  if (opened_file[FD] == NULL) fprintf(stderr, "INVALID FD\n");
  char inode[BLOCKSIZE];
  readBlock(fd_FS, opened_file[FD] -> inode_blk_pos, inode);
  access = _get_time(inode, INODE_CREAT_TIME_POS);
  create = _get_time(inode, INODE_ACCESS_TIME_POS);
  printf("fd: %d\n created time\n %s last accessed time\n %s", FD, ctime(&create), ctime(&access));
}

void tfs_display_framents() {
  char super_block[BLOCKSIZE];
  int i;
  readBlock(fd_FS, SUPER_BLOCK_POS, super_block);
  printf("\ndisk image: ");
  for (i = 0; i < super_block[SB_BLOCK_LEN_POS]; i++) {
    if (super_block[SB_BIT_MAP_POS + i] == FREE) {
      printf(".");
    } else {
      printf("/");
    }
  }
  printf("\n");
}




