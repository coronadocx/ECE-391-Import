/* Assembly linkage for interrupt handlers */

/* This is explicitly done to save FLAGS and registers on the stack
*  upon encountering an interrupt
*/


/* Defining global keyboard and rtc handlers */
.global keyboardhandlerasm
.global rtchandlerasm
.global systemcallasm

/* keyboardhandlerasm
* INPUTS -- none
* OUTPUTS -- none
* RETURN Value -- none
* Side Effects -- Saves registers and flags, calls the C keyboard handler,
* pops all registers and flags and does an iret to return to the main program at the
* location it was interrupted at
*/
keyboardhandlerasm:
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
rtchandlerasm:
      pushal

      call rtchandler

      popal

      iret

halt_handlerasm:
   call halt
   jmp RETURN
execute_handlerasm:
   call execute

   jmp RETURN




read_handlerasm:
  call read

  jmp RETURN


write_handlerasm:
  call write

  jmp RETURN

open_handlerasm:
  call open

  jmp RETURN

close_handlerasm:
    call close

    jmp RETURN

systemcallasm:

          cmpl $0,%eax
          jle  DEFAULTERROR
          cmpl $6,%eax
          jg   DEFAULTERROR
          pushfl
          pushal

          pushl %edx
          pushl %ecx
          pushl %ebx

          addl $1,%eax


          jmp  *jumptable(,%eax,4)

DEFAULTERROR:
    movl $-1,%eax
    iret

RETURN:
    add $12,%esp
    popfl
    popal
    iret
jumptable:
   .long halt_handlerasm,execute_handlerasm,read_handlerasm,write_handlerasm,open_handlerasm,close_handlerasm
.end
