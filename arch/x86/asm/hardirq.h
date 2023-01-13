#ifndef __ASM_HARDIRQ_H
#define __ASM_HARDIRQ_H

#include <linux/tasks.h>

extern unsigned int local_irq_count[NR_CPUS];
#define in_interrupt()	(local_irq_count[smp_processor_id()] != 0)

#ifndef __SMP__

#define hardirq_trylock(cpu)	(local_irq_count[cpu] == 0)
#define hardirq_endlock(cpu)	do { } while (0)

#define hardirq_enter(cpu)	(local_irq_count[cpu]++)
#define hardirq_exit(cpu)	(local_irq_count[cpu]--)

#define synchronize_irq()	do { } while (0)

#else

#include <asm/atomic.h>

extern unsigned char global_irq_holder;
extern unsigned volatile int global_irq_lock;
extern atomic_t global_irq_count;

static inline void release_irqlock(int cpu)
{
	/* if we didn't own the irq lock, just ignore.. */
	if (global_irq_holder == (unsigned char) cpu) {
		global_irq_holder = NO_PROC_ID;
		global_irq_lock = 0;
	}
}

static inline void hardirq_enter(int cpu)
{
	++local_irq_count[cpu];
	atomic_inc(&global_irq_count);
}

static inline void hardirq_exit(int cpu)
{
	atomic_dec(&global_irq_count);
	--local_irq_count[cpu];
}

static inline int hardirq_trylock(int cpu)
{
	unsigned long flags;

	__save_flags(flags);
	__cli();
	atomic_inc(&global_irq_count);
	if (atomic_read(&global_irq_count) != 1 || test_bit(0,&global_irq_lock)) {
		atomic_dec(&global_irq_count);
		__restore_flags(flags);
		return 0;
	}
	++local_irq_count[cpu];
	return 1;
}

static inline void hardirq_endlock(int cpu)
{
	__cli();
	hardirq_exit(cpu);
	__sti();
}

extern void synchronize_irq(void);

#endif /* __SMP__ */

#endif /* __ASM_HARDIRQ_H */
