#ifndef __ASM_SH_SMPLOCK_H
#define __ASM_SH_SMPLOCK_H

#ifndef __SMP__

#define lock_kernel()				do { } while(0)
#define unlock_kernel()				do { } while(0)
#define release_kernel_lock(task, cpu, depth)	((depth) = 1)
#define reacquire_kernel_lock(task, cpu, depth)	do { } while(0)

#else

#error "We do not support SMP on SH yet"

#endif /* __SMP__ */

#endif /* __ASM_SH_SMPLOCK_H */
