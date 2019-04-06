/* Assembly linkage for interrupt handlers */

/* This is explicitly done to save FLAGS and registers on the stack
*  upon encountering an interrupt
*/

.text

/* Defining global keyboard and rtc handlers */
.global keyboardhandlerasm
.global rtchandlerasm
.global systemcallasm
.global contextswitchasm
.global restoreparent

contextswitchasm:
    xorl %edx,%edx
    movl 8(%esp),%edx
    xorl %ecx,%ecx
    movl %esp,%ecx

    /* code to save parent process stuff */
    movl $0x002B, %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl %ebp
    pushl %ecx
    pushl %edx
    call save_parent_registers
    add $12,%esp

    xorl %edx,%edx
    movl 4(%esp),%edx

    pushl $0x002B
    pushl $0x83ffffc
    pushfl
    pushl $0x0023
    pushl %edx


    iret

    ret

restoreparent:
     movl 4(%esp),%ebp
     movl 8(%esp),%esp

     ret

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

         popl %ebx
         popl %ecx
         popl %edx

         popfl
         iret
jumptable:
   .long halt_handlerasm,execute_handlerasm,read_handlerasm,write_handlerasm,open_handlerasm,close_handlerasm
.end
