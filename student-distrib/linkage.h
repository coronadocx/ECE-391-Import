#ifndef _LINKAGE_H
#define _LINKAGE_H

#include "types.h"
#include "pcb.h"
#include "x86_desc.h"
void keyboardhandlerasm();
void rtchandlerasm();
void systemcallasm();
void contextswitchasm(uint32_t eipval,pcb* current_process);

#endif
