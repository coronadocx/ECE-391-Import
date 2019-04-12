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

#define NUMBEROFPROCESSESSUPPORTED 6
#define ENTRIESINFDARRAY 8
#define SIZEOFOPERATIONSTABLE 4
#define NOTINUSE 0
#define INUSE 1
#define MAXBUFSIZEEXECUTE 128

#define STARTOFEIPINEXECUTABLE 24
#define EXTENDEDPAGESIZE 0x400000
#define VIRTUALADDRESSFOREXECUTABLEDATA 0x8048000
#define INVALIDORFAIL -1
#define FILETYPEDEVICE 0
#define FILETYPEDIRECTORY 1
#define FILETYPEGENERAL 2
#define DEFAULTFILEOFFSET 0
#define METADATAEXECUTESIZE 40
#define FDSTART 2
#define FDEND 7


int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t write(int32_t fd, const void*buf,int32_t nbytes);
int32_t read(int32_t fd,void*buf,int32_t nbytes);
int32_t close(int32_t fd);
int32_t open(const uint8_t* filename);
int32_t getargs(uint8_t * buf, int32_t nbytes);
int32_t vidmap(uint8_t ** screen_start);
int32_t set_handler(int32_t signum,void* handler_address);
int32_t sigreturn(void);


#endif
