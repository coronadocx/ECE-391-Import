/* c file for rtc */

#include "i8259.h"
#include "rtc.h"
#include "pcb.h"

/*
 * init_rtc
 *   DESCRIPTION: Function to initialize the RTC, including setting the correct
 *                Registers A and B and setting the frame rate
 *   INPUTS: None
 *   OUTPUTS:None
 *   RETURN VALUE: None
 *   SIDE EFFECTS:  Initializes the RTC, and sets the current frequency
 *                  Given by the lower 4 bits of register A
 */
void init_rtc()
{
	int32_t i;

	/* referenced wiki.osdev.org/RTC to understand the rtc */
	outb(REG_B,RTC_CMD_PORT);
	char prev = inb(RTC_DATA_PORT);
	outb(REG_B,RTC_CMD_PORT);
	outb(prev|RTC_MASK,RTC_DATA_PORT);
	enable_irq(RTC_IRQ_NO);
	
	// Initialize vRTC vars
	for (i=0; i<MAX_PROCESS; i++) {
		rtc_proc_intr[i] = 0;
		rtc_proc_ctr[i] = 0;
		rtc_proc_ctr_max[i] = 0;
	}

	_rtc_set_freq_hard(MAX_FREQ);	// Initialize at highest frequency

	return;
}


/*
 * 	rtc_read
 *	DESCRIPTION: Blocks current process until next RTC interrupt
 * 	INPUT:	fd		- UNUSED
 * 	OUTPUT:	buf		- UNUSED
 *			nbytes	- UNUSED
 * 	RETURN VALUE: Returns 0 once an interrupt has occurred
 * 	SIDE EFFECT: NONE
 *  NOTE: This does not read the current RTC frequency
 */
int32_t rtc_read(int32_t fd,void* buf,int32_t nbytes)
{
	int32_t pid = get_pcb_address()->process_id;
	rtc_proc_intr[pid] = 0; 
	while (rtc_proc_intr[pid] == 0) {/*Wait for interrupt*/};
	return 0;
}


/*
 * 	rtc_write
 * 	DESCRIPTION: Changes the RTC's frequency
 * 	INPUT:	fd		- File descriptor for the RTC
 * 			buf		- Pointer to a 4-byte integer specifying the interrupt rate
 *	OUTPUT: nbytes  - Holds the number of bytes to write
 *	RETURN VALUE: 0 on success, -1 on failure
 *	SIDE EFFECT: Modifies the process's rtc counter max
 */
int32_t rtc_write(int32_t fd,const void * buf, int32_t nbytes)
{
  	int32_t* buf2= (int32_t*) buf;

	if ( buf2 == NULL) {
		return -1;
	}

	else if((*buf2) & ((*buf2)-1) || *buf2 > MAX_FREQ){
		return -1;
	}

	_rtc_set_freq((*buf2));
	return 0;
}


/*
 * 	rtc_open
 * 	DESCRIPTION: Sets RTC frequency to 2Hz then returns
 * 	INPUT: filename - UNUSED
 *	OUTPUT: none
 *	RETURN VALUE: 0 always
 *	SIDE EFFECT: Changes the current process's RTC frequency to 2HZ
 */
int32_t rtc_open(const uint8_t * filename)
{
	(void)_rtc_set_freq(MIN_FREQ);
	return 0;
}


/*	NOTE: Will be implemented in later checkpoint
 * 	rtc_close
 *  DESCRIPTION: Does nothing
 * 	INPUT: none
 *	OUTPUT:none
 *	RETURN VALUE: 0 always
 *	SIDE EFFECT: none
 */
int32_t rtc_close(int32_t fd)
{
	return 0;
}

/* 
 *	_rct_set_freq
 *	DESCRIPTION: Set the frequency at which a process's vRTC interrupts are generated
 *	INPUT: freq - frequency the process will be set to
 * 	OUTPUT: none
 * 	RETURN VALUE: 0 always
 *	SIDE EFFECTS: modifies rtc_proc_ctr_max for a given process
 */
void _rtc_set_freq(int32_t freq)
{
	rtc_proc_ctr_max[get_pcb_address()->process_id] = (MAX_FREQ/freq);
	return;
}


/*
 * 	_rtc_set_freq_hard
 * 	DESCRIPTION: Set the frequency at which rtc interrupts are generated 
 *	INPUT: freq - frequency the hardware will be set to
 *	OUTOUT: none
 *	RETURN VALUE: none
 *	SIDE EFFECTS: changes the rate at which the rtc chip generates interrupts
 */
void _rtc_set_freq_hard(int32_t freq)
{	
	int32_t tmp;
	int8_t rate = INIT_RATE;
	int8_t prev;

	if (freq & (freq-1) || freq > MAX_FREQ || freq < MIN_FREQ)
		return;

	// Calculate the RTC rate setting
	tmp = MAX_FREQ;
	while (tmp != freq){
		rate++;
		tmp = tmp >> 1;
	}

	// Change RTC hardware to calculated rate
	cli();										// Disable interrupts
	outb(REG_A_NMI_OFF,RTC_CMD_PORT);			// Select A, NMI off
	prev = inb(RTC_DATA_PORT);					// Collect old state
	outb(REG_A_NMI_OFF,RTC_CMD_PORT);			// Select A again
	outb((prev & MASK_LOWER) | rate,RTC_DATA_PORT);	// Write new rate to RTC
	sti();										// Enable interrupts

	return;	
}