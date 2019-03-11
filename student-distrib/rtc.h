/* header file for rtc */
#ifndef RTC_H
#define RTC_H

#define RTC_CMD_PORT        0x70
#define RTC_DATA_PORT       0x71


#define REG_C               0x0C
#define REG_B               0x8B
#define RTC_MASK            0x40

#define RTC_IRQ_NO          8

void init_rtc();

#endif
