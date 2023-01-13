/*
 * include/asm-mips/uaccess.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997 by Ralf Baechle
 *
 * $Id: uaccess.h,v 1.5 1997/12/01 16:44:08 ralf Exp $
 */
#ifndef __ASM_MIPS_UACCESS_H
#define __ASM_MIPS_UACCESS_H

#include <linux/errno.h>
#include <linux/sched.h>
#include <asm/asm.h>

#define STR(x)  __STR(x)
#define __STR(x)  #x

/*
 * The fs value determines whether argument validity checking should be
 * performed or not.  If get_fs() == USER_DS, checking is performed, with
 * get_fs() == KERNEL_DS, checking is bypassed.
 *
 * For historical reasons, these macros are grossly misnamed.
 */
#define KERNEL_DS 0
#define USER_DS 1

#define VERIFY_READ    0
#define VERIFY_WRITE   1

extern int active_ds;

#define get_fs()        active_ds
#define set_fs(x)       (active_ds=(x))

static inline unsigned long get_ds(void)
{
	return KERNEL_DS;
}

/*
 * Is a address valid? This does a straighforward calculation rather
 * than tests.
 *
 * Address valid if:
 *  - "addr" doesn't have any high-bits set
 *  - AND "size" doesn't have any high-bits set
 *  - AND "addr+size" doesn't have any high-bits set
 *  - OR we are in kernel mode.
 */
#define __access_ok(addr,size,mask) \
	(((__signed__ long)((mask)&(addr | size | (addr+size)))) >= 0)
#define __access_mask (-(long)get_fs())

#define access_ok(type,addr,size) \
__access_ok(((unsigned long)(addr)),(size),__access_mask)

extern inline int verify_area(int type, const void * addr, unsigned long size)
{
	return access_ok(type,addr,size) ? 0 : -EFAULT;
}

/*
 * Uh, these should become the main single-value transfer routines ...
 * They automatically use the right size if we just have the right
 * pointer type ...
 *
 * As MIPS uses the same address space for kernel and user data, we
 * can just do these as direct assignments.
 *
 * Careful to not
 * (a) re-use the arguments for side effects (sizeof is ok)
 * (b) require any knowledge of processes at this stage
 */
#define put_user(x,ptr)	__put_user_check((x),(ptr),sizeof(*(ptr)),__access_mask)
#define get_user(x,ptr) __get_user_check((x),(ptr),sizeof(*(ptr)),__access_mask)

/*
 * The "__xxx" versions do not do address space checking, useful when
 * doing multiple accesses to the same area (the user has to do the
 * checks by hand with "access_ok()")
 */
#define __put_user(x,ptr) __put_user_nocheck((x),(ptr),sizeof(*(ptr)))
#define __get_user(x,ptr) __get_user_nocheck((x),(ptr),sizeof(*(ptr)))

/*
 * The "xxx_ret" versions return constant specified in third argument, if
 * something bad happens. These macros can be optimized for the
 * case of just returning from the function xxx_ret is used.
 */

#define put_user_ret(x,ptr,ret) ({ \
if (put_user(x,ptr)) return ret; })

#define get_user_ret(x,ptr,ret) ({ \
if (get_user(x,ptr)) return ret; })

#define __put_user_ret(x,ptr,ret) ({ \
if (__put_user(x,ptr)) return ret; })

#define __get_user_ret(x,ptr,ret) ({ \
if (__get_user(x,ptr)) return ret; })

struct __large_struct { unsigned long buf[100]; };
#define __m(x) (*(struct __large_struct *)(x))

#define copy_to_user(to,from,n)   __copy_tofrom_user((to),(from),(n),__cu_to)
#define copy_from_user(to,from,n) __copy_tofrom_user((to),(from),(n),__cu_from)

extern size_t __copy_user(void *__to, const void *__from, size_t __n);

#define __copy_tofrom_user(to,from,n,v) ({ \
	void * __cu_to; \
	const void * __cu_from; \
	long __cu_len; \
	\
	__cu_to = (to); \
	__cu_from = (from); \
	__cu_len = (n); \
	if (__access_ok(((unsigned long)(v)),__cu_len,__access_mask)) \
		__cu_len = __copy_user(__cu_to, __cu_from, __cu_len); \
	__cu_len; \
})

/*
 * Yuck.  We need two variants, one for 64bit operation and one
 * for 32 bit mode and old iron.
 */
#ifdef __mips64
#define __GET_USER_DW __get_user_asm("ld")
#else
#define __GET_USER_DW __get_user_asm_ll32
#endif

