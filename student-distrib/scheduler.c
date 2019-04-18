#include "scheduler.h"
#include "lib.h"

static scheduler global_scheduler;


void init_global_scheduler(int current_terminal){
  global_scheduler.curr_t=current_terminal;
  global_scheduler.terminals[0].noc=0;
  global_scheduler.terminals[1].noc=0;
  global_scheduler.terminals[2].noc=0;
  global_scheduler.terminals[3].noc=0;
  global_scheduler.vidmaps[0].start_address=0xB9000;
  global_scheduler.vidmaps[1].start_address=0xBA000;
  global_scheduler.vidmaps[2].start_address=0xBB000;

}

void switch_terminals(int next_terminal){
  global_scheduler.curr_t=next_terminal;
}
int get_current_noc(){
  return global_scheduler.terminals[global_scheduler.curr_t].noc;
}
int get_current_terminal(){
  return global_scheduler.curr_t;
}

void set_line_buffer(char linebuffer[128]){
  memcpy(linebuffer,global_scheduler.terminals[global_scheduler.curr_t].lb,global_scheduler.terminals[global_scheduler.curr_t].noc);
}
void set_global_buffer(char linebuffer[128],int numberofchars){
  global_scheduler.terminals[global_scheduler.curr_t].noc=numberofchars;
  memcpy(global_scheduler.terminals[global_scheduler.curr_t].lb,linebuffer,numberofchars);

}
