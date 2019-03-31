#ifndef PCB_H
#define PCB_H

#include "sys_call.h"








#define FD_ARR_SIZE       8

/* Function to get the address of the current pcb */
extern pcb* get_pcb_address();


otable_t rtctable={rtc_open,rtc_read,rtc_write,rtc_close};
otable_t filetable={fs_open,fs_red,fs_write,fs_close};
otable_t directorytable={dir_open,dir_read,dir_write,dir_close};
otable_t stdin_table={terminal_open,terminal_read,NULL,terminal_close};
otable_t stdout_table={terminal_open,NULL,terminal_write,terminal_close};



typedef struct task_struct{
  file_desc fd_array[FD_ARR_SIZE];
  pcb* parent;
  int32_t parent_esp;
  int32_t parent_ebp;

}pcb;




#endif
