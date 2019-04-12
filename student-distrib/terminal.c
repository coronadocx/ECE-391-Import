/*.c file for the terminal */

#include "keyboard.h"
#include "terminal.h"
#include "lib.h"
 char terminalbuffer[KEYBOARD_BUFFER_LENGTH];
volatile int flag=0;
volatile int numberofchars=0;

/* int init()
 *
 * Initializes the terminalbuffer
 * Inputs: None
 * Outputs: 0 always
 * Side Effects: None
 */


int init(){
  memset(terminalbuffer,0,KEYBOARD_BUFFER_LENGTH);

  return 0;
}

/* void enable_cursor()
 *
 * Enables the cursor on the screen
 * Inputs: None
 * Outputs: None
 * Side Effects: Initializes the cursor on the screen
 */

void enable_cursor(){
  /*referenced wiki.osdev.org/Text_Mode_cursor*/
  outb(REG_A,VMEMPORT_3D4);
  outb((inb(VMEMPORT_3D5)&SELECT_REG_C)|CURSOR_START,VMEMPORT_3D5);
  outb(SELECT_REG_B,VMEMPORT_3D4);
  outb((inb(VMEMPORT_3D5)&REG_E)|CURSOR_END,VMEMPORT_3D5);
}

/* void update_cursor()
 *
 * updates the cursor on the screen to the new location
 * Inputs: None
 * Outputs: None
 * Side Effects: Changes the location of the cursor on the screen
 */

void update_cursor(){
  /*referenced wiki.osdev.org/Text_Mode_cursor*/
  int x= getpositionx();
  int y= getpositiony();
  int pos= y*X_WIDTH+x;
  outb(REG_F,VMEMPORT_3D4);
  outb((uint8_t) (pos &MASK_LOWER_8 ),VMEMPORT_3D5);
  outb(REG_E,VMEMPORT_3D4);
  pos=pos>>GET_UPPER_8;
  outb((uint8_t) (pos & MASK_LOWER_8),VMEMPORT_3D5);

}

/* int write()
 *
 * Writes inputs from the terminalbuffer onto the screen.
 * Inputs: None
 * Outputs: Returns 0
 * Side Effects: Prints characters from the terminalbuffer to the screen
 */


int terminal_write(int32_t fd,void*buf,int32_t nbytes){
  int i=0;
  int8_t* buf2= (int8_t*)buf;

  for(i=0;i<nbytes;i++){
    putc(buf2[i]);
  }
  
  update_cursor();
  return 0;
}

/* int read
 * Writes characters from the buffer to the terminal buffer and appends a
 *  new line character at the end. Prints the number of bytes which were read
 * Inputs: char* buffer
 * Outputs: 0 always
 * Side Effects: None
 */
int terminal_read(int32_t fd,void* buffer,int32_t nbytes){


  int sizetocopy;
  memset(buffer,0,nbytes);
  while(!flag){

  }
  if(nbytes<numberofchars){
    sizetocopy=nbytes;
  }
  else{
    sizetocopy=numberofchars;
  }
    //memset(linebuffer,0,KEYBOARD_BUFFER_LENGTH);
  int i=0;
  while(i<numberofchars){
    *((int8_t*)buffer+i)=terminalbuffer[i];
     i=i+1;
  }
  *((int8_t*)buffer+i)='\n';
  putc('\n');
  update_cursor();
  int retvalue = numberofchars;
  memset(terminalbuffer,0,KEYBOARD_BUFFER_LENGTH);
  numberofchars=0;
  flag=0;

  return retvalue+1;
}

/* terminal_open
 *
 * Does nothing
 * Inputs: None
 * Outputs: Returns 0
 * Side Effects: None
 */
int terminal_open(const uint8_t* filename){
  return 0;
}


/*
 *  terminal_close
 *  INPUT: fd
 *  OUTPUT: none
 *  RETURN VALUE: 0 always
 *  SIDE EFFECT: none
 */
int32_t terminal_close(int32_t fd){
  return 0;
}


/*
 *  set_terminal_buffer
 *  INPUT:  buf     - destination buffer
 *          nchars  - number of characters to copy
 *  OUTPUT: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: Helper function which copies contents
 *                of buf into the terminal buffer.
 */
void set_terminal_buffer(uint8_t* buf,uint32_t nchars){
  int i=0;
  memset(terminalbuffer,0,KEYBOARD_BUFFER_LENGTH);
  while(i<nchars){
    terminalbuffer[i]=*(buf+i);
    i=i+1;
  }
  numberofchars=nchars;
  terminalbuffer[numberofchars]='\n';
  flag=1;

}
