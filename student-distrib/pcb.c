#include "pcb.h"


void init_pcb(){

}
pcb* get_pcb_address(){

  /* Here, we try to calculate which PCB this system call belongs to
   * We do this by taking the Top of Stack Pointer and ANDing it with a
     bitmask to get the nearest 8KB aligned page
     */
    int x;


  /* Bit-Mask 0xFFE000 should give us an 8KB aligned address of the PCB */
    return ((pcb*)&x) & (PCB_MASK);

};
