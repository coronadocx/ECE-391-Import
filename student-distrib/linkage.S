/* Assembly linkage for interrupt handlers */

/* This is explicitly done to save FLAGS and registers on the stack
*  upon encountering an interrupt
*/


/* Defining global keyboard and rtc handlers */
.global keyboardhandlerasm
.global rtchandlerasm

/* keyboardhandlerasm
* INPUTS -- none
* OUTPUTS -- none
* RETURN Value -- none
* Side Effects -- Saves registers and flags, calls the C keyboard handler,
* pops all registers and flags and does an iret to return to the main program at the
* location it was interrupted at
*/
keyboardhandlerasm :

   pushal
   call keyboard_handler

   popal

   iret


   /* rtchandlerasm
   * INPUTS -- none
   * OUTPUTS -- none
   * RETURN Value -- none
   * Side Effects -- Saves registers and flags, calls the C rtc handler,
   * pops all registers and flags and does an iret to return to the main program at the
   * location it was interrupted at
   */
rtchandlerasm :
      pushal

      call rtchandler

      popal

      iret


.end
