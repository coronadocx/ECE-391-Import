#include "scheduler.h"
#include "lib.h"
#include "paging.h"

static scheduler_t global_scheduler;

/* 
 *  init_global_scheduler
 *  INPUT:  none
 *  OUTPUT: none
 *  RETURN: none
 *  EFFECT: creates and populates global_scheduler object
 */
void init_global_scheduler()
{
  uint32_t i;

  global_scheduler.current_term = 0;
  global_scheduler.visable_term = 0;
  for (i=0; i<NUM_TERMS; i++) {
    global_scheduler.terminals[i].noc = 0;
    global_scheduler.vid_bufs[i] = (uint8_t*)(T1_BUF_ADDR + VMEM_SIZE*i);
  }
}

/*
 *  switch_terminals
 *  INPUT:  next_terminal
 *  OUTPUT: none
 *  RETURN: none
 *  EFFECT: switches which terminal is being viewed by the user
 *  SIDE EFFECT: modifies video memory and terminal video buffers
 */
void switch_terminals(int32_t next_terminal)
{
  cli();
  int32_t  prev_terminal;
  uint8_t* prev_terminal_buf;
  uint8_t* next_terminal_buf; 
  uint8_t* vmem;


  // Get previous terminal and its buffer
  prev_terminal = global_scheduler.visable_term;
  prev_terminal_buf = global_scheduler.vid_bufs[prev_terminal];

  if (next_terminal == prev_terminal)
    return;

  // Set next terminal and get its buffer
  global_scheduler.visable_term = next_terminal;
  next_terminal_buf = global_scheduler.vid_bufs[next_terminal];

  vmem = (uint8_t*)VID_START_ADDR;

  // // cli();
  paging_set_write_to_videomem();
  memcpy(prev_terminal_buf, vmem, VMEM_SIZE);   // Savem vmem to previous terminal buffer
  memcpy(vmem, next_terminal_buf, VMEM_SIZE);   // Write next terminal buffer to vmem
  paging_set_write_to_buffer(global_scheduler.current_term);
  sti();

  // paging_set_write_to_videomem();
  // // If current terminal is being viewed
  // if (global_scheduler.current_term == global_scheduler.visable_term){
  //   // Have virtural map to video memory
  //   paging_set_write_to_videomem();
  // }
  // else{
  //   // Have virtural map to buffer
  //   paging_set_write_to_buffer(global_scheduler.current_term);
  // }
  // // sti();

  // if (global_scheduler.visable_term == 0)
  //   paging_set_write_to_videomem();
  // else
  //   paging_set_write_to_buffer(global_scheduler.visable_term);

  // return; 
}


/*
 *  get_current_noc
 *  INPUT:  none
 *  OUTPUT: none
 *  RETURN: the noc value of the current terminal
 *  EFFECT: helper, gets the noc value of the current terminal
 */
int32_t get_current_noc()
{
  return global_scheduler.terminals[global_scheduler.visable_term].noc;
}


/*
 *  get_current_terminal()
 *  INPUT:  none
 *  OUTPUT: none
 *  RETURN: none
 *  EFFECT: helper, gets the value for the current terminal
 */
int32_t get_current_terminal()
{
  return global_scheduler.visable_term;
}

/*
 *  set_line_buffer
 *  INPUT:  linebuffer - TODO 
 *  OUTPUT: TODO
 *  RETURN: none
 *  EFFECT: TODO
 */
void set_line_buffer(char linebuffer[128])
{
  memcpy(linebuffer,global_scheduler.terminals[global_scheduler.visable_term].lb,global_scheduler.terminals[global_scheduler.visable_term].noc);
}

/*
 *  set_global_buffer
 *  INPUT:  TODO
 *  OUTPUT: TODO 
 *  RETURN: none  
 *  EFFECT: TODO
 */
void set_global_buffer(char linebuffer[128],int numberofchars)
{
  global_scheduler.terminals[global_scheduler.visable_term].noc=numberofchars;
  memcpy(global_scheduler.terminals[global_scheduler.visable_term].lb,linebuffer,numberofchars);
}

/*
 *  scheduler_next
 *  INPUT:  none
 *  OUTPUT: none
 *  RETURN: none
 *  EFFECT: Increments the counter which keeps track of which terminal is 
 *          being processed. Then, depending on which terminal is being viewed, 
 *          may change paging structure to show or hide accordingly
 */
void scheduler_next()
{
  global_scheduler.current_term = (global_scheduler.current_term+1)%3;  // Set next terminal

  // If current terminal is being viewed
  if (global_scheduler.current_term == global_scheduler.visable_term){
    // Have virtural map to video memory
    paging_set_write_to_videomem();
  }
  else{
    // Have virtural map to buffer
    paging_set_write_to_buffer(global_scheduler.current_term);
  }
}
