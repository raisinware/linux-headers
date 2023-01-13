#ifndef _ALPHA_SEMAPHORE_H
#define _ALPHA_SEMAPHORE_H

/*
 * SMP- and interrupt-safe semaphores..
 *
 * (C) Copyright 1996 Linus Torvalds
 * (C) Copyright 1996 Richard Henderson
 */

#include <asm/current.h>
#include <asm/system.h>
#include <asm/atomic.h>

struct semaphore {
	/* Careful, inline assembly knows about the position of these two.  */
	atomic_t count;
	atomic_t waking;		/* biased by -1 */
	struct wait_queue *wait;
};

#define MUTEX ((struct semaphore) \
 { ATOMIC_INIT(1), ATOMIC_INIT(-1), NULL })
#define MUTEX_LOCKED ((struct semaphore) \
 { ATOMIC_INIT(0), ATOMIC_INIT(-1), NULL })

#define sema_init(sem, val)	atomic_set(&((sem)->count), val)

extern void __down(struct semaphore * sem);
extern int  __down_interruptible(struct semaphore * sem);
extern int  __down_trylock(struct semaphore * sem);
extern void __up(struct semaphore * sem);

/* All have custom assembly linkages.  */
extern void __down_failed(struct semaphore * sem);
extern void __down_failed_interruptible(struct semaphore * sem);
extern void __down_failed_trylock(struct semaphore * sem);
extern void __up_wakeup(struct semaphore * sem);

/*
 * Whee.  Hidden out of line code is fun.  The contention cases are
 * handled out of line in kernel/sched.c; arch/alpha/lib/semaphore.S
 * takes care of making sure we can call it without clobbering regs.
 */

extern inline void down(struct semaphore * sem)
{
	/* Given that we have to use particular hard registers to 
	   communicate with __down_failed anyway, reuse them in 
	   the atomic operation as well. 

	   __down_failed takes the semaphore address in $24, and
	   it's return address in $28.  The pv is loaded as usual.
	   The gp is clobbered (in the module case) as usual.  */

	__asm__ __volatile__ (
		"/* semaphore down operation */\n"
		"1:	ldl_l	$27,%0\n"
		"	subl	$27,1,$27\n"
		"	mov	$27,$28\n"
		"	stl_c	$28,%0\n"
		"	beq	$28,2f\n"
		"	blt	$27,3f\n"
		"4:	mb\n"
		".section .text2,\"ax\"\n"
		"2:	br	1b\n"
		"3:	lda	$24,%0\n"
		"	jsr	$28,__down_failed\n"
		"	ldgp	$29,0($28)\n"
		"	br	4b\n"
		".previous"
		: : "m"(sem->count)
		: "$24", "$27", "$28", "memory");
}

extern inline int down_interruptible(struct semaphore * sem)
{
	/* __down_failed_interruptible takes the semaphore address in $24,
	   and it's return address in $28.  The pv is loaded as usual.
	   The gp is clobbered (in the module case) as usual.  The return
	   value is in $24.  */

	register int ret __asm__("$24");

	__asm__ __volatile__ (
		"/* semaphore down interruptible operation */\n"
		"1:	ldl_l	$27,%1\n"
		"	subl	$27,1,$27\n"
		"	mov	$27,$28\n"
		"	stl_c	$28,%1\n"
		"	beq	$28,2f\n"
		"	blt	$27,3f\n"
		"	mov	$31,%0\n"
		"4:	mb\n"
		".section .text2,\"ax\"\n"
		"2:	br	1b\n"
		"3:	lda	$24,%1\n"
		"	jsr	$28,__down_failed_interruptible\n"
		"	ldgp	$29,0($28)\n"
		"	br	4b\n"
		".previous"
		: "=r"(ret)
		: "m"(sem->count)
		: "$27", "$28", "memory");

	return ret;
}

/*
 * down_trylock returns 0 on success, 1 if we failed to get the lock.
 *
 * We must manipulate count and waking simultaneously and atomically.
 * Do this by using ll/sc on the pair of 32-bit words.
 */

extern inline int down_trylock(struct semaphore * sem)
{
	long ret, tmp, tmp2;

	/* "Equivalent" C.  Note that we have to do this all without
	   (taken) branches in order to be a valid ll/sc sequence.

	   do {
	       tmp = ldq_l;
	       ret = 0;
	       tmp -= 1;
	       if ((int)tmp < 0)		// count
	           break;
	       if ((long)tmp < 0)		// waking
	           break;
	       tmp += 0xffffffff00000000;
	       ret = 1;
	       tmp = stq_c = tmp;
	   } while (tmp == 0);
	*/

	__asm__ __volatile__(
		"1:	ldq_l	%1,%3\n"
		"	lda	%0,0\n"
		"	subl	%1,1,%2\n"
		"	subq	%1,1,%1\n"
		"	blt	%2,2f\n"
		"	blt	%1,2f\n"
		"	ldah	%1,-32768(%1)\n"
		"	ldah	%1,-32768(%1)\n"
		"	lda	%0,1\n"
		"	stq_c	%1,%3\n"
		"	beq	%1,3f\n"
		"2:	mb\n"
		".section .text2,\"ax\"\n"
		"3:	br	1b\n"
		".previous"
		: "=&r"(ret), "=&r"(tmp), "=&r"(tmp2)
		: "m"(*sem)
		: "memory");

	return ret;
}

extern inline void up(struct semaphore * sem)
{
	/* Given that we have to use particular hard registers to 
	   communicate with __up_wakeup anyway, reuse them in 
	   the atomic operation as well. 

	   __up_wakeup takes the semaphore address in $24, and
	   it's return address in $28.  The pv is loaded as usual.
	   The gp is clobbered (in the module case) as usual.  */

	__asm__ __volatile__ (
		"/* semaphore up operation */\n"
		"	mb\n"
		"1:	ldl_l	$27,%0\n"
		"	addl	$27,1,$27\n"
		"	mov	$27,$28\n"
		"	stl_c	$28,%0\n"
		"	beq	$28,2f\n"
		"	mb\n"
		"	ble	$27,3f\n"
		"4:\n"
		".section .text2,\"ax\"\n"
		"2:	br	1b\n"
		"3:	lda	$24,%0\n"
		"	jsr	$28,__up_wakeup\n"
		"	ldgp	$29,0($28)\n"
		"	br	4b\n"
		".previous"
		: : "m"(sem->count)
		: "$24", "$27", "$28", "memory");
}

#endif
