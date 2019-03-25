/* c file for keyboard */
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include"terminal.h"
static char chararray[NUM_KEYS]={' ','e','1','2','3','4','5','6','7','8','9','0','-','=','b','t','q','w','e','r','t','y','u','i','o','p','[',']',
'\n','0','a','s','d','f','g','h','j','k','l',';','\'','`','s','\\','z','x','c','v','b','n','m',',','.','/','r','\0','\0',' '};
static char linebuffer[KEYBOARD_BUFFER_LENGTH];
int numberofchars=0;



 /*
  * check_input
  *   DESCRIPTION: Function that handles keyboard inputs .
  *   INPUTS: None
  *   OUTPUTS:None
  *   RETURN VALUE: None
  *   SIDE EFFECTS:  populates the keyboard buffer and performs actions based on keyboard inputs
  */

void check_input(){

 uint32_t a;
 a=inb(KEYBOARD_CMD_PORT);
 switch(a){
   case  LEFTSHIFT: chararray[ LEFTSHIFT]='1';break;
   case  LEFTSHIFTRELEASED :chararray[ LEFTSHIFT]='0';break;
   case RIGHTSHIFT:chararray[RIGHTSHIFT]='1';break;
   case RIGHTSHIFTRELEASED:chararray[RIGHTSHIFT]='0';break;
   case LEFTCONTROLPRESSED:chararray[LEFTCONTROLPRESSED]='1';break;
   case LEFTCONTROLRELEASED:chararray[LEFTCONTROLPRESSED]='0';break;
   case BACKSPACE:{
     handlebackspace();
     linebuffer[numberofchars]='\0';
   if(numberofchars!=0)
    numberofchars=numberofchars-1;

   break;
 }
   case ENTER: {
     putc(chararray[a]);
     linebuffer[numberofchars]='\n';
     read(linebuffer);
     memset(linebuffer,0,KEYBOARD_BUFFER_LENGTH);
     numberofchars=0;


     break;
   }
   case CAPSLOCK: if(chararray[CAPSLOCK]=='0'){
                    chararray[CAPSLOCK]='1';
                  }
              else {
                  chararray[CAPSLOCK]='0';
                }break;
   default:{ if( ( chararray[CAPSLOCK]=='1'|| chararray[ LEFTSHIFT]=='1'||chararray[RIGHTSHIFT]=='1') && chararray[a]>=ASCIILOWERCASEA && chararray[a]<=ASCIILOWERCASEZ)
              {

                char temp = chararray[a]-32;
                if(temp=='L' && chararray[LEFTCONTROLPRESSED]=='1' ){
                  clear();
                  setposition(0,0);
                    update_cursor();
                    numberofchars=0;
                    memset(linebuffer,0,KEYBOARD_BUFFER_LENGTH);

                }
                else{
                if(numberofchars!=KEYBOARD_BUFFER_LENGTH-1){
                linebuffer[numberofchars]=temp;
                putc(temp);
                  update_cursor();
                numberofchars=numberofchars+1;
              }
              }
                }
            else
              {
                if(chararray[a]&& numberofchars!=KEYBOARD_BUFFER_LENGTH-1)
                {
                linebuffer[numberofchars]=chararray[a];
                putc(chararray[a]);
                numberofchars++;
                  update_cursor();
                  }

              }

               break;
           }
}
 send_eoi(1);

}


/*
 * init_keyboard
 *   DESCRIPTION: Function which initializes the Keyboard by enabling the first IRQ
 *   OUTPUTS:None
 *   RETURN VALUE: none
 *   SIDE EFFECTS: IRQ 1 is enabled and initializes the keycodes for the keyboard.
 */

void init_keyboard(){
  enable_irq(1);

// The following Initializes the Numbers in the chararray
// Each index into the char array (ie: HEX value) refers to scancodes for the Keyboard
// This is a simpler method because we only needed the numbers and lower case
// letters for the demo
chararray[CAPSLOCK]='0';
chararray[ LEFTSHIFT]='0';


}
