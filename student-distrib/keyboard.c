/* c file for keyboard */
#include "lib.h"
#include "i8259.h"
char chararray[255];
void check_input(){

 uint32_t a;
 char b;
 a=inb(0x60);
 putc(chararray[a]);
 send_eoi(1);


}


void init_keyboard(){
  enable_irq(1);
  chararray[0x1E]='a';
  chararray[0x02]='1';
  chararray[0x03]='2';
  chararray[0x04]='3';
  chararray[0x05]='4';
  chararray[0x06]='5';
  chararray[0x07]='6';
  chararray[0x08]='7';
  chararray[0x09]='8';
  chararray[0x0A]='9';
  chararray[0x0B]='0';

  chararray[0x1E]='a';
  chararray[0x30]='b';
  chararray[0x2E]='c';
  chararray[0x20]='d';
  chararray[0x12]='e';
  chararray[0x21]='f';
  chararray[0x22]='g';
  chararray[0x23]='h';
  chararray[0x17]='i';
  chararray[0x24]='j';
  chararray[0x25]='k';
  chararray[0x26]='l';
  chararray[0x32]='m';
  chararray[0x31]='n';
  chararray[0x18]='o';
  chararray[0x19]='p';
  chararray[0x10]='q';
  chararray[0x13]='r';
  chararray[0x1F]='s';
  chararray[0x14]='t';
  chararray[0x16]='u';
  chararray[0x2f]='v';
  chararray[0x11]='w';
  chararray[0x2d]='x';
  chararray[0x15]='y';
  chararray[0x2c]='z';



}
