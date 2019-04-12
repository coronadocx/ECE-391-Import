
#include "linkage.h"
#include "sys_call.h"
#include "filesystem.h"
#include "pcb.h"
#include "x86_desc.h"
#include "paging.h"
#include "types.h"
#include "rtc.h"
#include "lib.h"
// extern void contextswitchasm(uint32_t eipval,pcb* current_process);


//static int8_t args[126];  // 128 characters in keyboard buffer. 1 for \n and 1 for enter  filename can take
// otable_t rtctable,filetable,directorytable,stdin_table,stdout_table;
// rtctable.(*open)("hello") = 0;

/* Tables of function pointers */
void* rtctable[SIZEOFOPERATIONSTABLE]={&rtc_open,&rtc_read,&rtc_write,&rtc_close};
void* filetable[SIZEOFOPERATIONSTABLE]={&fs_open,&fs_read,&fs_write,&fs_close};
void* directorytable[SIZEOFOPERATIONSTABLE]={&dir_open,&dir_read,&dir_write,&dir_close};
void* stdin_table[SIZEOFOPERATIONSTABLE]={&terminal_open,&terminal_read,NULL,&terminal_close};
void* stdout_table[SIZEOFOPERATIONSTABLE]={&terminal_open,NULL,&terminal_write,&terminal_close};
int8_t processes_running[NUMBEROFPROCESSESSUPPORTED] = {NOTINUSE,NOTINUSE,NOTINUSE,NOTINUSE,NOTINUSE,NOTINUSE};


/*
 * open
 *   DESCRIPTION: open system call. opens a file in the fd array
 *   INPUTS: filename
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t fd -> file descriptor index of opened file
 *   SIDE EFFECTS:
 */

int32_t open(const uint8_t* filename){

  if(filename == NULL || strlen((int8_t*)filename)==0){
    return INVALIDORFAIL;
  }
  /* Directory Entry struct */
  dentry_t dir_entry;
 /* Error handling if file not found */
  if(read_dentry_by_name((int8_t*)filename,&dir_entry)==-1){
    return INVALIDORFAIL;
  }

  int file_type;
  int inode_num;

/* Copy over filetype and inode number */
  file_type = dir_entry.file_type;
  inode_num = dir_entry.inode_num;

  /* Get address of relevant PCB */
  pcb* curr_pcb;
  curr_pcb = get_pcb_address();



/* Traverse through the relevant file descriptor array */
  int i;
  /* i starts at 2 because 0 and 1 are occupied by stdin and stdout */
  for(i = 2; i < ENTRIESINFDARRAY; i++){

    /* Found an empty file descriptor spot */
    if(curr_pcb->fd_array[i].flags[IN_USE_INDEX] == NOTINUSE){
      break;
    }

  }

  /* If no file descriptor is free, return -1 */
  if(i >= ENTRIESINFDARRAY){
    return INVALIDORFAIL;
  }

  /* Populating the file descriptor struct */
  /* File operations table pointer */
  if(file_type == FILETYPEDEVICE){ // file type 0 is rtc device type
    curr_pcb->fd_array[i].operationstable = (rtctable);
  }
  else if(file_type == FILETYPEDIRECTORY){ // file type 1 is directoy
    curr_pcb->fd_array[i].operationstable = (directorytable);
  }
  else if(file_type == FILETYPEGENERAL){
    curr_pcb->fd_array[i].operationstable =  (filetable);
  }

  /* Inode */
  curr_pcb->fd_array[i].inode_num = inode_num;

  /* File position */
  curr_pcb->fd_array[i].file_pos = DEFAULTFILEOFFSET;

  /* Filling flags */
  curr_pcb->fd_array[i].flags[IN_USE_INDEX] = INUSE;
  curr_pcb->fd_array[i].flags[FTYPE_INDEX] = file_type;

// ASK TA HOW TO CALL THE FUNCTION
 int32_t (*fun_ptr)(const uint8_t*);
 fun_ptr = (curr_pcb->fd_array[i].operationstable)[FILE_OPS_OPEN];
 (*fun_ptr)(filename);
  //curr_pcb->fd_array[i].operationstable[FILE_OPS_OPEN](filename);

  return i;

}


/*
 * close
 *   DESCRIPTION: close  system call. tearsdown stack and closes the file in the fd array
 *   INPUTS: fd -> FD index to be closed
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure
 *   SIDE EFFECTS:
 */


