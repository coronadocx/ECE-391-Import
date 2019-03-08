


.global handle0asm
.global rtchandlerasm
handle0asm :
 
   pushal
   call handle0

   popal

   iret

rtchandlerasm :
      pushal

      call rtchandler

      popal

      iret


.end
