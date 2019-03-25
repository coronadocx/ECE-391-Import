
#include "filesystem.h"


/* Number of Directory Entries */
static uint32_t dir_entries;
/* Number of inodes */
static uint32_t num_inodes;
/* Number of data blocks */
static uint32_t num_dblocks;
/* global dentry struct to be set based on fs_open */
static dentry_t a;
static dentry_t b;
/* the boot block address */
uint32_t* boot_block_addr;



/*
 *  set_mod_start
 *   DESCRIPTION: Function that assigns the boot_block_addr
 *   ARGUMENTS: unsigned int boot block address
 *   OUTPUTS:None
 *   RETURN VALUE: 0 for success -1 for error
 *   SIDE EFFECTS: boot_block_addr initialized
 */
int32_t set_mod_start(unsigned int mod_start){

    /* Assign boot_block_address for FileSystem Read functions */
    boot_block_addr = (unsigned int*) mod_start;
    return 0;
}
/*
 *  fs_open
 *   DESCRIPTION: sets the global dentry based on filename
 *  ARGUMENTS: file descriptor
 *   OUTPUTS:None
 *   RETURN VALUE: 0 for success -1 for error
 *   SIDE EFFECTS: sets global dentry_t structure based on filename
 */
extern int32_t fs_open(int8_t* filename){
  if(read_dentry_by_name(filename,&a)==-1){
    return -1;
  }
  return 0;
}

/*
 * fs_close
 *   DESCRIPTION:function that resets the global dentry struct
 *   ARGUMENTS : none
 *   OUTPUTS:None
 *   RETURN VALUE: 0 for success -1 for error
 *   SIDE EFFECTS: returns 0 for now may have to populate for future checkpoints
 */

int32_t fs_close(){
    a=b;
    return 0;
}

/*
 * fs_read
 *   DESCRIPTION: populates the buffer based on the file descriptor used in fs_open.
*    ARGUMENTS: Buffer and number of bytes to be read from the file
 *   OUTPUTS:None
 *   RETURN VALUE:0 for success -1 for error
 *   SIDE EFFECTS: populates th buffer
 */

extern int32_t fs_read(void* buf, int32_t nbytes){
  /* If <= 0 bytes to be read, invalid input */
  if(nbytes <= 0)
    return -1;

  if(buf == NULL)
    return -1;

   if( read_data(a.inode_num,0,(uint8_t*) buf,nbytes)==-1){
     return -1;
   }

      /* Fill in the rest later */


return 0;
}

/*
 * fs_write
 *   DESCRIPTION: Read only file system. just does error checking
*    ARGUMENTS: buffer and number of bytes to be read
 *   OUTPUTS:None
 *   RETURN VALUE:0 for success -1 for error
 *   SIDE EFFECTS: none
 */
extern int32_t fs_write(void* buf, int32_t nbytes){
    /* If <= 0 bytes to be written, return success */
    if(nbytes <= 0)
      return 0;

    /* Read only filesystem, but NULL ptr handling */
    if(buf == NULL)
        return -1;

    /* It is a read only filesystem */
    return -1;

};


/*
 * read_dentry_by_name
 *   DESCRIPTION: Function which searches for file by name
  * ARGUMENTS: file name and the dentry struct to be populated
 *   OUTPUTS:None
 *   RETURN VALUE: 0 for success -1 for error
 *   SIDE EFFECTS: dentry struct is populated with info about the file found by given filename
 */
