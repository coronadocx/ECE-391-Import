
#ifndef PAGING_H
#define PAGING_H

#include "types.h"

#define K_BYTE			1024				//Bytes per kilobyte
#define M_BYTE			(1000*K_BYTE)		//Bytes per megabyte
#define G_BYTE			(1000*M_BYTE)		//Bytes per gigabyte

#define PAGE_SIZE		1024				//Bytes per page
#define KER_START_ADDR	0x400000			//Starting address of the Kernel (1-expage)
#define VID_START_ADDR	0xB8000				//Starting address of the video memory (8-page)
extern void initialize_paging();

#endif
