#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "filesystem.h"
#include "rtc.h"
#include "i8259.h"

#define PASS 1
#define FAIL 0

#define VMEM_START		0xb8000
#define VMEM_END			0xb9000


#define KMEM_START		0x400000
#define KMEM_END			0x800000

#define KMEM_OFFSET		0x100000

#define VMEM_OFFSET		0x000100
/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

unsigned int boot_block_addr;
static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 *
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) &&
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;

		}

	}

		return result;
	}

		/* page_test_null - Null pointer
		 *
		 * Tries to access NULL pointer, should receive a page fault
		 * Inputs: None
		 * Outputs: PASS/FAIL
		 * Side Effects: None
		 * Coverage: Accessing NULL pointer
		 */
	int page_test_null(){
		printf("This test tries to dereference a NULL Pointer and should page fault\n");
		int *i = NULL;
		int j;
		j = 5; //Just a random value
		*i = j;

		return 0;
	}
	/* page_test_null - Null pointer
	 *
	 * Tries to write to and read from several kernel and video memory addresses
	 * Inputs: None
	 * Outputs: PASS/FAIL
	 * Side Effects: None
	 * Coverage: Accessing different mapped kernel and video memory addresses
	 */

	int page_test(){
		printf("This test tries to access different addresses\n");
		int i;
		int* i_ptr;
		int j;
		j = 0;
		int result = PASS;
		printf("Testing Kernel Memory Addresses\n");
		i_ptr = (int*)KMEM_START;
		/* Testing Kernel Memory Addresses */
		for(i=KMEM_START; i < KMEM_END; i+= KMEM_OFFSET){
				printf("Writing %d into Memory address: %x\n", j, i);
				*i_ptr = j;
				if(*i_ptr != j){
					result = FAIL;
				}
				i_ptr = (int*)(i+KMEM_OFFSET);
				j++;
		}
		printf("Testing Video Memory Addresses\n");
		i_ptr = (int*)VMEM_START;
		for(i=VMEM_START; i < VMEM_END; i+= VMEM_OFFSET){
				printf("Writing %d into Memory address: %x\n", j, i);
				*i_ptr = j;
				if(*i_ptr != j){
					result = FAIL;
				}
				i_ptr = (int*)(i+VMEM_OFFSET);
				j++;
		}

		return result;
	}

	//i=i/0;
		//	asm volatile("int $40");
	 //


	// add more tests here
	/* Checkpoint 2 tests */

	/* Read_By_Index_Tests - Example
	 *
	 * Checks all 17 entries of the filesystem
	 * Inputs: None
	 * Outputs: PASS/FAIL
	 * Side Effects: None
	 * Coverage: Load IDT, IDT definition
	 * Files: x86_desc.h/S
	 */

	int read_by_index_test(){

		clear();
		int i;
		dentry_t a;
		i = 11;
		for(i = 0; i < 17; i++){
			if(read_dentry_by_index(i,&a) == -1){
				printf("ERROR! Entry not found\n");
				return FAIL;
			}


			printf("File Name: %s\n",a.fname);
			printf("File Type: %d\n",a.file_type);
			printf("Inode Num: %d\n",a.inode_num);

		}

		return PASS;
	}






	int read_by_name_test(){

		clear();
		int i;
		char* s;
		dentry_t a;
		for(i = 0; i < 17; i++){
		if(i == 0){
			s = ".";
	}
		else if(i == 1){
			s = "counter";
	}
		else if(i == 2){
			s = "cat";
	}
		else if(i == 3){
			s = "hello";
	}
		else if(i == 4){
		s = "pingpong";
	}
		else if(i == 5){
			s = "frame0.txt";
	}
		else if(i == 6){
			s = "frame1.txt";
	}
		else if(i == 7){
		s = "verylargetextwithverylongname.tx";
	}
		else if(i == 8){
		s = "shell";
	}
		else if(i == 9){
			s = "ls";
	}
		else if(i == 10){
			s = "rtc";
	}
		else if(i == 11){
			s = "testprint";
	}
		else if(i == 12){
			s = "grep";
	}
		else if(i == 13){
			s = "created.txt";
	}
		else if(i == 14){
			s = "fish";
	}
		else if(i == 15){
			s = "sigtest";
	}
		else if(i == 16){
			s = "syserr";
	}

		else return FAIL;


		if(read_dentry_by_name((int8_t*)s,&a) == -1){
			printf("ERROR! Entry not found\n");
			return FAIL;
		}


		printf("File Name: %s\n",a.fname);
		printf("File Type: %d\n",a.file_type);
		printf("Inode Num: %d\n",a.inode_num);
}

		return PASS;
	}




