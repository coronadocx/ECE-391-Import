#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "lib.h"

#define NUM_TERMS	3

typedef struct terminal_struct{
	int8_t		lb[128];
	int32_t 	noc;
}terminal_t;

typedef struct sched{
  	terminal_t 	terminals[NUM_TERMS];
  	uint8_t * 	vid_bufs[NUM_TERMS];
  	int32_t 	visable_term;	// Terminal being viewed
  	int32_t 	current_term;	// Terminal being scheduled
}scheduler_t;


void init_global_scheduler();
void switch_terminals(int32_t next_terminal);
int get_current_noc();
int get_current_terminal();
void set_line_buffer(char linebuffer[128]);
void set_global_buffer(char linebuffer[128],int numberofchars);
void scheduler_next();
int32_t get_visible_terminal();

#endif
