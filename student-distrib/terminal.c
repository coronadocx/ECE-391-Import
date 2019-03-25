/*.c file for the terminal */

#include "keyboard.h"
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
  outb(0x0A,0x3D4);
  outb((inb(0x3d5)&0xC0)|0,0x3D5);
  outb(0x0B,0x3D4);
  outb((inb(0x3D5)&0xE0)|15,0x3D5);
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
  int pos= y*80+x;
  outb(0x0F,0x3D4);
  outb((uint8_t) (pos &0xFF ),0x3D5);
  outb(0x0E,0x3D4);
  pos=pos>>8;
  outb((uint8_t) (pos & 0xFF),0x3D5);

}

/* int write()
 *
 * Writes inputs from the terminalbuffer onto the screen.
 * Inputs: None
 * Outputs: Returns 0
 * Side Effects: Prints characters from the terminalbuffer to the screen
 */


int write(){
  int i=0;
  for(i=0;terminalbuffer[i]!='\n';i++){
    putc(terminalbuffer[i]);
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


int read(char* buffer){
  int i=0;
  for(i=0;*(buffer+i)!='\n';i++){
    terminalbuffer[i]=*(buffer+i);
  }
  terminalbuffer[i]='\n';
  printf("%d bytes were read\n",i+1);
  write();
  putc('\n');
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


int open(){
return 0;
}
