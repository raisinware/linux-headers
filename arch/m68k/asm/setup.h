/*
** asm/setup.h -- Definition of the Linux/m68k setup information
**
** Copyright 1992 by Greg Harp
**
** This file is subject to the terms and conditions of the GNU General Public
** License.  See the file COPYING in the main directory of this archive
** for more details.
**
** Created 09/29/92 by Greg Harp
**
** 5/2/94 Roman Hodek:
**   Added bi_atari part of the machine dependent union bi_un; for now it
**   contains just a model field to distinguish between TT and Falcon.
** 26/7/96 Roman Zippel:
**   Renamed to setup.h; added some useful macros to allow gcc some
**   optimizations if possible.
** 5/10/96 Geert Uytterhoeven:
**   Redesign of the boot information structure; moved boot information
**   structure to bootinfo.h
*/

#ifndef _M68K_SETUP_H
#define _M68K_SETUP_H

#include <linux/config.h>


    /*
     *  Linux/m68k Architectures
     */

#define MACH_AMIGA    1
#define MACH_ATARI    2
#define MACH_MAC      3
#define MACH_APOLLO   4
#define MACH_SUN3     5
#define MACH_MVME147  6
#define MACH_MVME16x  7
#define MACH_BVME6000 8
#define MACH_HP300    9

#ifdef __KERNEL__

#ifndef __ASSEMBLY__
extern u_long m68k_machtype;
#endif /* !__ASSEMBLY__ */

#if !defined(CONFIG_AMIGA)
#  define MACH_IS_AMIGA (0)
#elif defined(CONFIG_ATARI) || defined(CONFIG_MAC) || defined(CONFIG_APOLLO) \
	|| defined(CONFIG_MVME16x) || defined(CONFIG_BVME6000) || defined(CONFIG_HP300)
#  define MACH_IS_AMIGA (m68k_machtype == MACH_AMIGA)
#else
#  define CONFIG_AMIGA_ONLY
#  define MACH_IS_AMIGA (1)
#  define MACH_TYPE (MACH_AMIGA)
#endif

#if !defined(CONFIG_ATARI)
#  define MACH_IS_ATARI (0)
#elif defined(CONFIG_AMIGA) || defined(CONFIG_MAC) || defined(CONFIG_APOLLO) \
	|| defined(CONFIG_MVME16x) || defined(CONFIG_BVME6000) || defined(CONFIG_HP300)
#  define MACH_IS_ATARI (m68k_machtype == MACH_ATARI)
#else
#  define CONFIG_ATARI_ONLY
#  define MACH_IS_ATARI (1)
#  define MACH_TYPE (MACH_ATARI)
#endif

#if !defined(CONFIG_MAC)
#  define MACH_IS_MAC (0)
#elif defined(CONFIG_AMIGA) || defined(CONFIG_ATARI) || defined(CONFIG_APOLLO) \
	|| defined(CONFIG_HP300) || defined(CONFIG_MVME16x) || defined(CONFIG_BVME6000)
#  define MACH_IS_MAC (m68k_machtype == MACH_MAC)
#else
#  define CONFIG_MAC_ONLY
#  define MACH_IS_MAC (1)
#  define MACH_TYPE (MACH_MAC)
#endif

#if defined(CONFIG_SUN3)
#  error Currently no Sun-3 support!
#else
#define MACH_IS_SUN3 (0)
#endif

#if !defined (CONFIG_APOLLO)
#  define MACH_IS_APOLLO (0)
#elif defined(CONFIG_AMIGA) || defined(CONFIG_MAC) || defined(CONFIG_ATARI) \
	|| defined(CONFIG_MVME16x) || defined(CONFIG_BVME6000) || defined(CONFIG_HP300)
#  define MACH_IS_APOLLO (m68k_machtype == MACH_APOLLO)
#else
#  define CONFIG_APOLLO_ONLY
#  define MACH_IS_APOLLO (1)
#  define MACH_TYPE (MACH_APOLLO)
#endif

#if !defined (CONFIG_MVME16x)
#  define MACH_IS_MVME16x (0)
#elif defined(CONFIG_AMIGA) || defined(CONFIG_MAC) || defined(CONFIG_ATARI) \
	|| defined(CONFIG_APOLLO) || defined(CONFIG_BVME6000) || defined(CONFIG_HP300)
#  define MACH_IS_MVME16x (m68k_machtype == MACH_MVME16x)
#else
#  define CONFIG_MVME16x_ONLY
#  define MACH_IS_MVME16x (1)
#  define MACH_TYPE (MACH_MVME16x)
#endif

#if !defined (CONFIG_BVME6000)
#  define MACH_IS_BVME6000 (0)
#elif defined(CONFIG_AMIGA) || defined(CONFIG_MAC) || defined(CONFIG_ATARI) \
	|| defined(CONFIG_APOLLO) || defined(CONFIG_MVME16x) || defined(CONFIG_HP300)
#  define MACH_IS_BVME6000 (m68k_machtype == MACH_BVME6000)
#else
#  define CONFIG_BVME6000_ONLY
#  define MACH_IS_BVME6000 (1)
#  define MACH_TYPE (MACH_BVME6000)
#endif

#if !defined (CONFIG_HP300)
#  define MACH_IS_HP300 (0)
#elif defined(CONFIG_AMIGA) || defined(CONFIG_MAC) || defined(CONFIG_ATARI) \
	|| defined(CONFIG_APOLLO) || defined(CONFIG_MVME16x) || defined(CONFIG_BVME6000)
#  define MAC_IS_HP300 (m68k_machtype == MACH_HP300)
#else
#  define CONFIG_HP300_ONLY
#  define MACH_IS_HP300 (1)
#  define MACH_TYPE (MACH_HP300)
#endif

