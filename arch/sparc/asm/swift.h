/* swift.h: Specific definitions for the _broken_ Swift SRMMU
 *          MMU.
 *
 * Copyright (C) 1996 David S. Miller (davem@caip.rutgers.edu)
 */

#ifndef _SPARC_SWIFT_H
#define _SPARC_SWIFT_H

/* Swift is so brain damaged, here is the mmu control register. */
#define SWIFT_ST       0x00800000   /* SW tablewalk enable */
#define SWIFT_WP       0x00400000   /* Watchpoint enable   */

/* Branch folding (buggy, disable on production systems!)  */
#define SWIFT_BF       0x00200000
#define SWIFT_PMC      0x00180000   /* Page mode control   */
#define SWIFT_PE       0x00040000   /* Parity enable       */
#define SWIFT_PC       0x00020000   /* Parity control      */
#define SWIFT_AP       0x00010000   /* Graphics page mode control (TCX/SX) */
#define SWIFT_AC       0x00008000   /* Alternate Cacheability (see viking.h) */
#define SWIFT_BM       0x00004000   /* Boot mode */
#define SWIFT_RC       0x00003c00   /* DRAM refresh control */
#define SWIFT_IE       0x00000200   /* Instruction cache enable */
#define SWIFT_DE       0x00000100   /* Data cache enable */
#define SWIFT_SA       0x00000080   /* Store Allocate */
#define SWIFT_NF       0x00000002   /* No fault mode */
#define SWIFT_EN       0x00000001   /* MMU enable */

extern inline void swift_inv_insn_tag(unsigned long addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_TXTC_TAG));
}

extern inline void swift_inv_data_tag(unsigned long addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_DATAC_TAG));
}

extern inline void swift_flush_page(unsigned long page)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (page), "i" (ASI_M_FLUSH_PAGE));
}

extern inline void swift_flush_segment(unsigned long addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_FLUSH_SEG));
}

extern inline void swift_flush_region(unsigned long addr)
{
	__asm__ __volatile__("sta %%g0, [%0] %1\n\t" : :
			     "r" (addr), "i" (ASI_M_FLUSH_REGION));
}

extern inline void swift_flush_context(void)
{
	__asm__ __volatile__("sta %%g0, [%%g0] %0\n\t" : :
			     "i" (ASI_M_FLUSH_CTX));
}

#endif /* !(_SPARC_SWIFT_H) */
