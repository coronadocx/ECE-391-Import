


.global keyboardhandlerasm
.global rtchandlerasm
keyboardhandlerasm :

   pushal
   call keyboard_handler

   popal

   iret

rtchandlerasm :
      pushal

      call rtchandler

      popal

      iret


.end
