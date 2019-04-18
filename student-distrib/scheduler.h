#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct terminal_struct{
char lb[128];
int noc;
}terminal;

typedef struct videomaps{
 int start_address;
}vid_map;

typedef struct sched{
  terminal terminals[3];
  vid_map  vidmaps[3];
  int curr_t;
}scheduler;


void init_global_scheduler(int current_terminal);
void switch_terminals(int next_terminal);
int get_current_noc();
int get_current_terminal();
void set_line_buffer(char linebuffer[128]);
void set_global_buffer(char linebuffer[128],int numberofchars);
#endif