int32_t close(int32_t fd){
  if(fd<0){
    return INVALIDORFAIL;
  }
  /* Check for valid fd */
    if(fd < FDSTART || fd > FDEND)
      return INVALIDORFAIL;


  /* Get address of relevant PCB */
    pcb* curr_pcb;
    curr_pcb = get_pcb_address();

    /* Check if file descriptor index not in use */
    if(curr_pcb->fd_array[fd].flags[IN_USE_INDEX] == NOTINUSE)
      return -1;
    /* Check if operationstable* is valid */
    if(curr_pcb->fd_array[fd].operationstable == NULL)
      return -1;
      /* Else call the close function from the operations table */

    int32_t (*fun_ptr)(int32_t);
    fun_ptr = (curr_pcb->fd_array[fd].operationstable)[FILE_OPS_CLOSE];
    (*fun_ptr)(fd);
    //if((curr_pcb->fd_array[fd].operationstable[FILE_OPS_CLOSE](fd)) == -1)
      //return -1;

    /* Populate and mark fd as not in use */
    curr_pcb->fd_array[fd].operationstable = NULL;
    curr_pcb->fd_array[fd].inode_num = 0;
    curr_pcb->fd_array[fd].file_pos = DEFAULTFILEOFFSET;
    curr_pcb->fd_array[fd].flags[IN_USE_INDEX] = NOTINUSE;
    curr_pcb->fd_array[fd].flags[FTYPE_INDEX] = 0;
    curr_pcb->fd_array[fd].flags[RSVD_INDEX_0] = 0;
    curr_pcb->fd_array[fd].flags[RSVD_INDEX_1] = 0;

    /* Return 0 on success */
    return 0;

}

/*
 * read
 *   DESCRIPTION: read system call. Calls the appropriate read function from file ops table
 *   INPUTS: fd -> File descriptor index
 *           buf -> buffer to read into
 *           nbytes -> number of bytes to read
 *   OUTPUTS:None
 *   RETURN VALUE: Returns the number of bytes read, or -1 on failure
 *   SIDE EFFECTS:
 */


int32_t read(int32_t fd,void*buf,int32_t nbytes){
  if(fd<0){
    return INVALIDORFAIL;
  }
  if(buf == NULL)
    return INVALIDORFAIL;

  if(fd == 1)
    return -1; // success ;

  /* Get address of relevant PCB */
  pcb* curr_pcb;
  curr_pcb = get_pcb_address();

  /* File needs to be in use for read */
  if(curr_pcb->fd_array[fd].flags[IN_USE_INDEX] == NOTINUSE)
    return INVALIDORFAIL;

  /* Function pointer points to read of file operations table of fd*/
  int32_t (*fun_ptr)(int32_t, void*, int32_t);
  fun_ptr = (curr_pcb->fd_array[fd].operationstable)[FILE_OPS_READ];


  int file_type;
	int filesize;
	int inode_num;
    /* For rtc_read and terminal read */
  if(file_type == FILETYPEDEVICE || fd == 0){
    return (*fun_ptr)(fd, buf, nbytes);
   }


   /* Get filesize from the inode */

	file_type = (int)curr_pcb->fd_array[fd].flags[FTYPE_INDEX];
  inode_num = curr_pcb->fd_array[fd].inode_num;
  filesize = get_filesize(inode_num);

  /* Beyond this point, we would only need to handle reads for files
	 * and directories
   */

	 /* If file position is greater than filesize, return 0 */
	if(file_type==FILETYPEGENERAL && curr_pcb->fd_array[fd].file_pos >= filesize)
	 return 0; // success ;

	/* Handles file and directory reads */
	int retval;
	retval = (*fun_ptr)(fd, buf, nbytes);

  return retval;

}


/*
 * write
 *   DESCRIPTION: Write system call. Calls the appropriate write function from file ops table
 *   INPUTS: fd -> File descriptor index
 *           buf -> buffer to write from
 *           nbytes -> number of bytes to write
 *   OUTPUTS:None
 *   RETURN VALUE: Returns 0 on success, -1 on failure
 *   SIDE EFFECTS:
 */

int32_t write(int32_t fd, const void*buf,int32_t nbytes){
    if(fd<0){
      return INVALIDORFAIL;
    }
	 if(buf == NULL)
    return INVALIDORFAIL;
   if(fd==0) // is that of stdin. stdin cant write !
      return INVALIDORFAIL;

	  /* Get address of relevant PCB */
  pcb* curr_pcb;
  curr_pcb = get_pcb_address();

  /* File needs to be in use for write */
  if(curr_pcb->fd_array[fd].flags[IN_USE_INDEX] == NOTINUSE)
    return INVALIDORFAIL;
	//int file_type = (int)curr_pcb->fd_array[fd].flags[FTYPE_INDEX];
  /* Function pointer points to read of file operations table of fd*/
  //if(fd==1 || file_type==0){
  int32_t (*fun_ptr)(int32_t, void*, int32_t);
  fun_ptr = (curr_pcb->fd_array[fd].operationstable)[FILE_OPS_WRITE];
   return (*fun_ptr)(fd, (void*) buf, nbytes);
  //}

//return 0;
}


