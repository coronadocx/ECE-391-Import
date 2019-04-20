#include "scheduler.h"
#include "lib.h"
#include "paging.h"

static scheduler global_scheduler;


void init_global_scheduler(int current_terminal){
  global_scheduler.curr_t=current_terminal;
  global_scheduler.terminals[0].noc=0;
  global_scheduler.terminals[1].noc=0;
  global_scheduler.terminals[2].noc=0;
  global_scheduler.vidmaps[0].start_address=(uint32_t**)T1_BUF;
  global_scheduler.vidmaps[1].start_address=(uint32_t**)T2_BUF;
  global_scheduler.vidmaps[2].start_address=(uint32_t**)T3_BUF;

}

void switch_terminals(int next_terminal){

  // get the current terminal index and corresponding memory addresses for terminals
  int current_terminal = global_scheduler.curr_t;
  uint32_t** current_terminal_buf = global_scheduler.vidmaps[current_terminal].start_address;
  uint32_t** next_terminal_buf = global_scheduler.vidmaps[next_terminal].start_address;


  global_scheduler.curr_t=next_terminal;

  uint32_t** vmem =(uint32_t**) VID_START_ADDR;

  // Save the current video memory to the current terminal buffer
  // and copy over the new/next terminal buffer to videomemory
  memcpy(current_terminal_buf,vmem,VMEM_SIZE);
  memcpy(vmem,next_terminal_buf,VMEM_SIZE);



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
