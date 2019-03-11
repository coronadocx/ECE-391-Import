#include "tests.h"
#include "x86_desc.h"
#include "lib.h"

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
/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* page_fault_outside_range -
 *
 * Tries to access pointer outside the range, should receive a page fault
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Accessing non existent memory address pointer
 */


int page_fault_outside_range(){
	int *addr;
	addr = (int*)0x0b9300;
	*addr = 5;
	return 0;
}


/* Test suite entry point */
void launch_tests(){
	TEST_OUTPUT("idt_test", idt_test());
	// TEST_OUTPUT("page_test_null", page_test_null());
	// TEST_OUTPUT("page_test", page_test());
	// launch your tests here

	TEST_OUTPUT("test_outside_range",page_fault_outside_range());
}
