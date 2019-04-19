/* header file for the iterupt descriptor table */

#ifndef IDT_H
#define IDT_H

#define EXCEPTION_COUNT 32
#define SYSTEMCALLNO 0x80
#define PITNO 0x20
void initialize_IDT();
#endif
