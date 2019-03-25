/* c file for keyboard */
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include"terminal.h"
static char chararray[NUM_KEYS]={' ','e','1','2','3','4','5','6','7','8','9','0','-','=','b','t','q','w','e','r','t','y','u','i','o','p','[',']',
'\n','0','a','s','d','f','g','h','j','k','l',';','\'','`','s','\\','z','x','c','v','b','n','m',',','.','/','r','\0','\0',' '};
static char shiftarray[NUMBERSONKEYBOARD]={'~','!','@','#','$','%','^','&','*','('};  // handling if shift is pressed on any num keys on qwerty keyboard
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


/* 1 is used to indicate the shift,capslock, control are pressed. 0 in the chararray indicates that they are not pressed or released */
void check_input(){

 uint32_t a;
 a=inb(KEYBOARD_CMD_PORT);  // read from the keyboard port
 switch(a){
   case  LEFTSHIFT: chararray[ LEFTSHIFT]='1';break;    // check if left shift is pressed
   case  LEFTSHIFTRELEASED :chararray[ LEFTSHIFT]='0';break; // check if left shift is released
   case RIGHTSHIFT:chararray[RIGHTSHIFT]='1';break;  // check if right shift is pressed
   case RIGHTSHIFTRELEASED:chararray[RIGHTSHIFT]='0';break;  // check if right shift is released
   case LEFTCONTROLPRESSED:chararray[LEFTCONTROLPRESSED]='1';break;  // check if control is pressed
   case LEFTCONTROLRELEASED:chararray[LEFTCONTROLPRESSED]='0';break; // check is control is released
   case BACKSPACE:{  // check for backspace
                    handlebackspace();
                    linebuffer[numberofchars]='\0';
                    if(numberofchars!=0) // make sure characters have been written
                    numberofchars=numberofchars-1; // backspace removes the number of chars
                    break;
                  }
   case ENTER:    {  // adding newline to buffer. This triggers a terminal read
                    putc(chararray[a]);
                    linebuffer[numberofchars]='\n';
                    read(linebuffer); // call to terminal read.
                    memset(linebuffer,0,KEYBOARD_BUFFER_LENGTH); // reset the linebuffer
                    numberofchars=0; // reset the number of chars read
                    break;
                  }
   case CAPSLOCK:{
                    if(chararray[CAPSLOCK]=='0')
                    {  // check if capslock had been pressed. basic toggling functionality
                      chararray[CAPSLOCK]='1';
                    }
                    else {
                      chararray[CAPSLOCK]='0';  // switch off capslock
                    }
                  }
                   break;
                /* deciding how to put the character on the screen */
   default:{
            if( ( chararray[CAPSLOCK]=='1'|| chararray[ LEFTSHIFT]=='1'||chararray[RIGHTSHIFT]=='1') && chararray[a]>=ASCIILOWERCASEA && chararray[a]<=ASCIILOWERCASEZ)
              {

                char temp = chararray[a]-32;
                if(temp=='L' && chararray[LEFTCONTROLPRESSED]=='1' ){
                  clear();
                  setposition(0,0);  // clear the screen and set the x y position to 0,0
                    update_cursor(); // update the position of the cursor
                    numberofchars=0; // reset number of chars in keyboard buffer
                    memset(linebuffer,0,KEYBOARD_BUFFER_LENGTH);

                }
                else{
                if(numberofchars!=KEYBOARD_BUFFER_LENGTH-1){// -1 because last character is reserved for \n
                linebuffer[numberofchars]=temp;
                putc(temp);
                  update_cursor(); // update the position of the cursor
                numberofchars=numberofchars+1; // add number of chars in keyboard buffer
              }
              }
                }
            else if(chararray[LEFTSHIFT]=='1' || chararray[RIGHTSHIFT]=='1')
            {
                char temp;
                if(chararray[a]&& numberofchars!=KEYBOARD_BUFFER_LENGTH-1){ // -1 because last character is reserved for \n
                  if(chararray[a]>=ASCII1 && chararray[a]<=ASCII9){ // if the shift is for numbers 1 to 9
                    int x = chararray[a]-'0';
                    if(shiftarray[x]){
                      temp=shiftarray[x];
                    }
                  }
                  else{ // handle shift for all other special characters
                    switch(chararray[a]){
                      case '-': temp='_';break;
                      case '=':temp='+';break;
                      case '[':temp='{';break;
                      case ']':temp='}';break;
                      case '`':temp='~';break;
                      case '\\':temp='|';break;
                      case ';':temp=':';break;
                      case '\'':temp='\"';break;
                      case ',':temp='<';break;
                      case '.':temp='>';break;
                      case '/':temp='?';break;
                      case '0':temp=')';break;
                      default:break;
                    }
                  }
                  linebuffer[numberofchars]=temp;
                  putc(temp);
                  numberofchars++;
                    update_cursor();  // update the position of the cursor

                }

            }
            else
              {
                if(chararray[a]&& numberofchars!=KEYBOARD_BUFFER_LENGTH-1)// -1 because last character is reserved for \n
                {
                linebuffer[numberofchars]=chararray[a];
                putc(chararray[a]);
                numberofchars++;
                  update_cursor(); // update the position of the cursor
                  }

              }

               break;
           }
}
 send_eoi(1); // send the end of intrupt signal for keyboard interupt which is at irq 1

}


/*
 * init_keyboard
 *   DESCRIPTION: Function which initializes the Keyboard by enabling the first IRQ
 *   OUTPUTS:None
 *   RETURN VALUE: none
 *   SIDE EFFECTS: IRQ 1 is enabled and initializes the keycodes for the keyboard.
 */

void init_keyboard(){
  enable_irq(1); // enable interupts for irq1

// The following Initializes the Numbers in the chararray
// Each index into the char array (ie: HEX value) refers to scancodes for the Keyboard
// This is a simpler method because we only needed the numbers and lower case
// letters for the demo
chararray[CAPSLOCK]='0';  // intially capslock is off
chararray[ LEFTSHIFT]='0'; // initially left shift is not pressed. 


}
