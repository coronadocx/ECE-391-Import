#ifndef SYS_CALL_H
#define SYS_CALL_H
#include "types.h"

#define PCB_MASK    0xFFE000
#define END_KMEM    0x800000
#define PCB_SIZE    0x002000
#define KERNEL_MEM_SIZE 0x2000

#define IN_USE_INDEX  0
#define FTYPE_INDEX   1
#define RSVD_INDEX_0  2
#define RSVD_INDEX_1  3


#define FILE_OPS_OPEN  0
#define FILE_OPS_READ  1
#define FILE_OPS_WRITE 2
#define FILE_OPS_CLOSE 3



/*
 * halt
 *   DESCRIPTION: halt system call. tearsdown stack and restores to parent process
 *   INPUTS: status -> character 
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure 
 *   SIDE EFFECTS:  
 */
int32_t halt(uint8_t status);
/*
 * execute 
 *   DESCRIPTION: halt system call. tearsdown stack and restores to parent process
 *   INPUTS: status -> character 
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure 
 *   SIDE EFFECTS:  
 */
int32_t execute(const uint8_t* command);
/*
 * write 
 *   DESCRIPTION: write  system call. tearsdown stack and restores to parent process
 *   INPUTS: status -> character 
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure 
 *   SIDE EFFECTS:  
 */
int32_t write(int32_t fd, const void*buf,int32_t nbytes);
/*
 * read
 *   DESCRIPTION: read system call. tearsdown stack and restores to parent process
 *   INPUTS: status -> character 
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure 
 *   SIDE EFFECTS:  
 */
int32_t read(int32_t fd,void*buf,int32_t nbytes);
/*
 * close 
 *   DESCRIPTION: close  system call. tearsdown stack and restores to parent process
 *   INPUTS: status -> character 
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure 
 *   SIDE EFFECTS:  
 */
int32_t close(int32_t fd);
/*
 * open
 *   DESCRIPTION: open system call. tearsdown stack and restores to parent process
 *   INPUTS: status -> character 
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure 
 *   SIDE EFFECTS:  
 */
int32_t open(const uint8_t* filename);
#endif
