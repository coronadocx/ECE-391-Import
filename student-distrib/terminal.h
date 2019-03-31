/* HEADER FILE FOR TERMINAL FILE */
#ifndef TERMINAL_H
#define TERMINAL_H


#define CURSOR_START 0
#define CURSOR_END 15

#define REG_A 				0x0A
#define SELECT_REG_B 				0x0B
#define SELECT_REG_C 				0x0C
#define REG_E 				0x0E
#define REG_F 				0x0F

#define X_WIDTH 80
#define GET_UPPER_8 8
#define VMEMPORT_3D4 0x3D4
#define VMEMPORT_3D5 0x3D5
#define MASK_LOWER_8 0xFF

int init();
int read(int32_t fd,void* buffer,int32_t nbytes);
int write(int32_t fd,void*buf,int32_t nbytes);
int open(const uint8_t* filename);
int32_t close(int32_t fd);
void enable_cursor();
void update_cursor();
#endif
