#ifndef SYS_CALL_H
#define SYS_CALL_H






typedef struct operation_table{
 int32_t (*open)(int8_t* filename);
 int32_t (*read)(int32_t fd,void* buf, int32_t nbytes);
 int32_t (*write)(int32_t fd,const void* buf, int32_t nbytes);
 int32_t (*close)(int32_t fd);

}otable_t;


/* struct for a file descriptor */

typedef struct file_descriptor{
  otable_t* operationstable;
  int32_t inode_num;
  int32_t file_pos;
  int32_t flags[2];
}file_desc;

int32_t halt(uint8_t status);
int32_t execute(const uint8_t* command);
int32_t write(int32_t fd, const void*buf,int32_t nbytes);
int32_t read(int32_t fd,void*buf,int32_t nbytes);
int32_t close(int32_t fd);
int32_t open(const uint8_t* filename);
#endif