/*
 * execute
 *   DESCRIPTION: execute system call. Starts a process and runs it
 *   INPUTS: command -> Executable to run
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure
 *   SIDE EFFECTS:
 */

int32_t execute(const uint8_t* command){
  int32_t i;
  int32_t j;
  int8_t buf[MAXBUFSIZEEXECUTE];
  dentry_t dir_entry;
  int32_t arg_size; // XXX tmp var to hold array size
  int32_t arg_idx;  // XXX tmp var to hold command starting idx
  int8_t arg_arr[MAXBUFSIZEEXECUTE]; // XXX tmp var to hold cmd
  if(command==NULL){
    return -1;
  }
  //Make sure buffer isn't too thicc
  if (strlen((int8_t*)command) > MAXBUFSIZEEXECUTE){
    printf("Bad Length\n" );
    return INVALIDORFAIL;
  }

  for (i=0; i<MAXBUFSIZEEXECUTE; i++)
    arg_arr[i] = '\0';

  // XXX save the command buffer locally copies the entire command
  arg_size = 0;
  while(command[arg_size] != '\n' && command[arg_size] != '\0') {
    arg_arr[arg_size] = command[arg_size];
    arg_size++;
  }

  arg_arr[arg_size+1] = '\0';



  i=0;


  //leading spaces
  while(command[i]==' '){
    i=i+1;
  }

  // parse the filename from the command
  for(j=0; command[i]!=' ' && command[i]!='\n' && command[i]!='\0';i++, j++){
    buf[j]=command[i];
  }

  // Set the last character to sentinal
  if (j == MAXBUFSIZEEXECUTE)
    j = MAXBUFSIZEEXECUTE-1;
  buf[j]='\0';
  arg_idx = j+1;

  // check if program exists
  if(read_dentry_by_name(buf,&dir_entry)==-1){
    return INVALIDORFAIL ;
  }

  /* checks if you can read data */
  int8_t check_buf[METADATAEXECUTESIZE];
  uint8_t executablebytes[SIZEOFOPERATIONSTABLE];
  if( read_data(dir_entry.inode_num,0,(uint8_t*) check_buf,METADATAEXECUTESIZE)==INVALIDORFAIL){
    return INVALIDORFAIL;
  }

  /* check if the file is an executable. these four magic numbers are used to check if a fine is an executable  */
  if(check_buf[0]!=0x7f || check_buf[1]!=0x45||check_buf[2]!=0x4C||check_buf[3]!=0x46){  // we use these magic numbers only once and hence they are not macros
    return INVALIDORFAIL ;
  }

  /* set the bytes 24 to 27 */
  read_data(dir_entry.inode_num, STARTOFEIPINEXECUTABLE, (uint8_t*) executablebytes,SIZEOFOPERATIONSTABLE);




  /* create pcb and set parent pcb */
  i=1;
  uint32_t current_pid=-1;
  while(i<(NUMBEROFPROCESSESSUPPORTED+1)){
    if(processes_running[i-1]==0){
       current_pid=i;
       processes_running[i-1]=INUSE;
       break;
    }
    i=i+1;
  }
  if(current_pid==-1){
    return INVALIDORFAIL;
  }

  pcb* parent_pcb;
  int parent_pid;
  int*y;
  int x;
  y=&x;
  paging_change_process(current_pid);
  /*  read data into virtual address 128 MB */
  read_data(dir_entry.inode_num,0,(uint8_t*) VIRTUALADDRESSFOREXECUTABLEDATA,EXTENDEDPAGESIZE);
  /* Bit-Mask 0xFFE000 should give us an 8KB aligned address of the PCB */
  parent_pcb = get_pcb_address();
  parent_pid = (END_KMEM - (unsigned int) parent_pcb)/(PCB_SIZE) - 1;
  /* this part is to create the pcb */
  pcb* current_process=(pcb*)((int)END_KMEM-(current_pid+1)*PCB_SIZE); // start at 8MB-2*8kb

  if(current_pid!=1){
    current_process->parent =parent_pcb ;
    current_process->parent_process_id=parent_pid;
  }
  else{
    current_process->parent=NULL;
  }

  // SETS UP STD IN
  current_process->fd_array[0].operationstable=stdin_table;
  current_process->fd_array[0].inode_num=INVALIDORFAIL;
  current_process->fd_array[0].file_pos=DEFAULTFILEOFFSET;
  current_process->fd_array[0].flags[0]=INUSE;

  // SETS UP STD OUT
  current_process->fd_array[1].operationstable=stdout_table;
  current_process->fd_array[1].inode_num=INVALIDORFAIL;
  current_process->fd_array[1].file_pos=DEFAULTFILEOFFSET;
  current_process->fd_array[1].flags[0]=INUSE;

  current_process->process_id=current_pid;

  i=2;
  /* sets the other file descriptors to not be in use */
  while(i<ENTRIESINFDARRAY){
  current_process->fd_array[i].file_pos=DEFAULTFILEOFFSET;
  current_process->fd_array[i].flags[0]=NOTINUSE;
  i=i+1;
  }

  // Clean the old buffer because we do not create new structures
  for (i=0; i< PCB_MAX_ARGS; i++) {
    current_process->arg_arr[i] = '\0';
  }

  // Store the rest of the command as args in the current process
  current_process->arg_size = arg_size;
  for (i=0, j=arg_idx; j < arg_size && arg_arr[j] != '\0'; i++,j++) { // W_SPACE ? it was zero anyways
    current_process->arg_arr[i] = arg_arr[j];
  }

  /* this part alters the TSS */
  tss.ss0 = KERNEL_DS;
  tss.esp0 = END_KMEM - (current_pid)*KERNEL_MEM_SIZE; // This will be the processe's kernelmode stack

  /* lets do the context switch!!!!!! */
  contextswitchasm(*((uint32_t*) executablebytes),current_process);



  return 0; // success ; // success
}


