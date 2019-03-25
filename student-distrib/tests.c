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


/* read_data_fromfile - Example
 *
 * Reads the data from the file according to the given filename
 * Inputs: int8_t* filename
 * Outputs: PASS/FAIL
 * Side Effects: None
 */

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

/* Read_By_Index_Tests - Example
 *
 * Checks all 17 entries of the filesystem
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */

int read_by_index_test(int index){

	clear();
	setposition(0,0);
	dentry_t a;
		if(read_dentry_by_index(index,&a) == -1){
			printf("ERROR! Entry not found\n");
			return FAIL;
		}


		printf("File Name: %s\n",a.fname);
		printf("File Type: %d\n",a.file_type);
		printf("Inode Num: %d\n",a.inode_num);




	return read_data_fromfile(a.fname);
}





/* read_by_name_test- Example
 *
 * Reads the data from the file according to the given filename. tests the read_dentry_by_name function.
 * Inputs: int8_t* filename
 * Outputs: PASS/FAIL
 * Side Effects: None
 */
	int read_by_name_test(int8_t* filename){

		clear();
		setposition(0,0);
		dentry_t a;


		if(read_dentry_by_name(filename,&a) == -1){
			printf("ERROR! Entry not found\n");
			return FAIL;
		}

        if(read_data_fromfile(filename)==-1){
            printf("could not read file");
            return FAIL;
        }
		printf("File Name: %s\n",a.fname);
		printf("File Type: %d\n",a.file_type);
		printf("Inode Num: %d\n",a.inode_num);
		return PASS;
	}
/* dir_read_test- Example
 *
 * Lists the directories in the filesystem
 * Inputs: Boot block address
 * Outputs: PASS/FAIL
 * Side Effects: None
 */
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
/* testing_fs_open- Example
 *
 * Tests to see if fs_open works 
 * Inputs: int8_t* filename
 * Outputs: PASS/FAIL
 * Side Effects: None
 */
int testing_fs_open(int8_t* filename){
	clear();
	setposition(0,0);
	if(fs_open(filename)==-1){
		printf("error opening");
		return FAIL;
	}
	printf("FS OPEN SUCCESSFULL\n");
	return PASS;
}
/* testing_fs_read
 *
 * Tests to see if fs_read returns the correct data based on fsopens file descriptor
 * Inputs: uint32_t nbytes.
 * Outputs: PASS/FAIL
 * Side Effects: None
 */
int testing_fs_read(uint32_t nbytes){
	int i;
	uint8_t  buf[nbytes];
	if(fs_read(buf,nbytes)==-1){
	 printf("error reading");
	 return FAIL;
	}
	for(i=0;i<nbytes;i++){
			putc(buf[i]);

	}


	return PASS;
}



/* Checkpoint 2 tests */
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(unsigned int start ){
  boot_block_addr=start;



	//CHECKPOINT 2 TESTs


	 // TEST CASE 1 LIST ALL FILES IN DIRECTORY
	// TEST_OUTPUT("dir_read_test", dir_read_test((unsigned int *) start) );

	 // TEST CASE 2 READ FROM FILES
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("frame0.txt"));
	 //TEST_OUTPUT("read_data_fromfile", read_data_fromfile("cat"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("counter"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("fish"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("frame1.txt"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("grep"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("hello"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("ls"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("pingpong"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("shell"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("sigtest"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("syserr"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("testprint"));
	 TEST_OUTPUT("read_data_fromfile", read_data_fromfile("verylargetextwithverylongname.tx"));
	// TEST_OUTPUT("read_data_fromfile", read_data_fromfile("verylargetextwithverylongname.txt"));// returns FAIL

	// TEST CASE 3 RTC DRIVER TESTS:
	/*	Tests for the RTC Driver, Tests rtc write by changing frequency. And Read by accepting an interrupt.	*/

	//TEST_OUTPUT("testing_rtc_driver Open Function",testing_rtc_driver(-1));
	// TEST_OUTPUT("testing_rtc_driver Over Limit",testing_rtc_driver(2));
//	 TEST_OUTPUT("testing_rtc_driver Higher Frequency",testing_rtc_driver(512));
	// TEST_OUTPUT("testing_rtc_driver Not a power of 2",testing_rtc_driver(6));
	// TEST_OUTPUT("testing_rtc_driver More than 1024",testing_rtc_driver(2048));

	// TEST CASE 4
//	TEST_OUTPUT("read_by_index_test", read_by_index_test(1));

	// TEST CASE 5
//	TEST_OUTPUT("read_by_name_test", read_by_name_test("frame0.txt"));

// TEST CASE 6
/*   testing fs_open and fs_read   */

//testing_fs_open("frame0.txt");
//TEST_OUTPUT("testing_fs_read", testing_fs_read(187));

//testing_fs_open("verylargetextwithverylongname.tx");
//TEST_OUTPUT("testing_fs_read", testing_fs_read(187));

//testing_fs_open("verylargetextwithverylongname.txt");
//TEST_OUTPUT("testing_fs_read", testing_fs_read(187));

//testing_fs_open("frame0.txt");
//TEST_OUTPUT("testing_fs_read", testing_fs_read(187));







	// launch your tests here
}
