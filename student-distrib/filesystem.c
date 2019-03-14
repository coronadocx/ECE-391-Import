
#include "filesystem.h"


/* Number of Directory Entries */
extern uint32_t dir_entries;
/* Number of inodes */
extern uint32_t num_inodes;
/* Number of data blocks */
extern uint32_t num_dblocks;

/* Global boot block variable for starting address of FileSystems */
module_t* boot_block_module;


int32_t fs_open(multiboot_info_t *mbi){

    /* Initially set boot_block_module to NULL */
    boot_block_module = NULL;

    /* Error handling for when mbi or mbi->mods_addr is NULL */
    if(mbi == NULL)
      return -1;
    if(mbi->mods_addr == NULL)
      return -1;

    /* Assign boot_block_module for FileSystem Read functions */
    boot_block_module = (module_t*)mbi->mods_addr;
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
  if(dentry_t == NULL)
    return -1;
  /* Error handling for fname == NULL */
  if(fname == NULL)
    return -1;

  /* Error handling for boot_block_addr == NULL */
  if((uint32_t* )boot_block_module->mod_start == NULL)
    return -1;

  /* Setting boot_block_addr for filesystem read */
  uint32_t* boot_block_addr = (uint32_t* )boot_block_module->mod_start;

  int i = 0;
  /* Making a local copy of file name */
  /******* Do we need to make this local copy ****/
  uint8_t file_name[FILE_NAME_SIZE];
  while(fname[i] != '\0'){
    file_name[i] = fname[i];
    i++;
  }

  /* Copy over # dir_entries */
  dir_entries = *(boot_block_addr);
  /* Copy over # inodes */
  /****** Should this be 4 or 1? *******/
  num_inodes =  *(boot_block_addr + 4);
  /* Copy over # data blocks */
  /****** Should this be 8 or 2? *******/
  num_dblocks = *(boot_block_addr + 8);

  /* We get the pointer to the first directory */
  uint32_t* first_directory = (boot_block_addr + 64);

  /* Checking if this is the directory entry */
  /***** Should bytes being compared be i or FILE_NAME_SIZE ******/
  if(strncmp(first_directory, file_name, i) == 0){
    strcpy(dentry->fname, file_name);
    dentry->file_type = 1;
    dentry->inode_num = 0;
    /* Return after filling dentry */
    return 0;
  }

  /* We get the pointer to the first file */
  uint8_t* first_file = (first_directory + 64);
  /* Using this to keep track of the index in the boot_dir_list */
  int j = 0;
  while(strncmp((first_file + j*64), file_name, i) != 0){
    /* If we could not find the filename, return -1 */
    if(j >= 62)
      return -1;

    j++;
  }

  /* We assume we found the filename and dentry at this point */
  /* j has the value of the index */
  /* Filling in the values of the dentry */

  strcpy(dentry->fname, file_name);
  /* 32 + 4 since first 32 bytes are for filename */
  dentry->file_type = (32 + 4 + first_file + j*64);
  /* 32 + 8 since first 32 bytes are for filename */
  dentry->inode_num = (32 + 8 + first_file + j*64);

/* Return 0 on success */
return 0;


}

int32_t read_dentry_by_index(uint32_t* index, dentry_t* dentry){

  /* Error handling for dentry == NULL */
  if(dentry_t == NULL)
    return -1;

}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){

  /* Error handling for buf == NULL */
  if(buf == NULL)
    return -1;

  /* Error handling for zero length; no bytes to be read */
  if(length == 0)\
    return -1;

  /* Given inode index cannot be bigger than total number of inodes */
  if(inode >= num_inodes)
      return -1;


}