/*
 * halt
 *   DESCRIPTION: halt system call. tearsdown stack and restores to parent process
 *   INPUTS: status -> character
 *   OUTPUTS:None
 *   RETURN VALUE: int32_t indicating success or failure
 *   SIDE EFFECTS:
 */

int32_t halt(uint8_t status){
  /* get the address of the process to halt */
  pcb* curr_pcb;
  curr_pcb = get_pcb_address();
  processes_running[curr_pcb->process_id-1]=NOTINUSE;
  uint32_t parentid=curr_pcb->parent_process_id;
  int i=0;
  // close all the open files
  while(i<ENTRIESINFDARRAY){
    if(curr_pcb->fd_array[i].flags[0]==INUSE){
      close(i);
    }
    i=i+1;
  }
 // restore paging
  paging_change_process(parentid);
  tss.ss0=KERNEL_DS; // switch back to parent stack
  tss.esp0= END_KMEM - (parentid)*KERNEL_MEM_SIZE;

  return restoreparent(curr_pcb->parent_ebp,curr_pcb->parent_esp,(uint32_t) status); // assembly function to restor ebp and esp





}

//helper function
/*
 * save_parent_registers
 *   DESCRIPTION: Saves the parent registers %esp and %ebp before running a new process
 *   INPUTS: pcb* a -> Address of the pcb structure
 *          int32_t esp -> esp value
 *          int32_t ebp -> ebp value
 *   OUTPUTS:None
 *   RETURN VALUE: None
 *   SIDE EFFECTS: Saves the %ebp and %esp registers in the pcb struct
 */

void save_parent_registers(pcb* a,int32_t esp,int32_t ebp){
a->parent_esp=esp;
a->parent_ebp=ebp;
return ;
}

////////////////////////////////CP4/////////////////////////////////

/*
 *  getargs
 *  INPUT:  nbytes  - number of bytes/chars to read into the buffer
 *
 *  OUTPUT: buf     - buffer that args will be stored into
 *
 *  RETURN: 0   - success
 *          -1  - buffer is too small or noarg or NULL buffer
 *
 *  EFFECT: Reads the program's command line arguments into a buffer
 */
int32_t getargs(uint8_t * buf, int32_t nbytes)
{
    pcb * curr_pcb = get_pcb_address();
  // NOTE: need the last element in buf for sentinel
  if (buf == NULL || nbytes < 0 || (nbytes-1) < curr_pcb->arg_size)
    return INVALIDORFAIL;




  strncpy((int8_t*)buf, (int8_t*)curr_pcb->arg_arr, nbytes); // Copy args into buf
//  buf[nbytes] = '\0'; // Insert nullbyte sentinel

  return 0;
}
