/* c file for keyboard */
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include"terminal.h"
static char chararray[255]={' ','e','1','2','3','4','5','6','7','8','9','0','-','=','b','t','q','w','e','r','t','y','u','i','o','p','[',']',
'\n','0','a','s','d','f','g','h','j','k','l',';','\'','`','s','\\','z','x','c','v','b','n','m',',','.','/','r','\0','\0',' '};
static char linebuffer[128];
int numberofchars=0;



 /*
  * check_input
  *   DESCRIPTION: Function that checks the keyboard input and also Temporarily
  *               Checks the letters e and d to enable and disable the rtc for the demo.
                  This function primarily outputs the letter that was pressed on the keyboard.
  *   INPUTS: None
  *   OUTPUTS:None
  *   RETURN VALUE: None
  *   SIDE EFFECTS:  None. on press of e and d it enables and disables the rtc
  */

void check_input(){

 uint32_t a;
 // char b;
 a=inb(KEYBOARD_CMD_PORT);
 int x=getpositionx();
 int y=getpositiony()+1;
 if(x>=79){
   setposition(0,y);
 }
 if(y>25){
   scroll();
 }


 switch(a){
   case 0x2A: chararray[0x2A]='1';break;
   case 0xAA:chararray[0x2A]='0';break;
   case 0x36:chararray[0x36]='1';break;
   case 0xB6:chararray[0x36]='0';break;
   case 0x1D:chararray[0x1D]='1';break;
   case 0x9D:chararray[0x1D]='0';break;
   case 0x0E:{
     handlebackspace();
     linebuffer[numberofchars]='\0';
   if(numberofchars!=0)
    numberofchars=numberofchars-1;

   break;
 }
   case 0x1C: {
     putc(chararray[a]);
     linebuffer[numberofchars]='\n';
     read(linebuffer);
     memset(linebuffer,0,128);
     numberofchars=0;


     break;
   }
   case 0x3A: if(chararray[0x3A]=='0'){
                    chararray[0x3A]='1';
                  }
              else {
                  chararray[0x3A]='0';
                }break;
   default:{ if( ( chararray[0x3A]=='1'|| chararray[0x2A]=='1'||chararray[0x36]=='1') && chararray[a]>=97 && chararray[a]<=122)
              {

                char temp = chararray[a]-32;
                if(temp=='L' && chararray[0x1D]=='1' ){
                  clear();
                  setposition(0,0);
                    update_cursor();
                    numberofchars=0;
                    memset(linebuffer,0,128);

                }
                else{
                if(numberofchars!=127){
                linebuffer[numberofchars]=temp;
                putc(temp);
                  update_cursor();
                numberofchars=numberofchars+1;
              }
              }
                }
            else
              {
                if(chararray[a]&& numberofchars!=127)
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
/*
 if(chararray[a]=='e')
    enable_irq(8);
 if(chararray[a]=='d')
    disable_irq(8);
*/

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
chararray[0x3A]='0';
chararray[0x2A]='0';


}
