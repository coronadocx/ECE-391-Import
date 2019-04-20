
#ifndef PAGING_H
#define PAGING_H

#include "lib.h"

#define K_BYTE			1024				//Bytes per kilobyte
#define M_BYTE			(1024*K_BYTE)		//Bytes per megabyte
#define G_BYTE			(1024*M_BYTE)		//Bytes per gigabyte

#define PAGE_SIZE		1024				//Bytes per page
#define PROCESS_SIZE    0x400000            //Size of user-process memory

#define KER_START_ADDR	0x400000			//Starting address of the Kernel (1-expage)
#define VID_START_ADDR	0xB8000				//Starting address of the video memory (8-page)

#define T1_BUF_ADDR     0xB9000
#define T2_BUF_ADDR     0xBA000
#define T3_BUF_ADDR     0xBB000

#define VMEM_SIZE       4096

#define USR_START_ADDR  0x800000

#define VMEM_PAGE       184
#define T1_BUF_PAGE     185
#define T2_BUF_PAGE     186
#define T3_BUF_PAGE     187

//Masks
#define VMEM_OR_MASK    0x7
#define PTABLE_OR_MASK  0x13
#define PTABLE_OR_MASK_USER  0x17
#define KMEM_OR_MASK    0x183
#define UMEM_OR_MASK    0x87

#define PIDMIN 0
#define PIDMAX 5

#define USER_MEM_LOCATION 32
#define VMEM_33 33
#define ONEMB_PAGETABLE 256



typedef struct pg_struct {							//Page Entry Struct
    uint32_t base_addr;
    uint8_t avail;
    uint8_t g;
    uint8_t pat;
    uint8_t d;
    uint8_t a;
    uint8_t pcd;
    uint8_t pwt;
    uint8_t u_s;
    uint8_t r_w;
    uint8_t p;
    uint8_t ps;
    uint8_t is_directory_entry;				//Used to differentiate between page table entries and page data entries
} pe_t;

extern void paging_initialize();
extern uint32_t paging_change_process(uint32_t process_id);
extern uint32_t paging_vidmap();
// extern uint32_t paging_create_entry(pe_t entry);

#endif
