
#include "linkage.h"
#include "sys_call.h"
#include "filesystem.h"
#include "pcb.h"
#include "x86_desc.h"
#include "paging.h"

extern void contextswitchasm(uint32_t eipval,pcb* current_process);


static int8_t args[126];  // 128 characters in keyboard buffer. 1 for \n and 1 for enter  filename can take
int8_t processes_running[2] = {0,0}
int32_t open(const uint8_t* filename){

  if(filename == NULL){
    return -1;
  }
  /* Directory Entry struct */
  dentry_t dir_entry;

 /* Error handling if file not found */
  if(read_dentry_by_name(filename,&dir_entry)==-1){
    return -1;
  }

  int file_type;
  int inode_num;

/* Copy over filetype and inode number */

  file_type = dir_entry.file_type;
  inode_num = dir_entry.inode_num;


/* Here, we try to calculate which PCB this system call belongs to
 * We do this by taking the Top of Stack Pointer and ANDing it with a
   bitmask to get the nearest 8KB aligned page
   */

  int x;
  pcb* current_pcb;
  int curr_pid;

/* Bit-Mask 0xFFE000 should give us an 8KB aligned address of the PCB */
  curr_pcb = ((pcb*)&x) & (PCB_MASK);
  curr_pid = (int) ((END_KMEM) - curr_pcb)/PCB_SIZE;

 /* Process IDs start at 0 and go till 5 */
  curr_pid--;

/* If file descriptor array of the PCB has no free index, return -1 */
  if(curr_pcb->current_index == -1)
    return -1;

/* Should continue writing this */
  curr_pcb->fd_array[(curr_pcb->current_index)]


}

int32_t close(int32_t fd){


}
int32_t read(int32_t fd,void*buf,int32_t nbytes){

}
int32_t write(int32_t fd, const void*buf,int32_t nbytes){

}

int32_t execute(const uint8_t* command){
int i=0;
int8_t buf[32];
dentry_t a;

//leading spaces
while(command[i]==' '){
  i=i+1;
}

// parse the filename form the command
for(;command[i]!=' ' && command[i]!='\n' && command[i]!='\0';i++)
{
buf[i]=command[i];
}
i=i+1; // accounting for the space
for(;command[i]!='\n';i++){
args[i]=command[i];
}


// Check file_type
//Check executable
// Check Null
// Check Pointer
// Check process ID



// check if program exists

if(read_dentry_by_name(buf,&dir_entry)==-1){
  return -1 ;
}

/* checks if you can read data */
int8_t check_buf[40];
int8_t executablebytes[4];
if( read_data(dir_entry.inode_num,0,(uint8_t*) check_buf,40)==-1){
  return -1;
}

/* check if the file is an executable */
if(check_buf[0]!=0x7f || check_buf[1]!=0x45||check_buf[2]!=0x4C||check_buf[3]!=0x46){
  return -1 ;
}
/* set up paging */




/* set the bytes 24 to 27 */
executablebytes[0]=check_buf[23];
executablebytes[1]=check_buf[24];
executablebytes[2]=check_buf[25];
executablebytes[3]=check_buf[26];


/* create pcb and set parent pcb */
i=1;
uint32_t current_pid=-1;
while(i<3){
  if(processes_running[i-1]==0){
     current_pid=i;
     processes_running[i-1]=1;
     break;
  }
  i=i+1;
}
if(current_pid==-1){
  return -1;
}

pcb* parent_pcb;
int parent_pid;
int x;
paging_change_process(current_pid);
/*  read data into virtual address 128 MB */
read_data(dir_entry.inode_num,0,(uint8_t*) 0x800000,get_filesize(dir_entry.inode_num));
/* Bit-Mask 0xFFE000 should give us an 8KB aligned address of the PCB */
parent_pcb = ((pcb*)&x) & (PCB_MASK);
parent_pid = (int) ((END_KMEM) - parent_pcb)/PCB_SIZE;
/* this part is to create the pcb */
pcb* current_process=(pcb*) END_KMEM-(current_pid+1)*KERNEL_MEM_SIZE; // start at 8MB-2*8kb

if(current_pid!=1){
  current_process->process->parent =parent_pcb ;
}
else{
  current_process->process->parent=NULL
}

// SETS UP STD IN
current_process->fd_array[0]->operationtable=&stdin_table;
current_process->fd_array[0]->inode_num=-1;
current_process->fd_array[0]->file_pos=0;
current_process->fd_array[0]->flags[0]=1;

// SETS UP STD OUT
current_process->fd_array[1]->operationtable=&stdout_table;
current_process->fd_array[1]->inode_num=-1;
current_process->fd_array[1]->file_pos=0;
current_process->fd_array[1]->flags[0]=1;

i=2;
/* sets the other file descriptors to not be in use */
while(i<8){
current_process->fd_array[i]->file_pos=0;
current_process->fd_array[i]->flags[0]=0;
i=i+1;
}


/* this part alters the TSS */
tss_t.ss0 = KERNEL_DS;
tss_t.esp0 = END_KMEM - current_pid*KERNEL_MEM_SIZE; // This will be the processe's kernelmode stack

/* lets do the context switch!!!!!! */
contextswitchasm((uint32_t) executablebytes,current_process);




}

int32_t halt(uint8_t status){

}

//helper function
void save_parent_registers(pcb* a,int32_t esp,int32_t,ebp){
a->parent_esp=esp;
a->parent_ebp=ebp;
return ;
}
