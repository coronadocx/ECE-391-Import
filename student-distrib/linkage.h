#ifndef _LINKAGE_H
#define _LINKAGE_H

#include "types.h"
#include "pcb.h"
#include "x86_desc.h"
void keyboardhandlerasm();
void rtchandlerasm();
void systemcallasm();
void contextswitchasm(uint32_t eipval,pcb* current_process);
void restoreparent(uint32_t ebp,uint32_t esp);
#endif
