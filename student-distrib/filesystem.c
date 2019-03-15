
#include "filesystem.h"


/* Number of Directory Entries */
static uint32_t dir_entries;
/* Number of inodes */
static uint32_t num_inodes;
/* Number of data blocks */
static uint32_t num_dblocks;

/* Global boot block variable for starting address of FileSystems */
static module_t* boot_block_module;
  uint32_t* boot_block_addr;


int32_t fs_open(unsigned int mod_start){

    /* Assign boot_block_module for FileSystem Read functions */
    boot_block_addr = mod_start;
    return 0;
}

int32_t fs_close(){

  if(boot_block_module == NULL)
    return -1;


/****** Should we really reset this? *****/
    /* Reset boot_block_module */
    boot_block_module = NULL;

    return 0;
}

extern int32_t fs_read(void* buf, int32_t nbytes){
  /* If <= 0 bytes to be read, invalid input */
  if(nbytes <= 0)
    return -1;

  if(buf == NULL)
    return -1;

      /* Fill in the rest later */


}

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


int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry){

  /* Error handling for dentry == NULL */
  if(dentry == NULL)
    return -1;
  /* Error handling for fname == NULL */
  if(fname == NULL)
    return -1;

  /* Error handling for boot_block_addr == NULL */
  if(boot_block_addr == NULL)
    return -1;

  /* Setting boot_block_addr for filesystem read */

  int i = 0;
  /* Making a local copy of file name */
  /******* Do we need to make this local copy ****/
  char file_name[FILE_NAME_SIZE];
  while(fname[i] != '\0'){
    file_name[i] = fname[i];
    i++;
  }

  /* Copy over # dir_entries */
  dir_entries = *(boot_block_addr);
  /* Copy over # inodes */
  /****** Should this be 4 or 1? *******/
  num_inodes =  *(boot_block_addr + 1);
  /* Copy over # data blocks */
  /****** Should this be 8 or 2? *******/
  num_dblocks = *(boot_block_addr + 2);

  /* We get the pointer to the first directory */
  uint32_t* first_directory = (uint32_t*)(boot_block_addr + 16);

  /* Checking if this is the directory entry */
  /***** Should bytes being compared be i or FILE_NAME_SIZE ******/
  if(strncmp(first_directory, file_name, i) == 0){
    strncpy(dentry->fname, file_name,i);
    dentry->file_type = 1;
    dentry->inode_num = 0;
    /* Return after filling dentry */
    return 0;
  }

  /* We get the pointer to the first file */
  uint8_t* first_file = (first_directory + 64);
  /* Using this to keep track of the index in the boot_dir_list */
  int j = 2;
  while(strncmp((first_file + j*64), file_name, i) != 0){
    /* If we could not find the filename, return -1 */
    if(j >= 64)
      return -1;

    j++;
  }

  /* We assume we found the filename and dentry at this point */
  /* j has the value of the index */
  /* Filling in the values of the dentry */

  strncpy(dentry->fname, file_name,i);
  /* 32 + 4 since first 32 bytes are for filename */
  dentry->file_type = (32 + 4 + first_file + j*64);
  /* 32 + 8 since first 32 bytes are for filename */
  dentry->inode_num = (32 + 8 + first_file + j*64);

/* Return 0 on success */
return 0;


}

int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){

  /* Error handling for dentry == NULL */
  if(dentry == NULL)
    return -1;

  /* Error handling for index > 62); index starts at 0 for dir "." */
  if(index > BOOT_BLOCK_DIR_ENTRIES)
    return -1;

 /* Setting boot block address */


/* Setting address to dentry that we want to read from */
  uint8_t* dentry_addr = (uint8_t*) ((boot_block_addr +32+ (index * 16)));

/***** Hopefully this copies over all 32 bytes *****/
 strncpy(dentry->fname, (int8_t*) dentry_addr,strlen(dentry_addr));


/****** Is this assignment fine or should we use offset to access data elements ****/
 dentry->file_type = ((dentry_t*)dentry_addr)->file_type;
 dentry->inode_num = ((dentry_t*)dentry_addr)->inode_num;

}

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
  /****** Should this be 4 or 1? *******/
  num_inodes =  *(boot_block_addr + 4);
  /* Copy over # data blocks */
  /****** Should this be 8 or 2? *******/
  num_dblocks = *(boot_block_addr + 8);
  /* Given inode index cannot be bigger than total number of inodes */
  if(inode >= num_inodes || inode > BOOT_BLOCK_DIR_ENTRIES)
    return -1;

  /* Now that we've made sure everything is Gucci, on to
  *  writing the actual function */


   /* Length in Bytes */
 uint32_t inode_file_size;
 /* Num of data blocks in inode */
 uint32_t inode_d_blocks;
/* Points to 0th inode */
 uint32_t* inode_start_addr = boot_block_addr + FS_BLOCK_SIZE;
/* Pointer to required inode */
 uint32_t* inode_addr = inode_start_addr + (inode * FS_BLOCK_SIZE);
/* Pointer to data block[0] in absolute block numbers */
/* Will be useful in indexing into data blocks */
 uint32_t* dblock_start_addr = inode_start_addr + (num_inodes*FS_BLOCK_SIZE);


 /* Now that we have a pointer to the inode, let the dereferencing begin */

 inode_file_size = *(inode_addr);
 inode_d_blocks = (inode_file_size)/DBLOCK_SIZE;
 if(inode_file_size < DBLOCK_SIZE)
  inode_d_blocks = 1;

  /* Starting # of data block */



}