#define __get_user_nocheck(x,ptr,size) ({ \
long __gu_err; \
__typeof(*(ptr)) __gu_val; \
long __gu_addr; \
__asm__("":"=r" (__gu_val)); \
__gu_addr = (long) (ptr); \
__asm__("":"=r" (__gu_err)); \
switch (size) { \
case 1: __get_user_asm("lb"); break; \
case 2: __get_user_asm("lh"); break; \
case 4: __get_user_asm("lw"); break; \
case 8: __GET_USER_DW; break; \
default: __get_user_unknown(); break; \
} x = (__typeof__(*(ptr))) __gu_val; __gu_err; })

#define __get_user_check(x,ptr,size,mask) ({ \
long __gu_err; \
__typeof__(*(ptr)) __gu_val; \
long __gu_addr; \
__asm__("":"=r" (__gu_val)); \
__gu_addr = (long) (ptr); \
__asm__("":"=r" (__gu_err)); \
if (__access_ok(__gu_addr,size,mask)) { \
switch (size) { \
case 1: __get_user_asm("lb"); break; \
case 2: __get_user_asm("lh"); break; \
case 4: __get_user_asm("lw"); break; \
case 8: __GET_USER_DW; break; \
default: __get_user_unknown(); break; \
} } x = (__typeof__(*(ptr))) __gu_val; __gu_err; })

#define __get_user_asm(insn) \
({ \
__asm__ __volatile__( \
	"1:\t" insn "\t%1,%2\n\t" \
	"move\t%0,$0\n" \
	"2:\n\t" \
	".section\t.fixup,\"ax\"\n" \
	"3:\t.set\tnoat\n\t" \
	"la\t$1,2b\n\t" \
	"li\t%0,%3\n\t" \
	"jr\t$1\n\t" \
	".set\tat\n\t" \
	".previous\n\t" \
	".section\t__ex_table,\"a\"\n\t" \
	STR(PTR)"\t1b,3b\n\t" \
	".previous" \
	:"=r" (__gu_err), "=r" (__gu_val) \
	:"o" (__m(__gu_addr)), "i" (-EFAULT) \
	:"$1"); })

/*
 * Get a long long 64 using 32 bit registers.
 */
#define __get_user_asm_ll32 \
({ \
__asm__ __volatile__( \
	"1:\tlw\t%1,%2\n" \
	"2:\tlw\t%D1,%3\n\t" \
	"move\t%0,$0\n" \
	"3:\t.section\t.fixup,\"ax\"\n" \
	"4:\t.set\tnoat\n\t" \
	"la\t$1,3b\n\t" \
	"li\t%0,%4\n\t" \
	"jr\t$1\n\t" \
	".set\tat\n\t" \
	".previous\n\t" \
	".section\t__ex_table,\"a\"\n\t" \
	STR(PTR)"\t1b,4b\n\t" \
	STR(PTR)"\t2b,4b\n\t" \
	".previous" \
	:"=r" (__gu_err), "=&r" (__gu_val) \
	:"o" (__m(__gu_addr)), "o" (__m(__gu_addr + 4)), \
	 "i" (-EFAULT) \
	:"$1"); })

extern void __get_user_unknown(void);

/*
 * Yuck.  We need two variants, one for 64bit operation and one
 * for 32 bit mode and old iron.
 */
#ifdef __mips64
#define __PUT_USER_DW __put_user_asm("sd")
#else
#define __PUT_USER_DW __put_user_asm_ll32
#endif

#define __put_user_nocheck(x,ptr,size) ({ \
long __pu_err; \
__typeof__(*(ptr)) __pu_val; \
long __pu_addr; \
__pu_val = (x); \
__pu_addr = (long) (ptr); \
__asm__("":"=r" (__pu_err)); \
switch (size) { \
case 1: __put_user_asm("sb"); break; \
case 2: __put_user_asm("sh"); break; \
case 4: __put_user_asm("sw"); break; \
case 8: __PUT_USER_DW; break; \
default: __put_user_unknown(); break; \
} __pu_err; })

#define __put_user_check(x,ptr,size,mask) ({ \
long __pu_err; \
__typeof__(*(ptr)) __pu_val; \
long __pu_addr; \
__pu_val = (x); \
__pu_addr = (long) (ptr); \
__asm__("":"=r" (__pu_err)); \
if (__access_ok(__pu_addr,size,mask)) { \
switch (size) { \
case 1: __put_user_asm("sb"); break; \
case 2: __put_user_asm("sh"); break; \
case 4: __put_user_asm("sw"); break; \
case 8: __PUT_USER_DW; break; \
default: __put_user_unknown(); break; \
} } __pu_err; })

