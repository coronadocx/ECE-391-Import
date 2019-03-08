/* c file for rtc */
#include "lib.h"
#include "i8259.h"

void init_rtc(){
/* referenced wiki.osdev.org/RTC to understand the rtc */
outb(0x8B,0x70);
char prev = inb(0x71);
outb(0x8B,0x70);
outb(prev|0x40,0x71);
enable_irq(8);
  return;
}
