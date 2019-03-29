
#include "linkage.h"
#include "sys_call.h"
#include "pcb.h"

int32_t open(const uint8_t* filename){
  dentry_t a;
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

for(i=0;command[i]!=' ' && command[i]!='\n' && command[i]!='\0';i++){

}
}
int32_t halt(uint8_t status){

}