#define __put_user_asm(insn) \
({ \
__asm__ __volatile__( \
	"1:\t" insn "\t%1,%2\n\t" \
	"move\t%0,$0\n" \
	"2:\n\t" \
	".section\t.fixup,\"ax\"\n" \
	"3:\t.set\tnoat\n\t" \
	"la\t$1,2b\n\t" \
	"li\t%0,%3\n\t" \
	"jr\t$1\n\t" \
	".set\tat\n\t" \
	".previous\n\t" \
	".section\t__ex_table,\"a\"\n\t" \
	STR(PTR)"\t1b,3b\n\t" \
	".previous" \
	:"=r" (__pu_err) \
	:"r" (__pu_val), "o" (__m(__pu_addr)), "i" (-EFAULT) \
	:"$1"); })

#define __put_user_asm_ll32 \
({ \
__asm__ __volatile__( \
	"1:\tsw\t%1,%2\n\t" \
	"2:\tsw\t%D1,%3\n" \
	"move\t%0,$0\n" \
	"3:\n\t" \
	".section\t.fixup,\"ax\"\n" \
	"4:\t.set\tnoat\n\t" \
	"la\t$1,3b\n\t" \
	"li\t%0,%4\n\t" \
	"jr\t$1\n\t" \
	".set\tat\n\t" \
	".previous\n\t" \
	".section\t__ex_table,\"a\"\n\t" \
	STR(PTR)"\t1b,4b\n\t" \
	STR(PTR)"\t2b,4b\n\t" \
	".previous" \
	:"=r" (__pu_err) \
	:"r" (__pu_val), "o" (__m(__pu_addr)), "o" (__m(__pu_addr + 4)), \
	 "i" (-EFAULT) \
	:"$1"); })

extern void __put_user_unknown(void);

#define copy_to_user_ret(to,from,n,retval) ({ \
if (copy_to_user(to,from,n)) \
        return retval; \
})

#define copy_from_user_ret(to,from,n,retval) ({ \
if (copy_from_user(to,from,n)) \
        return retval; \
})

#define __copy_to_user(to,from,n)                       \
         __copy_user((to),(from),(n))

#define __copy_from_user(to,from,n)                     \
         __copy_user((to),(from),(n))

#define __clear_user(addr,size) \
({ \
	void *__cu_end; \
	__asm__ __volatile__( \
		".set\tnoreorder\n\t" \
		"1:\taddiu\t%0,1\n" \
		"bne\t%0,%1,1b\n\t" \
		"sb\t$0,-1(%0)\n\t" \
		"2:\t.set\treorder\n\t" \
		".section\t.fixup,\"ax\"\n" \
		"3:\t.set\tnoat\n\t" \
		"subu\t%0,1\n\t" \
		"j\t2b\n\t" \
		".set\tat\n\t" \
		".previous\n\t" \
		".section\t__ex_table,\"a\"\n\t" \
		STR(PTR)"\t1b,3b\n\t" \
		".previous" \
		:"=r" (addr), "=r" (__cu_end) \
		:"0" (addr), "1" (addr + size), "i" (-EFAULT) \
		:"$1","memory"); \
		size = __cu_end - (addr); \
})

#define clear_user(addr,n) ({ \
void * __cl_addr = (addr); \
unsigned long __cl_size = (n); \
if (__cl_size && __access_ok(VERIFY_WRITE, ((unsigned long)(__cl_addr)), __cl_size)) \
__clear_user(__cl_addr, __cl_size); \
__cl_size; })

/*
 * Returns: -EFAULT if exception before terminator, N if the entire
 * buffer filled, else strlen.
 */
extern long __strncpy_from_user(char *__to, const char *__from, long __to_len);

#define strncpy_from_user(dest,src,count) ({                            \
	const void * __sc_src = (src);                                  \
	long __sc_res = -EFAULT;                                        \
	if (access_ok(VERIFY_READ, __sc_src, 0)) {                      \
		__sc_res = __strncpy_from_user(dest, __sc_src, count);  \
} __sc_res; })

/* Returns: 0 if bad, string length+1 (memory size) of string if ok */
extern long __strlen_user(const char *);

extern inline long strlen_user(const char *str)
{
	return access_ok(VERIFY_READ,str,0) ? __strlen_user(str) : 0;
}

struct exception_table_entry
{
	unsigned long insn;
	unsigned long nextinsn;
};

/* Returns 0 if exception not found and fixup.unit otherwise.  */
extern unsigned long search_exception_table(unsigned long addr);

/* Returns the new pc */
#define fixup_exception(map_reg, fixup_unit, pc)                \
({                                                              \
	fixup_unit;                                             \
})

#endif /* __ASM_MIPS_UACCESS_H */