int32_t read_dentry_by_name(const int8_t* fname, dentry_t* dentry){

  /* Error handling for dentry == NULL */
  if(dentry == NULL)
    return -1;
  /* Error handling for fname == NULL */
  if(fname == NULL)
    return -1;

  /* Error handling for boot_block_addr == NULL */
  if(boot_block_addr == NULL)
    return -1;

  /* Copy over # dir_entries */
  dir_entries = *(boot_block_addr);
  /* Copy over # inodes */
  num_inodes =  *(boot_block_addr + 1);
  /* Copy over # data blocks */
  num_dblocks = *(boot_block_addr + 2);

  /* We get the pointer to the first directory */
  uint32_t* first_directory = (uint32_t*)(boot_block_addr + SKIP_TO_DIR_ENTRIES);

  /* Checking if this is the directory entry */
  /***** Should bytes being compared be i or FILE_NAME_SIZE ******/
  //int32_t strncmp(const int8_t* s1, const int8_t* s2, uint32_t n);
  if(strncmp((int8_t*)first_directory, fname, strlen(fname)) == 0){
    strncpy(dentry->fname, fname,strlen(fname));
    dentry->file_type = 1;
    dentry->inode_num = 0;
    /* Return after filling dentry */
    return 0;
  }

  /* We get the pointer to the first file */
  int8_t* first_file = (int8_t*) (first_directory + SKIP_TO_DIR_ENTRIES);
  /* Using this to keep track of the index in the boot_dir_list */
  int j = 0;
  while(strncmp((first_file + j*BOOT_BLOCK_SIZE ), fname, strlen(fname)) != 0){
    /* If we could not find the filename, return -1 */
    if(j >= MAX_FILE_NUM)
      return -1;

    j++;
  }

  /* We assume we found the filename and dentry at this point */
  /* j has the value of the index */
  /* Filling in the values of the dentry */

  strncpy((int8_t*) dentry->fname, fname,FILE_NAME_SIZE);
  /* 32 + 4 since first 32 bytes are for filename */
  dentry->file_type = 0;
  dentry->file_type = *(FILE_NAME_SIZE + first_file + j*BOOT_BLOCK_SIZE );
  /* 32 + 8 since first 32 bytes are for filename */
  dentry->inode_num = 0;
  dentry->inode_num = *(FILE_NAME_SIZE + BYTES_IN_INT + first_file + j*BOOT_BLOCK_SIZE );

/* Return 0 on success */
return 0;


}

/*
 * read_dentry_by_index
 *   DESCRIPTION: Function which initializes the Keyboard by enabling the first IRQ
 *    ARGUMENTS: index to read file from and dentry to populate
 *   OUTPUTS:None
 *   RETURN VALUE: 0 for success -1 for error
 *   SIDE EFFECTS: dentry struct is populated with info about the file found by given index
 */

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){

  /* Error handling for dentry == NULL */
  if(dentry == NULL)
    return -1;

    /* Copy over # dir_entries */
    dir_entries = *(boot_block_addr);
    /* Copy over # inodes */
    // +1 to get to number of inodes
    num_inodes =  *(boot_block_addr + 1);
    /* Copy over # data blocks */
    // +2 to get to number of data blocks
    num_dblocks = *(boot_block_addr + 2);
  /* Error handling for index > 62); index starts at 0 for dir "." */
  if(index > BOOT_BLOCK_DIR_ENTRIES || (index >= dir_entries))
    return -1;

/* Setting address to dentry that we want to read from */
  int8_t* dentry_addr = (int8_t*) ((boot_block_addr +SKIP_TO_DIR_ENTRIES+ (index * SKIP_TO_DIR_ENTRIES)));

/* Copy over all 32 bytes*/

 strncpy(dentry->fname, ((dentry_t*)dentry_addr)->fname,FILE_NAME_SIZE);

/* Last entry should always be NULL */


/* Is this assignment fine or should we use offset to access data elements */
 dentry->file_type = ((dentry_t*)dentry_addr)->file_type;
 dentry->inode_num = ((dentry_t*)dentry_addr)->inode_num;
 dentry->fname[FILE_NAME_SIZE] = '\0';

 /* Return 0 on success */
 return 0;

}

