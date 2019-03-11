/* c file for rtc */
#include "lib.h"
#include "i8259.h"
#include "rtc.h"

void init_rtc(){
/* referenced wiki.osdev.org/RTC to understand the rtc */
outb(REG_B,RTC_CMD_PORT);
char prev = inb(RTC_DATA_PORT);
outb(REG_B,RTC_CMD_PORT);
outb(prev|RTC_MASK,RTC_DATA_PORT);
enable_irq(RTC_IRQ_NO);
  return;
}
