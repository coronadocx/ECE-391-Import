#ifndef PCB_H
#define PCB_H

#include "sys_call.h"

otable_t rtctable={rtc_open,rtc_read,rtc_write,rtc_close};
otable_t filetable={fs_open,fs_red,fs_write,fs_close};
otable_t directorytable={dir_open,dir_read,dir_write,dir_close};
otable_t stdin_table={terminal_open,terminal_read,NULL,terminal_close};
otable_t stdout_table={terminal_open,NULL,terminal_write,terminal_close};



typedef struct task_struct{
  file_desc fd_array[8];
  int32_t current_index;
  pcb* parent;

}pcb;




#endif
