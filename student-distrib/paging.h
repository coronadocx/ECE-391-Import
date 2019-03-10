
#ifndef PAGING_H
#define PAGING_H

#include "types.h"

#define K_BYTE			1024				//Bytes per kilobyte
#define M_BYTE			(1024*K_BYTE)		//Bytes per megabyte
#define G_BYTE			(1024*M_BYTE)		//Bytes per gigabyte

#define PAGE_SIZE		1024				//Bytes per page
#define KER_START_ADDR	0x400000			//Starting address of the Kernel (1-expage)
#define VID_START_ADDR	0xB8000				//Starting address of the video memory (8-page)


typedef struct {							//Page Entry Struct
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

extern void paging_initialize(uint32_t * page_directory, uint32_t * page_table_0M_4M);
extern uint32_t paging_create_entry(pe_t entry);

#endif
