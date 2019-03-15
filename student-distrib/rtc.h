/* header file for rtc */
#ifndef RTC_H
#define RTC_H

#define RTC_CMD_PORT        0x70
#define RTC_DATA_PORT       0x71

#define REG_A 				0x0A
#define REG_A_NMI_OFF		0x8A	//Disables NMI
#define REG_B               0x8B	//Disables NMI
#define REG_C               0x0C
#define RTC_MASK            0x40

#define RTC_IRQ_NO          8

#define MAX_FREQ			1024

void init_rtc();

// System Calls for RTC
int32_t rtc_read();
int32_t rtc_write(int32_t fd,const int32_t * buf, int32_t nbytes);
int32_t rtc_open(const uint8_t * filename);
int32_t rtc_close(int32_t fd);

int32_t rtc_write_test2();

volatile int32_t rtc_interrupt_occurred;

#endif