#ifndef MACH_TYPE
#  define MACH_TYPE (m68k_machtype)
#endif

#endif /* __KERNEL__ */


    /*
     *  CPU, FPU and MMU types
     *
     *  Note: we may rely on the following equalities:
     *
     *      CPU_68020 == MMU_68851
     *      CPU_68030 == MMU_68030
     *      CPU_68040 == FPU_68040 == MMU_68040
     *      CPU_68060 == FPU_68060 == MMU_68060
     */

#define CPUB_68020     0
#define CPUB_68030     1
#define CPUB_68040     2
#define CPUB_68060     3

#define CPU_68020      (1<<CPUB_68020)
#define CPU_68030      (1<<CPUB_68030)
#define CPU_68040      (1<<CPUB_68040)
#define CPU_68060      (1<<CPUB_68060)

#define FPUB_68881     0
#define FPUB_68882     1
#define FPUB_68040     2                       /* Internal FPU */
#define FPUB_68060     3                       /* Internal FPU */
#define FPUB_SUNFPA    4                       /* Sun-3 FPA */

#define FPU_68881      (1<<FPUB_68881)
#define FPU_68882      (1<<FPUB_68882)
#define FPU_68040      (1<<FPUB_68040)
#define FPU_68060      (1<<FPUB_68060)
#define FPU_SUNFPA     (1<<FPUB_SUNFPA)

#define MMUB_68851     0
#define MMUB_68030     1                       /* Internal MMU */
#define MMUB_68040     2                       /* Internal MMU */
#define MMUB_68060     3                       /* Internal MMU */
#define MMUB_APOLLO    4                       /* Custom Apollo */
#define MMUB_SUN3      5                       /* Custom Sun-3 */

#define MMU_68851      (1<<MMUB_68851)
#define MMU_68030      (1<<MMUB_68030)
#define MMU_68040      (1<<MMUB_68040)
#define MMU_68060      (1<<MMUB_68060)
#define MMU_SUN3       (1<<MMUB_SUN3)
#define MMU_APOLLO     (1<<MMUB_APOLLO)

#ifdef __KERNEL__

#ifndef __ASSEMBLY__
extern u_long m68k_cputype;
extern u_long m68k_fputype;
extern u_long m68k_mmutype;			/* Not really used yet */

    /*
     *  m68k_is040or060 is != 0 for a '040 or higher;
     *  used numbers are 4 for 68040 and 6 for 68060.
     */

extern int m68k_is040or060;
#endif /* !__ASSEMBLY__ */

#if !defined(CONFIG_M68020)
#  define CPU_IS_020 (0)
#elif defined(CONFIG_M68030) || defined(CONFIG_M68040) || defined(CONFIG_M68060)
#  define CPU_IS_020 (m68k_cputype & CPU_68020)
#else
#  define CPU_M68020_ONLY
#  define CPU_IS_020 (1)
#endif

#if !defined(CONFIG_M68030)
#  define CPU_IS_030 (0)
#elif defined(CONFIG_M68020) || defined(CONFIG_M68040) || defined(CONFIG_M68060)
#  define CPU_IS_030 (m68k_cputype & CPU_68030)
#else
#  define CPU_M68030_ONLY
#  define CPU_IS_030 (1)
#endif

#if !defined(CONFIG_M68040)
#  define CPU_IS_040 (0)
#elif defined(CONFIG_M68020) || defined(CONFIG_M68030) || defined(CONFIG_M68060)
#  define CPU_IS_040 (m68k_cputype & CPU_68040)
#else
#  define CPU_M68040_ONLY
#  define CPU_IS_040 (1)
#endif

#if !defined(CONFIG_M68060)
#  define CPU_IS_060 (0)
#elif defined(CONFIG_M68020) || defined(CONFIG_M68030) || defined(CONFIG_M68040)
#  define CPU_IS_060 (m68k_cputype & CPU_68060)
#else
#  define CPU_M68060_ONLY
#  define CPU_IS_060 (1)
#endif

#if !defined(CONFIG_M68020) && !defined(CONFIG_M68030)
#  define CPU_IS_020_OR_030 (0)
#else
#  define CPU_M68020_OR_M68030
#  if defined(CONFIG_M68040) || defined(CONFIG_M68060)
#    define CPU_IS_020_OR_030 (!m68k_is040or060)
#  else
#    define CPU_M68020_OR_M68030_ONLY
#    define CPU_IS_020_OR_030 (1)
#  endif
#endif

#if !defined(CONFIG_M68040) && !defined(CONFIG_M68060)
#  define CPU_IS_040_OR_060 (0)
#else
#  define CPU_M68040_OR_M68060
#  if defined(CONFIG_M68020) || defined(CONFIG_M68030)
#    define CPU_IS_040_OR_060 (m68k_is040or060)
#  else
#    define CPU_M68040_OR_M68060_ONLY
#    define CPU_IS_040_OR_060 (1)
#  endif
#endif

#define CPU_TYPE (m68k_cputype)

#endif /* __KERNEL__ */


    /*
     *  Miscellaneous
     */

#define NUM_MEMINFO	(4)
#define CL_SIZE		(256)

#ifndef __ASSEMBLY__

struct mem_info {
	unsigned long addr;		/* physical address of memory chunk */
	unsigned long size;		/* length of memory chunk (in bytes) */
};

#ifdef __KERNEL__
extern int m68k_num_memory;		/* # of memory blocks found */
extern struct mem_info m68k_memory[NUM_MEMINFO];/* memory description */
#endif /* __KERNEL__ */

#endif /* !__ASSEMBLY__ */


#endif /* _M68K_SETUP_H */
