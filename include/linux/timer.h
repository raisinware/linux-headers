#ifndef _LINUX_TIMER_H
#define _LINUX_TIMER_H

/*
 * Old-style timers. Please don't use for any new code.
 *
 * Numbering of these timers should be consecutive to minimize
 * processing delays. [MJ]
 */

#define BLANK_TIMER	0	/* Console screen-saver */
#define BEEP_TIMER	1	/* Console beep */
#define RS_TIMER	2	/* RS-232 ports */
#define SWAP_TIMER	3	/* Background pageout */
#define BACKGR_TIMER    4	/* io_request background I/O */
#define HD_TIMER	5	/* Old IDE driver */
#define FLOPPY_TIMER	6	/* Floppy */
#define QIC02_TAPE_TIMER 7	/* QIC 02 tape */
#define MCD_TIMER	8	/* Mitsumi CDROM */
#define GSCD_TIMER	9	/* Goldstar CDROM */
#define COMTROL_TIMER	10	/* Comtrol serial */
#define DIGI_TIMER	11	/* Digi serial */

#define COPRO_TIMER	31	/* 387 timeout for buggy hardware (boot only) */

struct timer_struct {
	unsigned long expires;
	void (*fn)(void);
};

extern unsigned long timer_active;
extern struct timer_struct timer_table[32];

/*
 * This is completely separate from the above, and is the
 * "new and improved" way of handling timers more dynamically.
 * Hopefully efficient and general enough for most things.
 *
 * The "hardcoded" timers above are still useful for well-
 * defined problems, but the timer-list is probably better
 * when you need multiple outstanding timers or similar.
 *
 * The "data" field is in case you want to use the same
 * timeout function for several timeouts. You can use this
 * to distinguish between the different invocations.
 */
struct timer_list {
	struct timer_list *next;
	struct timer_list *prev;
	unsigned long expires;
	unsigned long data;
	void (*function)(unsigned long);
};

extern void add_timer(struct timer_list * timer);
extern int  del_timer(struct timer_list * timer);

extern void it_real_fn(unsigned long);

extern inline void init_timer(struct timer_list * timer)
{
	timer->next = NULL;
	timer->prev = NULL;
}

#endif
