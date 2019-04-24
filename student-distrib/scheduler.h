#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "lib.h"
#include "pcb.h"
#include "sys_call.h"

#define NUM_TERMS	3
#define KEYBOARD_BUFFER_LENGTH 128
typedef struct terminal_struct{
	int8_t		lb[128];
	int32_t 	noc;
	int32_t 	esp;
	int32_t		ebp;
	int32_t   pid;
	int32_t screen_x;
	int32_t screen_y;
}terminal_t;

typedef struct sched{
  	terminal_t 	terminals[NUM_TERMS];
  	uint8_t * 	vid_bufs[NUM_TERMS];
  	int32_t 	visable_term;	// Terminal being viewed
  	int32_t 	current_term;	// Terminal being scheduled
  	pcb *		terminals_pcb[NUM_TERMS];	// Addresses for the PCBs
  	uint16_t	ss0[NUM_TERMS];	// Store terminal TSS values
  	uint16_t	esp0[NUM_TERMS];	// Store terminal TSS values
  	uint8_t		is_on[NUM_TERMS];
  	uint8_t		exe_bytes[SIZEOFOPERATIONSTABLE];
}scheduler_t;

void set_pid(uint32_t pid);
void init_global_scheduler();
void switch_terminals(int32_t next_terminal);
int get_current_noc();
int32_t get_current_terminal();
int32_t get_visable_terminal();
void set_line_buffer(char linebuffer[128]);
void set_global_buffer(char linebuffer[128],int numberofchars);
void scheduler_next();
void save_esp_ebp(int32_t esp,int32_t ebp);
uint32_t get_current_pid();
int32_t get_global_screen_y();
int32_t get_global_screen_x();
void set_global_screen_x(uint32_t x);
void set_global_screen_y(uint32_t y);

#endif
