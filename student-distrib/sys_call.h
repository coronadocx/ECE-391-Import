#ifndef SYS_CALL_H
#define SYS_CALL_H
#include "types.h"

#define PCB_MASK    0xFFE000
#define END_KMEM    0x800000
#define PCB_SIZE    0x008000
#define KERNEL_MEM_SIZE 0x2000

#define IN_USE_INDEX  0
#define FTYPE_INDEX   1
#define RSVD_INDEX_0  2
#define RSVD_INDEX_1  3


#define FILE_OPS_OPEN  0
#define FILE_OPS_READ  1
#define FILE_OPS_WRITE 2
#define FILE_OPS_CLOSE 3




int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t write(int32_t fd, const void*buf,int32_t nbytes);
int32_t read(int32_t fd,void*buf,int32_t nbytes);
int32_t close(int32_t fd);
int32_t open(const uint8_t* filename);
#endif
