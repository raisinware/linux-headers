/*
 *  include/asm-i386/bugs.h
 *
 *  Copyright (C) 1994  Linus Torvalds
 */

/*
 * This is included by init/main.c to check for architecture-dependent bugs.
 *
 * Needs:
 *	void check_bugs(void);
 */

#include <linux/config.h>
#include <asm/processor.h>

#define CONFIG_BUGi386

__initfunc(static void no_halt(char *s, int *ints))
{
	hlt_works_ok = 0;
}

__initfunc(static void no_387(char *s, int *ints))
{
	hard_math = 0;
	__asm__("movl %%cr0,%%eax\n\t"
		"orl $0xE,%%eax\n\t"
		"movl %%eax,%%cr0\n\t" : : : "ax");
}

static char __initdata fpu_error = 0;

__initfunc(static void copro_timeout(void))
{
	fpu_error = 1;
	timer_table[COPRO_TIMER].expires = jiffies+100;
	timer_active |= 1<<COPRO_TIMER;
	printk(KERN_ERR "387 failed: trying to reset\n");
	send_sig(SIGFPE, last_task_used_math, 1);
	outb_p(0,0xf1);
	outb_p(0,0xf0);
}

static double __initdata x = 4195835.0;
static double __initdata y = 3145727.0;

__initfunc(static void check_fpu(void))
{
	unsigned short control_word;

	if (!hard_math) {
#ifndef CONFIG_MATH_EMULATION
		printk(KERN_EMERG "No coprocessor found and no math emulation present.\n");
		printk(KERN_EMERG "Giving up.\n");
		for (;;) ;
#endif
		return;
	}
	/*
	 * check if exception 16 works correctly.. This is truly evil
	 * code: it disables the high 8 interrupts to make sure that
	 * the irq13 doesn't happen. But as this will lead to a lockup
	 * if no exception16 arrives, it depends on the fact that the
	 * high 8 interrupts will be re-enabled by the next timer tick.
	 * So the irq13 will happen eventually, but the exception 16
	 * should get there first..
	 */
	printk(KERN_INFO "Checking 386/387 coupling... ");
	timer_table[COPRO_TIMER].expires = jiffies+50;
	timer_table[COPRO_TIMER].fn = copro_timeout;
	timer_active |= 1<<COPRO_TIMER;
	__asm__("clts ; fninit ; fnstcw %0 ; fwait":"=m" (*&control_word));
	control_word &= 0xffc0;
	__asm__("fldcw %0 ; fwait": :"m" (*&control_word));
	outb_p(inb_p(0x21) | (1 << 2), 0x21);
	__asm__("fldz ; fld1 ; fdiv %st,%st(1) ; fwait");
	timer_active &= ~(1<<COPRO_TIMER);
	if (fpu_error)
		return;
	if (!ignore_irq13) {
		printk("Ok, fpu using old IRQ13 error reporting\n");
		return;
	}
	__asm__("fninit\n\t"
		"fldl %1\n\t"
		"fdivl %2\n\t"
		"fmull %2\n\t"
		"fldl %1\n\t"
		"fsubp %%st,%%st(1)\n\t"
		"fistpl %0\n\t"
		"fwait\n\t"
		"fninit"
		: "=m" (*&fdiv_bug)
		: "m" (*&x), "m" (*&y));
	if (!fdiv_bug)
		printk("Ok, fpu using exception 16 error reporting.\n");
	else
		printk("Hmm, fpu using exception 16 error reporting with FDIV bug.\n");
}

__initfunc(static void check_hlt(void))
{
	printk(KERN_INFO "Checking 'hlt' instruction... ");
	if (!hlt_works_ok) {
		printk("disabled\n");
		return;
	}
	__asm__ __volatile__("hlt ; hlt ; hlt ; hlt");
	printk("Ok.\n");
}

__initfunc(static void check_tlb(void))
{
#ifndef CONFIG_M386
	/*
	 * The 386 chips don't support TLB finegrained invalidation.
	 * They will fault when they hit an invlpg instruction.
	 */
	if (x86 == 3) {
		printk(KERN_EMERG "CPU is a 386 and this kernel was compiled for 486 or better.\n");
		printk("Giving up.\n");
		for (;;) ;
	}
#endif
}

/*
 *	Most 386 processors have a bug where a POPAD can lock the 
 *	machine even from user space.
 */
 
__initfunc(static void check_popad(void))
{
#ifdef CONFIG_M386
	int res, inp = (int) &res;

	printk(KERN_INFO "Checking for popad bug... ");
	__asm__ __volatile__( 
	  "movl $12345678,%%eax; movl $0,%%edi; pusha; popa; movl (%%edx,%%edi),%%ecx "
	  : "=eax" (res)
	  : "edx" (inp)
	  : "eax", "ecx", "edx", "edi" );
	/* If this fails, it means that any user program may lock CPU hard. Too bad. */
	if (res != 12345678) printk( "Bad.\n" );
		        else printk( "Ok.\n" );
#endif
}

/*
 *	B step AMD K6 before B 9729AIJW have hardware bugs that can cause
 *	misexecution of code under Linux. Owners of such processors should
 *	contact AMD for precise details and a CPU swap.
 *
 *	See	http://www.creaweb.fr/bpc/k6bug_faq.html
 *		http://www.amd.com/K6/k6docs/revgd.html
 */
 
__initfunc(static void check_amd_k6(void))
{
	/* B Step AMD K6 */
	if(x86_model==6 && x86_mask==1 && memcmp(x86_vendor_id, "AuthenticAMD", 12)==0)
	{
		printk(KERN_INFO "AMD K6 stepping B detected - system stability may be impaired. Please see.\n");
		printk(KERN_INFO "http://www.creaweb.fr/bpc/k6bug_faq.html");
	}
}

/*
 * All current models of Pentium and Pentium with MMX technology CPUs
 * have the F0 0F bug, which lets nonpriviledged users lock up the system:
 */

extern int pentium_f00f_bug;

__initfunc(static void check_pentium_f00f(void))
{
	/*
	 * Pentium and Pentium MMX
	 */
	printk("checking for F00F bug ...");
	if(x86==5 && !memcmp(x86_vendor_id, "GenuineIntel", 12))
	{
		extern void trap_init_f00f_bug(void);

		printk(KERN_INFO "\nIntel Pentium/[MMX] F0 0F bug detected - turning on workaround.\n");
		pentium_f00f_bug = 1;
		trap_init_f00f_bug();
	} else {
		printk(KERN_INFO " no F0 0F bug in this CPU, great!\n");
		pentium_f00f_bug = 0;
	}
}

__initfunc(static void check_bugs(void))
{
	check_tlb();
	check_fpu();
	check_hlt();
	check_popad();
	check_amd_k6();
	check_pentium_f00f();
	system_utsname.machine[1] = '0' + x86;
}