int read_data_test(){

			clear();
			setposition(0,0);
			int i;
			uint8_t buf[200];
				if(read_data(38, 0, buf,187) == -1){
						return FAIL;
				}

			printf("Printing the first thousand contents of the buffer\n");
			for(i = 0; i<200; i++){
				putc(buf[i]);
			}

			return PASS;
		}

int read_data_fromfile(int8_t* filename){
	  clear();
		setposition(0,0);
		dentry_t a;
		int i;
		unsigned int filesize;
		uint32_t* inode_start_addr;
		read_dentry_by_name(filename,&a);
		inode_start_addr = ((unsigned int *)boot_block_addr) + 1024;
		filesize = *(inode_start_addr + (a.inode_num)*1024);
		uint8_t buf[filesize];
		if(read_data(a.inode_num, 0, buf,filesize) == -1){
				return FAIL;
		}
		for(i=0;i<filesize;i++){
			putc(buf[i]);
		}
		printf("\n File Name: %s \n",filename);
		return PASS;

}
int dir_read_test(uint32_t* boot_block_addr){
			if(boot_block_addr == NULL)
			return 0;

			clear();
			setposition(0,0);

			int dir_entries;
			int num_inodes;
			int num_dblocks;
			int i;
			uint32_t* inode_start_addr;
			dentry_t d;
			uint32_t file_size;

			dir_entries = *(boot_block_addr);
			num_inodes  = *(boot_block_addr + 1);
			num_dblocks = *(boot_block_addr + 2);

			inode_start_addr = boot_block_addr + 1024;

			for(i = 0; i < dir_entries; i++){
				if(read_dentry_by_index(i, &d) == -1){
					return FAIL;
				}
				printf("File Name: %s, " , d.fname);
				printf("File Type: %u, ", d.file_type);
				file_size = *(inode_start_addr + (d.inode_num)*1024);
				printf("File Size: %d\n", file_size);
			}

			return PASS;
		}


/* testing_rtc_driver(int32_t rate)
 *
 *	This function or test case takes in a power of 2 as an input and sets the frequency of the RTC
 *	to that value. It can set the frequency upto a maximum of 1024 Hz.
 * Inputs: int32_t rate
 * Outputs: PASS/FAIL
 * Side Effects: Sets the frequency of the rtc
 * Coverage: Accessing different mapped kernel and video memory addresses
 */


int testing_rtc_driver(int32_t rate){
	int a;
	int xval=0;
	int yval=0;
	clear();
	setposition(0,0);
	// int32_t rate = 2;
	if(rate==-1){
		rtc_open(0);
	}
	else if (rtc_write(0,&rate,0)== -1){
			return FAIL;
	}

	while(1){
		a = rtc_read();
		writetovideomem(xval,yval);
	}

	return PASS;
}



/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(unsigned int start ){
	//TEST_OUTPUT("idt_test", idt_test());
	 TEST_OUTPUT("dir_read_test", dir_read_test( (unsigned int*) start));
	//TEST_OUTPUT("read_by_name_test", read_by_name_test());
  boot_block_addr=start;
  // TEST_OUTPUT("read_data_test", read_data_test());
	  //TEST_OUTPUT("read_data_fromfile", read_data_fromfile("frame0.txt"));
	//TEST_OUTPUT("page_test_null", page_test_null());
	// TEST_OUTPUT("page_test", page_test());

	/*	Tests for the RTC Driver, Tests rtc write by changing frequency. And Read by accepting an interrupt.	*/

	//TEST_OUTPUT("testing_rtc_driver Open Function",testing_rtc_driver(-1));
	// TEST_OUTPUT("testing_rtc_driver Over Limit",testing_rtc_driver(2));
	// TEST_OUTPUT("testing_rtc_driver Higher Frequency",testing_rtc_driver(512));
	// TEST_OUTPUT("testing_rtc_driver Not a power of 2",testing_rtc_driver(6));
	// TEST_OUTPUT("testing_rtc_driver More than 1024",testing_rtc_driver(2048));

	// launch your tests here
}