/*
 * read_data
 *   DESCRIPTION: Function which reads data from a file
 *   ARGUMENTS: int inode number, offset to read from , buffer to fill with content and length of text to be read
 *   OUTPUTS:None
 *   RETURN VALUE:0 for success -1 for error
 *   SIDE EFFECTS: fills the buffer with text required if successfull
 */

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){

  /* Error handling for buf == NULL */
  if(buf == NULL)
    return -1;

  /* Error handling for zero length or length greater than Max_File_Size; no bytes to be read */
  if(length == 0 || length > MAX_FILE_SIZE)
    return -1;

  if(offset > MAX_FILE_SIZE || (offset + length) > MAX_FILE_SIZE)
    return -1;

  /* Error handling for boot_block_addr == NULL */
  if(boot_block_addr == NULL)
    return -1;

  /* Setting boot_block_addr for filesystem read */

  /* Copy over # dir_entries */
  dir_entries = *(boot_block_addr);
  /* Copy over # inodes */
  // +1 to get to number of inodes from directory entries
  num_inodes =  *(boot_block_addr + 1);
  /* Copy over # data blocks */
  // +2 to get to number of datablocks from directory entries
  num_dblocks = *(boot_block_addr + 2);
  /* Given inode index cannot be bigger than total number of inodes */
  if(inode > NUM_INODES)
    return -1;

  /* Now that we've made sure everything is Gucci, on to
  *  writing the actual function */


   /* Length in Bytes */
 uint32_t inode_file_size;
 /* Num of data blocks in inode */
 uint32_t inode_d_blocks;
/* Points to 0th inode */
 uint32_t* inode_start_addr = boot_block_addr + ABS_BLK_OFFSET;
/* Pointer to required inode */
 uint32_t* inode_addr = inode_start_addr + (inode * ABS_BLK_OFFSET);
/* Pointer to data block[0] in absolute block numbers */
/* Will be useful in indexing into data blocks */
 uint32_t* dblock_start_addr = inode_start_addr + (num_inodes*ABS_BLK_OFFSET);

 uint32_t start_dblock;
 uint32_t end_dblock;

 uint32_t start_dblock_index;
 uint32_t end_dblock_index;

 uint32_t* start_dblock_addr;
 uint32_t* end_dblock_addr;


 /* Now that we have a pointer to the inode, let the dereferencing begin */

 inode_file_size = *(inode_addr);
 inode_d_blocks = ((inode_file_size)/FS_BLOCK_SIZE) + 1;

/* If length in B < 4096, we have to read from only 1 data block */
 if(inode_file_size < FS_BLOCK_SIZE)
  inode_d_blocks = 1;

  if((inode_file_size < length) || (inode_file_size < offset+length))
  return -1;

  /* Starting and ending# of data block */
  start_dblock = offset/(FS_BLOCK_SIZE);
  end_dblock = (offset+length)/(FS_BLOCK_SIZE);

/* Starting and ending indices of data blocks */
  start_dblock_index = *(inode_addr + 1 + start_dblock);
  end_dblock_index = *(inode_addr + 1 +  end_dblock);

/* Starting and ending addresses of data blocks */
  start_dblock_addr = (dblock_start_addr + (start_dblock_index * ABS_BLK_OFFSET));
  end_dblock_addr = (dblock_start_addr + (end_dblock_index * ABS_BLK_OFFSET));

/* If all the reading is to be done from a single d_block */
  if(start_dblock == end_dblock){
    memcpy(buf, ((uint8_t*)start_dblock_addr) + (offset % FS_BLOCK_SIZE), length);

    return 0;
  }

/* If reading has to be done over multiple dblocks */
  uint32_t j;
  uint32_t j_index;
  uint32_t* j_addr;
  uint32_t bytes_to_copy_in_dblock;
  uint32_t bytes_remaining = length;
  j = start_dblock;
  uint32_t offset_pos;
  offset_pos = (offset % FS_BLOCK_SIZE);
  uint32_t buf_offset = 0;

  /* Start_dblock handling */
  bytes_to_copy_in_dblock = FS_BLOCK_SIZE - offset_pos;
  j_index = *(inode_addr + 1 + j);
  j_addr = (dblock_start_addr + (j_index * ABS_BLK_OFFSET));
  memcpy(buf + buf_offset, ((uint8_t*)j_addr) + (offset % FS_BLOCK_SIZE), bytes_to_copy_in_dblock);

  buf_offset += bytes_to_copy_in_dblock;
  bytes_remaining -= bytes_to_copy_in_dblock;
  j++;
  /* These copy over all 4096 bytes of the data block since j!= end_dblock */
  while(j != end_dblock){

  bytes_to_copy_in_dblock = FS_BLOCK_SIZE;
  j_index = *(inode_addr + 1 + j);
  j_addr = (dblock_start_addr + (j_index * ABS_BLK_OFFSET));
  memcpy(buf + buf_offset, ((uint8_t*)j_addr), bytes_to_copy_in_dblock);

  buf_offset += bytes_to_copy_in_dblock;
  bytes_remaining -= bytes_to_copy_in_dblock;
  j++;
  }

  /* End_dblock handling */
  /* We only have those many bytes remaining */
  bytes_to_copy_in_dblock = bytes_remaining;
  j_index = *(inode_addr + 1 + j);
  j_addr = (dblock_start_addr + (j_index * ABS_BLK_OFFSET));
  memcpy(buf + buf_offset, ((uint8_t*)j_addr), bytes_to_copy_in_dblock);
  buf_offset += bytes_to_copy_in_dblock;
  bytes_remaining -= bytes_to_copy_in_dblock;

  if(bytes_remaining != 0){
    printf("There is an error as all bytes have not been copied\n");
    return -1;
  }

  return 0;


}
