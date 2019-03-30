
#include "linkage.h"
#include "sys_call.h"
#include "filesystem.h"
#include "pcb.h"]


static int8_t args[126];  // 128 characters in keyboard buffer. 1 for \n and 1 for enter  filename can take
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

int8_t check_buf[40];

if( read_data(dir_entry.inode_num,0,(uint8_t*) check_buf,40)==-1){
  return -1;
}
if(check_buf[0]!=0x7f || check_buf[1]!=0x45||check_buf[2]!=0x4C||check_buf[3]!=0x46){
  return -1 ;
}







}

int32_t halt(uint8_t status){

}
