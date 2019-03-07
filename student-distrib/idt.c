/* the .c file for the iterupt descriptor table */

#include "idt.h"
#include "x86_desc.h"
#include "lib.h"

void handle0(){
    printf("Divide by zero error");
  

}
void handle1(){
    printf("Debug exception");
}
void handle2(){
    printf("NMI Interupt");
}
void handle3(){
    printf("Breakpoint Exception");
}
void handle4(){
    printf("Overflow Exception");
}
void handle5(){
    printf("BOUND Range Exceeded Exception");
}
void handle6(){
    printf("Invalid Opcode Exception");
}
void handle7(){
    printf("Device Not Available Exception");
}
void handle8(){
    printf("Double Fault Exception");
}
void handle9(){
    printf("Coprocessor Segment Overrun");
}
void handle10(){
    printf("Invalid TSS exception");
}
void initialize_IDT(){

   int i=0;



 for(i=0;i<10;i++){
   idt[i].dpl=0;
   idt[i].present=1;
   idt[i].seg_selector=KERNEL_CS;
   idt[i].reserved4=0;
   idt[i].reserved3=1;
   idt[i].reserved2=1;
   idt[i].reserved1=1;
   idt[i].reserved0=0;
   idt[i].size=1;
   if(i==0)
   SET_IDT_ENTRY(idt[i],handle0);
   else if(i==1)
  SET_IDT_ENTRY(idt[i],handle1);
   else if(i==2)
   SET_IDT_ENTRY(idt[i],handle2);
   else if(i==3)
   SET_IDT_ENTRY(idt[i],handle3);
   else if(i==4)
   SET_IDT_ENTRY(idt[i],handle4);
   else if(i==5)
   SET_IDT_ENTRY(idt[i],handle5);
   else if(i==6)
   SET_IDT_ENTRY(idt[i],handle6);
   else if(i==7)
    SET_IDT_ENTRY(idt[i],handle7);
   else if(i==8)
     SET_IDT_ENTRY(idt[i],handle8);
   else
     SET_IDT_ENTRY(idt[i],handle9);

 }





}
