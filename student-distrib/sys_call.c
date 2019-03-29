
#include "linkage.h"
#include "sys_call.h"
#include "filesystem.h"
#include "pcb.h"

int32_t open(const uint8_t* filename){
  dentry_t dir_entry;
if(read_dentry_by_name(filename,&a)==-1){
  return -1;
}

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
int8_t args[126]; // 128 characters in keyboard buffer. 1 for \n and 1 for enter  filename can take
dentry_t a;
while(command[i]==' '){
  i=i+1;
}
if(i>32){
  return -1;
}
// parse the filename form the command
for(;command[i]!=' ' && command[i]!='\n' && command[i]!='\0' && i<32;i++)
{
buf[i]=command[i];
}
i=i+1; // accounting for the space
for(;command[i]!='\n';i++){
args[i]=command[i];
}


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

uint32_t eip_jump = (uint32_t) check_buf[23] << 24 |  (uint32_t) check_buf[24] << 16 | (uint32_t) check_buf[25] << 8 | (uint32_t) check_buf[26];







}

int32_t halt(uint8_t status){

}
