/* $Id: system.h,v 1.56 1997/04/14 05:39:30 davem Exp $ */
#ifndef __SPARC_SYSTEM_H
#define __SPARC_SYSTEM_H

#include <linux/kernel.h>

#include <asm/segment.h>

#ifdef __KERNEL__
#include <asm/page.h>
#include <asm/oplib.h>
#include <asm/psr.h>
#include <asm/ptrace.h>

#define EMPTY_PGT       (&empty_bad_page)
#define EMPTY_PGE	(&empty_bad_page_table)
#endif /* __KERNEL__ */

#ifndef __ASSEMBLY__

/*
 * Sparc (general) CPU types
 */
enum sparc_cpu {
  sun4        = 0x00,
  sun4c       = 0x01,
  sun4m       = 0x02,
  sun4d       = 0x03,
  sun4e       = 0x04,
  sun4u       = 0x05, /* V8 ploos ploos */
  sun_unknown = 0x06,
  ap1000      = 0x07, /* almost a sun4m */
};

/* Really, userland should not be looking at any of this... */
#ifdef __KERNEL__

extern enum sparc_cpu sparc_cpu_model;

extern unsigned long empty_bad_page;
extern unsigned long empty_bad_page_table;
extern unsigned long empty_zero_page;

extern struct linux_romvec *romvec;
#define halt() romvec->pv_halt()

/* When a context switch happens we must flush all user windows so that
 * the windows of the current process are flushed onto its stack. This
 * way the windows are all clean for the next process and the stack
 * frames are up to date.
 */
extern void flush_user_windows(void);
extern void kill_user_windows(void);
extern void synchronize_user_stack(void);
extern void fpsave(unsigned long *fpregs, unsigned long *fsr,
		   void *fpqueue, unsigned long *fpqdepth);

#ifdef __SMP__
#define SWITCH_ENTER \
	if(prev->flags & PF_USEDFPU) { \
		put_psr(get_psr() | PSR_EF); \
		fpsave(&prev->tss.float_regs[0], &prev->tss.fsr, \
		       &prev->tss.fpqueue[0], &prev->tss.fpqdepth); \
		prev->flags &= ~PF_USEDFPU; \
		prev->tss.kregs->psr &= ~PSR_EF; \
	}

#define SWITCH_DO_LAZY_FPU
#else
#define SWITCH_ENTER
#define SWITCH_DO_LAZY_FPU if(last_task_used_math != next) next->tss.kregs->psr&=~PSR_EF;
#endif

	/* Much care has gone into this code, do not touch it. */
#define switch_to(prev, next) do {							\
	__label__ here;									\
	register unsigned long task_pc asm("o7");					\
	SWITCH_ENTER									\
	SWITCH_DO_LAZY_FPU								\
	__asm__ __volatile__(								\
	".globl\tflush_patch_switch\nflush_patch_switch:\n\t"				\
	"save %sp, -0x40, %sp; save %sp, -0x40, %sp; save %sp, -0x40, %sp\n\t"		\
	"save %sp, -0x40, %sp; save %sp, -0x40, %sp; save %sp, -0x40, %sp\n\t"		\
	"save %sp, -0x40, %sp\n\t"							\
	"restore; restore; restore; restore; restore; restore; restore");		\
	if(!(next->tss.flags & SPARC_FLAG_KTHREAD) &&					\
	   !(next->flags & PF_EXITING))							\
		switch_to_context(next);						\
	task_pc = ((unsigned long) &&here) - 0x8;					\
	__asm__ __volatile__(								\
	"rd	%%psr, %%g4\n\t"							\
	"std	%%sp, [%%g6 + %3]\n\t"							\
	"rd	%%wim, %%g5\n\t"							\
	"wr	%%g4, 0x20, %%psr\n\t"							\
	"nop\n\t"									\
	"mov	%5, %%g7\n\t"								\
	"std	%%g4, [%%g6 + %2]\n\t"							\
	"st	%%g7, [%%g6 + %6]\n\t"							\
	"ldd	[%1 + %2], %%g4\n\t"							\
	"mov	%1, %%g6\n\t"								\
	"st	%1, [%0]\n\t"								\
	"wr	%%g4, 0x20, %%psr\n\t"							\
	"nop\n\t"									\
	"nop\n\t"									\
	"ldd	[%%g6 + %3], %%sp\n\t"							\
	"wr	%%g5, 0x0, %%wim\n\t"							\
	"ldd	[%%sp + 0x00], %%l0\n\t"						\
	"ldd	[%%sp + 0x38], %%i6\n\t"						\
	"wr	%%g4, 0x0, %%psr\n\t"							\
	"nop\n\t"									\
	"nop\n\t"									\
	"jmpl	%%o7 + 0x8, %%g0\n\t"							\
	" nop\n\t" : : "r" (&(current_set[smp_processor_id()])), "r" (next),		\
	  "i" ((const unsigned long)(&((struct task_struct *)0)->tss.kpsr)),		\
	  "i" ((const unsigned long)(&((struct task_struct *)0)->tss.ksp)),		\
	  "r" (task_pc), "i" (255),							\
	  "i" ((const unsigned long)(&((struct task_struct *)0)->processor))		\
	: "g1", "g2", "g3", "g4", "g5", "g7", "l2", "l3",				\
	"l4", "l5", "l6", "l7", "i0", "i1", "i2", "i3", "i4", "i5", "o0", "o1", "o2",	\
	"o3");										\
