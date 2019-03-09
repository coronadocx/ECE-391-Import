

#include "paging.h"
#include "lib.h"

static void * page_directory[PAGE_SIZE] __attribute__((aligned(4096)));	//Single page directory for system, 1024 entries
static void * page_table_0M_4M[PAGE_SIZE] __attribute__((aligned(4096)));	//Page table for memory block 0-4MB, 1024 entries


/*
 * 	initialize_paging
 *
 *	INPT: none
 *	OUTPUT: none
 *	RETURN VALUE: none
 *	SIDE EFFECTS: Changes x86 control registers to allow paging
 *				  Populates one page directory
 *				  Populates 1023 page tables (the last one is for the kernel)
 *
 */
void initialize_paging()
{
	uint32_t i;
	//NOTE Only allocate ram when the system actually needs it --> Don't make 4MB of page tables, dummy

    //Populate the first page table
    for (i=0; i<PAGE_SIZE; i++) {
    	if (i >= 184 && i < 191){	//XXX This is really magic but basically these are the indices of the video memory
    		page_table_0M_4M[i] = ((VID_START_ADDR + (PAGE_SIZE*i)) & 0xFFFFF003);	//Map 8 pages to video memory
    	}

    	else
    		page_table_0M_4M[i] = 0x0;	//Set every other page as "not present"
    }


    //Populate the page_directory, identity map the video memory and the kernel memory
    for (i=0; i<PAGE_SIZE; i++) {
    	if (i==0)
    		page_directory[i] = (page_table_0M_4M);	//Points to the first page table
    	else if (i==1)
    		page_directory[i] = (KER_START_ADDR & 0xFFC00193);	//Points to the kernel page
    	else
    		page_directory[i] = 0x0;	//Set every other entry as "not present"

    }


    /*
     * 	Enable Paging
     *
     *	PE  = 1 - bit 1 of CR0
     *	PG  = 1 - bit 31 of CR0
     * 	To set PE and PG:
     *		OR CR0 with 0x80000001
     *
     *	PAE = 0 - bit 4 of CR4
     *	PSE = 1 - bit 5 of CR4
     *  To clear PAE:
     *		AND CR4 with 0xFFFFFFF7
     * 	To set PSE:
     *		OR CR4 with 0x00000010
     */


    asm volatile("                        \n\
    	movl	%0, %%eax	                    \n\
    	movl 	%%eax, %%cr3			\n\
    	movl	%%cr0, %%eax			\n\
    	orl		$0x80000001, %%eax		\n\
    	movl	%%eax, %%cr0 			\n\
    	movl 	%%cr4, %%eax			\n\
    	andl 	$0xFFFFFFF7, %%eax		\n\
    	orl		$0x00000010, %%eax		\n\
    	movl 	%%eax, %%cr4 			\n\
    	"
    	:
    	: "r"(page_directory)
      : "eax","memory", "cc"
    );
}
