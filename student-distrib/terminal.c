/*.c file for the terminal */

#include "keyboard.h"
#include "terminal.h"
#include "lib.h"
static char terminalbuffer[KEYBOARD_BUFFER_LENGTH];


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
  for(i=0;buf2[i]!='\0';i++){
    putc(buf2[i]);
  }
  update_cursor();
  return 0;
}

/* int read(char* buffer)
 *
 * Writes characters from the buffer to the terminal buffer and appends a
 *  new line character at the end. Prints the number of bytes which were read
 * Inputs: char* buffer
 * Outputs: 0 always
 * Side Effects: None
 */


int terminal_read(int32_t fd,void* buffer,int32_t nbytes){
  int j;
    int8_t* buf2= (int8_t*)buffer;
    int newlineflag=return_newline_flag();
    char* linebuffer = returnkeyboardbufferaddress();
      int sizetocopy;
  while(!newlineflag){
    newlineflag=return_newline_flag();
    linebuffer =returnkeyboardbufferaddress();

    if(nbytes<return_numberofchars()){
      sizetocopy=nbytes;
    }
    else{
      sizetocopy=return_numberofchars();
    }
    j=0;
    while(j<sizetocopy){
      (buf2[j])= *(linebuffer+j);
      j=j+1;
    }

  }
    //memset(linebuffer,0,KEYBOARD_BUFFER_LENGTH);
  set_newline_flag(0);
  newlineflag=0;
  buf2[j++]='\n';
  if(sizetocopy==1){
  printf("%d byte was read\n",sizetocopy);
}
else {
    printf("%d bytes were read\n",sizetocopy);
}
  terminal_write(1,buf2,j);
  update_cursor();

  return 0;
}

/* int open()
 *
 * Does nothing
 * Inputs: None
 * Outputs: Returns 0
 * Side Effects: None
 */


int terminal_open(const uint8_t* filename){
return 0;
}

int32_t terminal_close(int32_t fd){
  return 0;
}