here:  } while(0)

/* Changing the IRQ level on the Sparc.   We now avoid writing the psr
 * whenever possible.
 */
extern __inline__ void setipl(unsigned long __orig_psr)
{
	__asm__ __volatile__("
		wr	%0, 0x0, %%psr
		nop
		nop
		nop
"		: /* no outputs */
		: "r" (__orig_psr)
		: "memory", "cc");
}

extern __inline__ void __cli(void)
{
	unsigned long tmp;

	__asm__ __volatile__("
		rd	%%psr, %0
		andcc	%0, %1, %%g0
		bne	1f
		 nop
		wr	%0, %1, %%psr
		nop
		nop
		nop
1:
"		: "=r" (tmp)
		: "i" (PSR_PIL)
		: "memory");
}

extern __inline__ void __sti(void)
{
	unsigned long tmp;

	__asm__ __volatile__("
		rd	%%psr, %0
		andcc	%0, %1, %%g0
		be	1f
		 nop
		wr	%0, %1, %%psr
		nop
		nop
		nop
1:
"		: "=r" (tmp)
		: "i" (PSR_PIL)
		: "memory");
}

extern __inline__ unsigned long getipl(void)
{
	unsigned long retval;

	__asm__ __volatile__("rd	%%psr, %0" : "=r" (retval));
	return retval;
}

extern __inline__ unsigned long swap_pil(unsigned long __new_psr)
{
	unsigned long retval, tmp1, tmp2;

	__asm__ __volatile__("
		rd	%%psr, %0
		and	%0, %4, %1
		and	%3, %4, %2
		xorcc	%1, %2, %%g0
		be	1f
		 nop
		wr %0, %4, %%psr
		nop
		nop
		nop
1:
"		: "=r" (retval), "=r" (tmp1), "=r" (tmp2)
		: "r" (__new_psr), "i" (PSR_PIL)
		: "memory", "cc");

	return retval;
}

extern __inline__ unsigned long read_psr_and_cli(void)
{
	unsigned long retval;

	__asm__ __volatile__("
		rd	%%psr, %0
		andcc	%0, %1, %%g0
		bne	1f
		 nop
		wr	%0, %1, %%psr
		nop
		nop
		nop
1:
"		: "=r" (retval)
		: "i" (PSR_PIL)
		: "memory");

	return retval;
}

extern char spdeb_buf[256];

#define __save_flags(flags)	((flags) = getipl())
#define __save_and_cli(flags)	((flags) = read_psr_and_cli())
#define __restore_flags(flags)	setipl((flags))

#ifdef __SMP__

extern void __global_cli(void);
extern void __global_sti(void);
extern unsigned long __global_save_flags(void);
extern void __global_restore_flags(unsigned long);
#define cli() __global_cli()
#define sti() __global_sti()
#define save_flags(x)	  ((x)=__global_save_flags())
#define restore_flags(x)  __global_restore_flags(x)
#define save_and_cli(x)   do { (x)=__global_save_flags(); __global_cli(); } while(0)

#else

#define cli() __cli()
#define sti() __sti()
#define save_flags(x) __save_flags(x)
#define restore_flags(x) __restore_flags(x)
#define save_and_cli(x) __save_and_cli(x)

#endif

/* XXX Change this if we ever use a PSO mode kernel. */
#define mb()  __asm__ __volatile__ ("" : : : "memory")

#define nop() __asm__ __volatile__ ("nop");

extern __inline__ unsigned long xchg_u32(__volatile__ unsigned long *m, unsigned long val)
{
	register unsigned long *ptr asm("g1");
	register unsigned long ret asm("g2");

	ptr = (unsigned long *) m;
	ret = val;

	__asm__ __volatile__("
	mov	%%o7, %%g4
	call	___xchg32
	 add	%%o7, 8, %%o7
"	: "=&r" (ret)
	: "0" (ret), "r" (ptr)
	: "g3", "g4", "g7", "memory", "cc");

	return ret;
}

#define xchg(ptr,x) ((__typeof__(*(ptr)))__xchg((unsigned long)(x),(ptr),sizeof(*(ptr))))
#define tas(ptr) (xchg((ptr),1))

extern void __xchg_called_with_bad_pointer(void);

static __inline__ unsigned long __xchg(unsigned long x, __volatile__ void * ptr, int size)
{
	switch (size) {
	case 4:
		return xchg_u32(ptr, x);
	};
	__xchg_called_with_bad_pointer();
	return x;
}

extern void die_if_kernel(char *str, struct pt_regs *regs) __attribute__ ((noreturn));

#endif /* __KERNEL__ */

#endif /* __ASSEMBLY__ */

#endif /* !(__SPARC_SYSTEM_H) */
