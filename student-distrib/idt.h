/* header file for the iterupt descriptor table */

#ifndef IDT_H
#define IDT_H
extern void initialize_IDT();

#define EXCEPTION_COUNT 32
#define SYSTEMCALLNO 0x80
#endif
