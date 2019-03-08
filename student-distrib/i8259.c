/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"


/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask=0xFF; /* IRQs 0-7  */
uint8_t slave_mask=0xFF;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {

  unsigned long flags;


  outb(0xFF,0x21);
  outb(0xFF,0xA1);

 //set up master
  outb(ICW1,MASTER_8259_PORT);
  outb(ICW2_MASTER,0x21); // master is at port 0x20 so this is that +1;
  outb(ICW3_MASTER ,0x21); // slave mapped to masters irq2
  outb(ICW4,0x21);

 //set up slave
  outb(ICW1,SLAVE_8259_PORT);
  outb(ICW2_SLAVE,0xA1); // slave is at port 0xA0 so this is that +1
  outb(ICW3_SLAVE,0xA1);
  outb(ICW4,0xA1);


  outb(master_mask,0x21);
  outb(slave_mask,0xA1);


}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
  int port=0x00;

if(irq_num>=0 && irq_num<8){
  port =0x21;
  master_mask &= ~(1<<irq_num);
  outb(master_mask,port);
}
if(irq_num>=8 && irq_num<16){
  port=0xA1;
  slave_mask &= ~(1<<irq_num);
  outb(slave_mask,port);
}


}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
  int port=0x00;
if(irq_num>=0 && irq_num<8){
  port =0x21;
  master_mask |= 1<<irq_num;
    outb(master_mask,port);
}
if(irq_num>=8 && irq_num<15){
  port=0xA1;
  slave_mask |= 1<<irq_num;
  outb(slave_mask,port);
}


}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
 int returnvl = EOI | irq_num;
 if(irq_num>=8)
    outb(returnvl,SLAVE_8259_PORT);
 outb(returnvl,MASTER_8259_PORT);

}
