/* Assembly linkage for interrupt handlers */

/* This is explicitly done to save FLAGS and registers on the stack
*  upon encountering an interrupt
*/

#include "x86_desc.h"
#include "sys_call.h"
/* Defining global keyboard and rtc handlers */
.global keyboardhandlerasm
.global rtchandlerasm
.global systemcallasm
.global contextswitchasm

contextswitchasm:
    pushl %ebp
    movl %esp,%ebp

    /* code to save parent process stuff */
    pushl %ebp
    pushl %esp
    pushl 12(%ebp)
    call save_parent_registers
    add $12,%esp


    pushl $USER_DS
    pushl %esp
    pushfl
    pushl $USER_CS
    pushl 8(%ebp)

    leave
    iret



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
          cmpl $10,%eax
          jg   DEFAULTERROR
          pushfl
        #  pushal
        # Manually push all values of registers

          pushl %edx
          pushl %ecx
          pushl %ebx

          pushl %esi
          pushl %edi

        # %eax not saved here


          pushl %edx
          pushl %ecx
          pushl %ebx

          subl $1,%eax


          jmp  *jumptable(,%eax,4)

DEFAULTERROR:
          movl $-1,%eax
          iret

RETURN:
        # Tearing down syscall args
          add $12,%esp
    /* Popal first and then PopFl */
    # popal
    # Restoring all values except %eax
    # We need %eax for the return value
         popl %edi
         popl %esi

         pushl %ebx
         pushl %ecx
         pushl %edx

         popfl
         iret
jumptable:
   .long halt_handlerasm,execute_handlerasm,read_handlerasm,write_handlerasm,open_handlerasm,close_handlerasm
.end
