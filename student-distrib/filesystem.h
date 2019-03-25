/* Header file for the FileSystem */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define FILE_NAME_SIZE          32
#define DENTRY_RESERVED         6

#define FS_BLOCK_SIZE           4096
#define BYTES_IN_INT            4
#define ABS_BLK_OFFSET        (FS_BLOCK_SIZE/BYTES_IN_INT)

#define BOOT_BLOCK_SIZE         64
#define BOOT_BLOCK_DIR_ENTRIES  (BOOT_BLOCK_SIZE-2)
#define NUM_INODES              64


#define INODE_MAX_D_BLOCKS      1023
#define MAX_FILE_SIZE           (INODE_MAX_D_BLOCKS*FS_BLOCK_SIZE)






/* Change these includes to what is necessary */
#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "debug.h"
#include "tests.h"
#include "idt.h"
#include "keyboard.h"
#include "rtc.h"
#include "paging.h"


/* Struct for a Directory Entry */
typedef struct dentry {
    char fname[FILE_NAME_SIZE];
    uint32_t file_type;
    uint32_t inode_num;
    uint32_t reserved[DENTRY_RESERVED];
} dentry_t;


/* Giving the multiboot_struct into fs_open for the boot block addr */
extern int32_t fs_open(unsigned int mod_start);

extern int32_t fs_close();

extern int32_t fs_read(void* buf, int32_t nbytes);

extern int32_t fs_write(void* buf, int32_t nbytes);

/* Searches for a directory entry by name, copies over elements and returns a pointer to the directory entry */
extern int32_t read_dentry_by_name(const int8_t* fname, dentry_t* dentry);

/* Searches for a directory entry by index of the directory entries in boot block(not inode#), copies over elements and returns a pointer to the directory entry */
extern int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);

/* Searches for a directory entry by inode#, copies over a number of elements from an offset into buf* */
extern int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

#endif
