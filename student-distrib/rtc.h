/* header file for rtc */
#ifndef RTC_H
#define RTC_H

#define RTC_IRQ_NO          8
#define RTC_CMD_PORT        0x70
#define RTC_DATA_PORT       0x71

#define REG_A 				0x0A
#define REG_A_NMI_OFF		0x8A	//Disables NMI
#define REG_B               0x8B	//Disables NMI
#define REG_C               0x0C
#define RTC_MASK            0x40
#define MASK_LOWER 			0xF0


#define MAX_FREQ			1024
#define MIN_FREQ			2
#define INIT_RATE 			5

#define MAX_PROCESS 		6



#include "types.h"

// Hardware setup functions
void init_rtc();

// System Calls for RTC
int32_t rtc_read(int32_t fd,void* buf,int32_t nbytes);
int32_t rtc_write(int32_t fd,const void * buf, int32_t nbytes);
int32_t rtc_open(const uint8_t * filename);
int32_t rtc_close(int32_t fd);

// Helper Functions
void _rtc_set_freq(int32_t freq);
void _rtc_set_freq_hard(int32_t freq);


extern void rtchandler();

// Globals for RTC - FIXME Should these be volatile?
volatile int32_t rtc_proc_intr[MAX_PROCESS];		// Set on virtural interrupt
int32_t rtc_proc_ctr[MAX_PROCESS];					// Counter used to determine when to send interrupt
int32_t rtc_proc_ctr_max[MAX_PROCESS];				// Max value for each counter

#endif
