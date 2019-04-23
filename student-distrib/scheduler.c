#include "scheduler.h"
#include "lib.h"
#include "paging.h"
#include "pcb.h"
#include "sys_call.h"
#include "filesystem.h"
#include "x86_desc.h"
#include "linkage.h"

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
  dentry_t dir_entry;

  // uint8_t exe_bytes[SIZEOFOPERATIONSTABLE];
  int8_t exe_name[] = "shell";

  // Read Data
  (void)read_dentry_by_name(exe_name, &dir_entry);

  // Set bytes 24-27
  (void)read_data(dir_entry.inode_num, STARTOFEIPINEXECUTABLE, global_scheduler.exe_bytes, SIZEOFOPERATIONSTABLE);

  // Create PCBs for the 3 initial shells, set up paging for first terminal  
  for (i=0; i<NUM_TERMS; i++) {
    // Set up PCBs
    // processes_running[i] = 1; // Mark process as running
    global_scheduler.terminals_pcb[i] = (pcb*)(END_KMEM - ((i+2)*PCB_SIZE)); // Grab PCB addr FIXME
    global_scheduler.terminals_pcb[i]->parent = NULL;   // Starting process has no parent
    global_scheduler.terminals_pcb[i]->process_id = (i+1);  // Process IDs are 1 indexed
    global_scheduler.ss0[i] = KERNEL_DS;  // All datasegments are the same
    global_scheduler.esp0[i] = (END_KMEM - ((i+1)*KERNEL_MEM_SIZE));
    set_up_stdin(global_scheduler.terminals_pcb[i]);
    set_up_stdout(global_scheduler.terminals_pcb[i]);
    set_up_fdsandargs(global_scheduler.terminals_pcb[i], 0, 0, exe_name);
    paging_change_process(i+1);
    (void)read_data(dir_entry.inode_num,0,(uint8_t*) VIRTUALADDRESSFOREXECUTABLEDATA,EXTENDEDPAGESIZE);


    // Initialize rest of scheduler object
    global_scheduler.terminals[i].noc = 0;
    global_scheduler.vid_bufs[i] = (uint8_t*)(T1_BUF_ADDR + VMEM_SIZE*i);
    global_scheduler.is_on[i] = 0;
  }

  paging_change_process(1); // Make paging look at shell 1 initially

  global_scheduler.current_term = 2;
  global_scheduler.visable_term = 0;

  return;
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
  if (global_scheduler.current_term != global_scheduler.visable_term)
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
  return global_scheduler.current_term;
}

/*
 * TODO
 */
int32_t get_visable_terminal()
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
  cli();
  uint32_t pid;

  global_scheduler.current_term = (global_scheduler.current_term+1)%3;  // Set next terminal
  pid = global_scheduler.terminals_pcb[global_scheduler.current_term]->process_id;

  // If current terminal is being viewed
  if (global_scheduler.current_term == global_scheduler.visable_term){
    // Have virtural map to video memory
    paging_set_write_to_videomem();
  }
  else{
    // Have virtural map to buffer
    paging_set_write_to_buffer(global_scheduler.current_term);
  }

  if (global_scheduler.is_on[global_scheduler.current_term] == 0){
    global_scheduler.is_on[global_scheduler.current_term] = 1; // Set init off
    tss.ss0 = KERNEL_DS;
    tss.esp0 = global_scheduler.esp0[global_scheduler.current_term];
    sti();
    (void)contextswitchasm(*(uint32_t*)global_scheduler.exe_bytes, global_scheduler.terminals_pcb[global_scheduler.current_term]);
    
  }

  else{
    // Switch to next process
    paging_change_process(pid);
    tss.ss0 = KERNEL_DS;
    tss.esp0 = (END_KMEM - (pid*KERNEL_MEM_SIZE));

    sti();
    (void)restoreparent(
      global_scheduler.terminals[global_scheduler.current_term].esp,
      global_scheduler.terminals[global_scheduler.current_term].ebp,
      0
      );
  }
}

//helper function
/*
 * save_esp_ebp
 *   DESCRIPTION: Saves the registers %esp and %ebp before running a new process
 *   INPUTS: pcb* a -> Address of the pcb structure
 *          int32_t esp -> esp value
 *          int32_t ebp -> ebp value
 *   OUTPUTS:None
 *   RETURN VALUE: None
 *   SIDE EFFECTS: Saves the %ebp and %esp registers in the pcb struct
 */

void save_esp_ebp(int32_t esp,int32_t ebp){
  global_scheduler.terminals[global_scheduler.current_term].esp=esp;
  global_scheduler.terminals[global_scheduler.current_term].ebp=ebp;
  return;
}