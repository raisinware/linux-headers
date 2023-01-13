#ifndef __ALPHA_T2__H__
#define __ALPHA_T2__H__

#include <linux/config.h>
#include <linux/types.h>

/*
 * T2 is the internal name for the core logic chipset which provides
 * memory controller and PCI access for the SABLE-based systems.
 *
 * This file is based on:
 *
 * SABLE I/O Specification
 * Revision/Update Information: 1.3
 *
 * jestabro@amt.tay1.dec.com Initial Version.
 *
 */

#define BYTE_ENABLE_SHIFT 5
#define TRANSFER_LENGTH_SHIFT 3
#define MEM_R1_MASK 0x03ffffff  /* Mem sparse space region 1 mask is 26 bits */

#ifdef CONFIG_ALPHA_SRM_SETUP
/* if we are using the SRM PCI setup, we'll need to use variables instead */
#define T2_DMA_WIN_BASE_DEFAULT    (1024*1024*1024)
#define T2_DMA_WIN_SIZE_DEFAULT    (1024*1024*1024)

extern unsigned int T2_DMA_WIN_BASE;
extern unsigned int T2_DMA_WIN_SIZE;

#else /* SRM_SETUP */
#define T2_DMA_WIN_BASE	(1024*1024*1024)
#define T2_DMA_WIN_SIZE	(1024*1024*1024)
#endif /* SRM_SETUP */

/* GAMMA-SABLE is a SABLE with EV5-based CPUs */
#ifdef CONFIG_ALPHA_GAMMA
#  define GAMMA_BIAS		0x8000000000UL
#else /* GAMMA */
#  define GAMMA_BIAS		0x0000000000UL
#endif /* GAMMA */

/*
 * Memory spaces:
 */
#define T2_CONF		        (IDENT_ADDR + GAMMA_BIAS + 0x390000000UL)
#define T2_IO			(IDENT_ADDR + GAMMA_BIAS + 0x3a0000000UL)
#define T2_SPARSE_MEM		(IDENT_ADDR + GAMMA_BIAS + 0x200000000UL)
#define T2_DENSE_MEM	        (IDENT_ADDR + GAMMA_BIAS + 0x3c0000000UL)

#define T2_IOCSR		(IDENT_ADDR + GAMMA_BIAS + 0x38e000000UL)
#define T2_CERR1		(IDENT_ADDR + GAMMA_BIAS + 0x38e000020UL)
#define T2_CERR2		(IDENT_ADDR + GAMMA_BIAS + 0x38e000040UL)
#define T2_CERR3		(IDENT_ADDR + GAMMA_BIAS + 0x38e000060UL)
#define T2_PERR1		(IDENT_ADDR + GAMMA_BIAS + 0x38e000080UL)
#define T2_PERR2		(IDENT_ADDR + GAMMA_BIAS + 0x38e0000a0UL)
#define T2_PSCR			(IDENT_ADDR + GAMMA_BIAS + 0x38e0000c0UL)
#define T2_HAE_1		(IDENT_ADDR + GAMMA_BIAS + 0x38e0000e0UL)
#define T2_HAE_2		(IDENT_ADDR + GAMMA_BIAS + 0x38e000100UL)
#define T2_HBASE		(IDENT_ADDR + GAMMA_BIAS + 0x38e000120UL)
#define T2_WBASE1		(IDENT_ADDR + GAMMA_BIAS + 0x38e000140UL)
#define T2_WMASK1		(IDENT_ADDR + GAMMA_BIAS + 0x38e000160UL)
#define T2_TBASE1		(IDENT_ADDR + GAMMA_BIAS + 0x38e000180UL)
#define T2_WBASE2		(IDENT_ADDR + GAMMA_BIAS + 0x38e0001a0UL)
#define T2_WMASK2		(IDENT_ADDR + GAMMA_BIAS + 0x38e0001c0UL)
#define T2_TBASE2		(IDENT_ADDR + GAMMA_BIAS + 0x38e0001e0UL)
#define T2_TLBBR		(IDENT_ADDR + GAMMA_BIAS + 0x38e000200UL)

#define T2_HAE_3		(IDENT_ADDR + GAMMA_BIAS + 0x38e000240UL)
#define T2_HAE_4		(IDENT_ADDR + GAMMA_BIAS + 0x38e000260UL)

#define HAE_ADDRESS	                T2_HAE_1

/*  T2 CSRs are in the non-cachable primary IO space from 3.8000.0000 to
 3.8fff.ffff
 *
 *  +--------------+ 3 8000 0000
 *  | CPU 0 CSRs   |            
 *  +--------------+ 3 8100 0000
 *  | CPU 1 CSRs   |            
 *  +--------------+ 3 8200 0000
 *  | CPU 2 CSRs   |            
 *  +--------------+ 3 8300 0000
 *  | CPU 3 CSRs   |            
 *  +--------------+ 3 8400 0000
 *  | CPU Reserved |            
 *  +--------------+ 3 8700 0000
 *  | Mem Reserved |            
 *  +--------------+ 3 8800 0000
 *  | Mem 0 CSRs   |            
 *  +--------------+ 3 8900 0000
 *  | Mem 1 CSRs   |            
 *  +--------------+ 3 8a00 0000
 *  | Mem 2 CSRs   |            
 *  +--------------+ 3 8b00 0000
 *  | Mem 3 CSRs   |            
 *  +--------------+ 3 8c00 0000           
 *  | Mem Reserved |            
 *  +--------------+ 3 8e00 0000           
 *  | PCI Bridge   |            
 *  +--------------+ 3 8f00 0000           
 *  | Expansion IO |            
 *  +--------------+ 3 9000 0000           
 *                                              
 *
 */
#define CPU0_BASE               (IDENT_ADDR + GAMMA_BIAS + 0x380000000L)
#define CPU1_BASE               (IDENT_ADDR + GAMMA_BIAS + 0x381000000L)
#define CPU2_BASE               (IDENT_ADDR + GAMMA_BIAS + 0x382000000L)
#define CPU3_BASE               (IDENT_ADDR + GAMMA_BIAS + 0x383000000L)
#define MEM0_BASE               (IDENT_ADDR + GAMMA_BIAS + 0x388000000L)
#define MEM1_BASE               (IDENT_ADDR + GAMMA_BIAS + 0x389000000L)
#define MEM2_BASE               (IDENT_ADDR + GAMMA_BIAS + 0x38a000000L)
#define MEM3_BASE               (IDENT_ADDR + GAMMA_BIAS + 0x38b000000L)

#ifdef __KERNEL__

/*
 * Translate physical memory address as seen on (PCI) bus into
 * a kernel virtual address and vv.
 */
extern inline unsigned long virt_to_bus(void * address)
{
	return virt_to_phys(address) + T2_DMA_WIN_BASE;
}

extern inline void * bus_to_virt(unsigned long address)
{
	return phys_to_virt(address - T2_DMA_WIN_BASE);
}

/*
 * I/O functions:
 *
 * T2 (the core logic PCI/memory support chipset for the SABLE
 * series of processors uses a sparse address mapping scheme to
 * get at PCI memory and I/O.
 */

#define vuip	volatile unsigned int *

extern inline unsigned int __inb(unsigned long addr)
{
	long result = *(vuip) ((addr << 5) + T2_IO + 0x00);
	result >>= (addr & 3) * 8;
	return 0xffUL & result;
}

extern inline void __outb(unsigned char b, unsigned long addr)
{
	unsigned int w;

	w = __kernel_insbl(b, addr & 3);
	*(vuip) ((addr << 5) + T2_IO + 0x00) = w;
	mb();
}

extern inline unsigned int __inw(unsigned long addr)
{
	long result = *(vuip) ((addr << 5) + T2_IO + 0x08);
	result >>= (addr & 3) * 8;
	return 0xffffUL & result;
}

extern inline void __outw(unsigned short b, unsigned long addr)
{
	unsigned int w;

	w = __kernel_inswl(b, addr & 3);
	*(vuip) ((addr << 5) + T2_IO + 0x08) = w;
	mb();
}

extern inline unsigned int __inl(unsigned long addr)
{
	return *(vuip) ((addr << 5) + T2_IO + 0x18);
}

extern inline void __outl(unsigned int b, unsigned long addr)
{
	*(vuip) ((addr << 5) + T2_IO + 0x18) = b;
	mb();
}


/*
 * Memory functions.  64-bit and 32-bit accesses are done through
 * dense memory space, everything else through sparse space.
 * 
 * For reading and writing 8 and 16 bit quantities we need to 
 * go through one of the three sparse address mapping regions
 * and use the HAE_MEM CSR to provide some bits of the address.
 * The following few routines use only sparse address region 1
 * which gives 1Gbyte of accessible space which relates exactly
 * to the amount of PCI memory mapping *into* system address space.
 * See p 6-17 of the specification but it looks something like this:
 *
 * 21164 Address:
 * 
 *          3         2         1                                                               
 * 9876543210987654321098765432109876543210
 * 1ZZZZ0.PCI.QW.Address............BBLL                 
 *
 * ZZ = SBZ
 * BB = Byte offset
 * LL = Transfer length
 *
 * PCI Address:
 *
 * 3         2         1                                                               
 * 10987654321098765432109876543210
 * HHH....PCI.QW.Address........ 00
 *
 * HHH = 31:29 HAE_MEM CSR
 * 
 */
#ifdef CONFIG_ALPHA_SRM_SETUP

extern unsigned long t2_sm_base;

extern inline unsigned long __readb(unsigned long addr)
{
	unsigned long result, shift, work;

	if ((addr >= t2_sm_base) && (addr <= (t2_sm_base + MEM_R1_MASK)))
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x00);
	else
	if ((addr >= 512*1024) && (addr < 1024*1024)) /* check HOLE */
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x00);
	else
	{
#if 0
	  printk("__readb: address 0x%lx not covered by HAE\n", addr);
#endif
	  return 0x0ffUL;
	}
	shift = (addr & 0x3) << 3;
	result = *(vuip) work;
	result >>= shift;
	return 0x0ffUL & result;
}

extern inline unsigned long __readw(unsigned long addr)
{
	unsigned long result, shift, work;

	if ((addr >= t2_sm_base) && (addr <= (t2_sm_base + MEM_R1_MASK)))
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x08);
	else
	if ((addr >= 512*1024) && (addr < 1024*1024)) /* check HOLE */
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x08);
	else
	{
#if 0
	  printk("__readw: address 0x%lx not covered by HAE\n", addr);
#endif
	  return 0x0ffffUL;
	}
	shift = (addr & 0x3) << 3;
	result = *(vuip) work;
	result >>= shift;
	return 0x0ffffUL & result;
}

/* on SABLE with T2, we must use SPARSE memory even for 32-bit access */
extern inline unsigned long __readl(unsigned long addr)
{
	unsigned long result, work;

	if ((addr >= t2_sm_base) && (addr <= (t2_sm_base + MEM_R1_MASK)))
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x18);
	else
	if ((addr >= 512*1024) && (addr < 1024*1024)) /* check HOLE */
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x18);
	else
	{
#if 0
	  printk("__readl: address 0x%lx not covered by HAE\n", addr);
#endif
	  return 0x0ffffffffUL;
	}
	result = *(vuip) work;
	return 0xffffffffUL & result;
}

extern inline void __writeb(unsigned char b, unsigned long addr)
{
	unsigned long work;

	if ((addr >= t2_sm_base) && (addr <= (t2_sm_base + MEM_R1_MASK)))
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x00);
	else
	if ((addr >= 512*1024) && (addr < 1024*1024)) /* check HOLE */
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x00);
	else
	{
#if 0
	  printk("__writeb: address 0x%lx not covered by HAE\n", addr);
#endif
	  return;
	}
	*(vuip) work = b * 0x01010101;
}

extern inline void __writew(unsigned short b, unsigned long addr)
{
	unsigned long work;

	if ((addr >= t2_sm_base) && (addr <= (t2_sm_base + MEM_R1_MASK)))
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x08);
	else
	if ((addr >= 512*1024) && (addr < 1024*1024)) /* check HOLE */
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x08);
	else
	{
#if 0
	  printk("__writew: address 0x%lx not covered by HAE\n", addr);
#endif
	  return;
	}
	*(vuip) work = b * 0x00010001;
}

/* on SABLE with T2, we must use SPARSE memory even for 32-bit access */
extern inline void __writel(unsigned int b, unsigned long addr)
{
	unsigned long work;

	if ((addr >= t2_sm_base) && (addr <= (t2_sm_base + MEM_R1_MASK)))
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x18);
	else
	if ((addr >= 512*1024) && (addr < 1024*1024)) /* check HOLE */
	  work = (((addr & MEM_R1_MASK) << 5) + T2_SPARSE_MEM + 0x18);
	{
#if 0
	  printk("__writel: address 0x%lx not covered by HAE\n", addr);
#endif
	  return;
	}
	*(vuip) work = b;
}

#else /* SRM_SETUP */

extern inline unsigned long __readb(unsigned long addr)
{
	unsigned long result, shift, msb;

	shift = (addr & 0x3) * 8 ;
	msb = addr & 0xE0000000 ;
	addr &= MEM_R1_MASK ;
	if (msb != hae.cache) {
	  set_hae(msb);
	}
	result = *(vuip) ((addr << 5) + T2_SPARSE_MEM + 0x00) ;
	result >>= shift;
	return 0xffUL & result;
}

extern inline unsigned long __readw(unsigned long addr)
{
	unsigned long result, shift, msb;

	shift = (addr & 0x3) * 8;
	msb = addr & 0xE0000000 ;
	addr &= MEM_R1_MASK ;
	if (msb != hae.cache) {
	  set_hae(msb);
	}
	result = *(vuip) ((addr << 5) + T2_SPARSE_MEM + 0x08);
	result >>= shift;
	return 0xffffUL & result;
}

/* on SABLE with T2, we must use SPARSE memory even for 32-bit access */
extern inline unsigned long __readl(unsigned long addr)
{
	unsigned long result, msb;

	msb = addr & 0xE0000000 ;
	addr &= MEM_R1_MASK ;
	if (msb != hae.cache) {
	  set_hae(msb);
	}
	result = *(vuip) ((addr << 5) + T2_SPARSE_MEM + 0x18);
	return 0xffffffffUL & result;
}

extern inline void __writeb(unsigned char b, unsigned long addr)
{
        unsigned long msb ; 

	msb = addr & 0xE0000000 ;
	addr &= MEM_R1_MASK ;
	if (msb != hae.cache) {
	  set_hae(msb);
	}
	*(vuip) ((addr << 5) + T2_SPARSE_MEM + 0x00) = b * 0x01010101;
}

extern inline void __writew(unsigned short b, unsigned long addr)
{
        unsigned long msb ; 

	msb = addr & 0xE0000000 ;
	addr &= MEM_R1_MASK ;
	if (msb != hae.cache) {
	  set_hae(msb);
	}
	*(vuip) ((addr << 5) + T2_SPARSE_MEM + 0x08) = b * 0x00010001;
}

/* on SABLE with T2, we must use SPARSE memory even for 32-bit access */
extern inline void __writel(unsigned int b, unsigned long addr)
{
        unsigned long msb ; 

	msb = addr & 0xE0000000 ;
	addr &= MEM_R1_MASK ;
	if (msb != hae.cache) {
	  set_hae(msb);
	}
	*(vuip) ((addr << 5) + T2_SPARSE_MEM + 0x18) = b;
}

#endif /* SRM_SETUP */

#define inb(port) \
(__builtin_constant_p((port))?__inb(port):_inb(port))

#define outb(x, port) \
(__builtin_constant_p((port))?__outb((x),(port)):_outb((x),(port)))

#define readl(a)	__readl((unsigned long)(a))
#define writel(v,a)	__writel((v),(unsigned long)(a))

#undef vuip

extern unsigned long t2_init (unsigned long mem_start,
				 unsigned long mem_end);

#endif /* __KERNEL__ */

/*
 * Sable CPU Module CSRS
 *
 * These are CSRs for hardware other than the CPU chip on the CPU module.
 * The CPU module has Backup Cache control logic, Cbus control logic, and
 * interrupt control logic on it.  There is a duplicate tag store to speed
 * up maintaining cache coherency.
 */

struct sable_cpu_csr {
unsigned long bcc;     long fill_00[3]; /* Backup Cache Control */
unsigned long bcce;    long fill_01[3]; /* Backup Cache Correctable Error */
unsigned long bccea;   long fill_02[3]; /* B-Cache Corr Err Address Latch */
unsigned long bcue;    long fill_03[3]; /* B-Cache Uncorrectable Error */
unsigned long bcuea;   long fill_04[3]; /* B-Cache Uncorr Err Addr Latch */
unsigned long dter;    long fill_05[3]; /* Duplicate Tag Error */
unsigned long cbctl;   long fill_06[3]; /* CBus Control */
unsigned long cbe;     long fill_07[3]; /* CBus Error */
unsigned long cbeal;   long fill_08[3]; /* CBus Error Addr Latch low */
unsigned long cbeah;   long fill_09[3]; /* CBus Error Addr Latch high */
unsigned long pmbx;    long fill_10[3]; /* Processor Mailbox */
unsigned long ipir;    long fill_11[3]; /* Inter-Processor Int Request */
unsigned long sic;     long fill_12[3]; /* System Interrupt Clear */
unsigned long adlk;    long fill_13[3]; /* Address Lock (LDxL/STxC) */
unsigned long madrl;   long fill_14[3]; /* CBus Miss Address */
unsigned long rev;     long fill_15[3]; /* CMIC Revision */
};

/*
 * Data structure for handling T2 machine checks:
 */
struct el_t2_frame_header {
	unsigned int	elcf_fid;	/* Frame ID (from above) */
	unsigned int	elcf_size;	/* Size of frame in bytes */
};

struct el_t2_procdata_mcheck {
	unsigned long	elfmc_paltemp[32];	/* PAL TEMP REGS. */
	/* EV4-specific fields */
	unsigned long	elfmc_exc_addr;	/* Addr of excepting insn. */
	unsigned long	elfmc_exc_sum;	/* Summary of arith traps. */
	unsigned long	elfmc_exc_mask;	/* Exception mask (from exc_sum). */
	unsigned long	elfmc_iccsr;	/* IBox hardware enables. */
	unsigned long	elfmc_pal_base;	/* Base address for PALcode. */
	unsigned long	elfmc_hier;	/* Hardware Interrupt Enable. */
	unsigned long	elfmc_hirr;	/* Hardware Interrupt Request. */
	unsigned long	elfmc_mm_csr;	/* D-stream fault info. */
	unsigned long	elfmc_dc_stat;	/* D-cache status (ECC/Parity Err). */
	unsigned long	elfmc_dc_addr;	/* EV3 Phys Addr for ECC/DPERR. */
	unsigned long	elfmc_abox_ctl;	/* ABox Control Register. */
	unsigned long	elfmc_biu_stat;	/* BIU Status. */
	unsigned long	elfmc_biu_addr;	/* BUI Address. */
	unsigned long	elfmc_biu_ctl;	/* BIU Control. */
	unsigned long	elfmc_fill_syndrome;	/* For correcting ECC errors. */
	unsigned long	elfmc_fill_addr;/* Cache block which was being read. */
	unsigned long	elfmc_va;	/* Effective VA of fault or miss. */
	unsigned long	elfmc_bc_tag;	/* Backup Cache Tag Probe Results. */
};

/* 
 * Sable processor specific Machine Check Data segment.
 */

struct el_t2_logout_header {
	unsigned int	elfl_size;	/* size in bytes of logout area. */
	int	elfl_sbz1:31;		/* Should be zero. */
	char	elfl_retry:1;		/* Retry flag. */
        unsigned int	elfl_procoffset;	/* Processor-specific offset. */
	unsigned int	elfl_sysoffset;		/* Offset of system-specific. */
	unsigned int	elfl_error_type;	/* PAL error type code. */
	unsigned int	elfl_frame_rev;		/* PAL Frame revision. */
};
struct el_t2_sysdata_mcheck {
	unsigned long    elcmc_bcc;	      /* CSR 0 */
	unsigned long    elcmc_bcce;	      /* CSR 1 */
	unsigned long    elcmc_bccea;      /* CSR 2 */
	unsigned long    elcmc_bcue;	      /* CSR 3 */
	unsigned long    elcmc_bcuea;      /* CSR 4 */
	unsigned long    elcmc_dter;	      /* CSR 5 */
	unsigned long    elcmc_cbctl;      /* CSR 6 */
	unsigned long    elcmc_cbe;	      /* CSR 7 */
	unsigned long    elcmc_cbeal;      /* CSR 8 */
	unsigned long    elcmc_cbeah;      /* CSR 9 */
	unsigned long    elcmc_pmbx;	      /* CSR 10 */
	unsigned long    elcmc_ipir;	      /* CSR 11 */
	unsigned long    elcmc_sic;	      /* CSR 12 */
	unsigned long    elcmc_adlk;	      /* CSR 13 */
	unsigned long    elcmc_madrl;      /* CSR 14 */
	unsigned long    elcmc_crrev4;     /* CSR 15 */
};

/*
 * Sable memory error frame - sable pfms section 3.42
 */
struct el_t2_data_memory {
	struct	el_t2_frame_header elcm_hdr;	/* ID$MEM-FERR = 0x08 */
	unsigned int  elcm_module;	/* Module id. */
	unsigned int  elcm_res04;	/* Reserved. */
	unsigned long elcm_merr;	/* CSR0: Error Reg 1. */
	unsigned long elcm_mcmd1;	/* CSR1: Command Trap 1. */
	unsigned long elcm_mcmd2;	/* CSR2: Command Trap 2. */
	unsigned long elcm_mconf;	/* CSR3: Configuration. */
	unsigned long elcm_medc1;	/* CSR4: EDC Status 1. */
	unsigned long elcm_medc2;	/* CSR5: EDC Status 2. */
	unsigned long elcm_medcc;	/* CSR6: EDC Control. */
	unsigned long elcm_msctl;	/* CSR7: Stream Buffer Control. */
	unsigned long elcm_mref;	/* CSR8: Refresh Control. */
	unsigned long elcm_filter;	/* CSR9: CRD Filter Control. */
};


/*
 * Sable other cpu error frame - sable pfms section 3.43
 */
struct el_t2_data_other_cpu {
	short	      elco_cpuid;	/* CPU ID */
	short	      elco_res02[3];	
	unsigned long elco_bcc;	/* CSR 0 */
	unsigned long elco_bcce;	/* CSR 1 */
	unsigned long elco_bccea;	/* CSR 2 */
	unsigned long elco_bcue;	/* CSR 3 */
	unsigned long elco_bcuea;	/* CSR 4 */
	unsigned long elco_dter;	/* CSR 5 */
	unsigned long elco_cbctl;	/* CSR 6 */
	unsigned long elco_cbe;	/* CSR 7 */
	unsigned long elco_cbeal;	/* CSR 8 */
	unsigned long elco_cbeah;	/* CSR 9 */
	unsigned long elco_pmbx;	/* CSR 10 */
	unsigned long elco_ipir;	/* CSR 11 */
	unsigned long elco_sic;	/* CSR 12 */
	unsigned long elco_adlk;	/* CSR 13 */
	unsigned long elco_madrl;	/* CSR 14 */
	unsigned long elco_crrev4;	/* CSR 15 */
};

/*
 * Sable other cpu error frame - sable pfms section 3.44
 */
struct el_t2_data_t2{
        struct el_t2_frame_header elct_hdr;	/* ID$T2-FRAME */
	unsigned long elct_iocsr;	/* IO Control and Status Register */
	unsigned long elct_cerr1;	/* Cbus Error Register 1 */
	unsigned long elct_cerr2;	/* Cbus Error Register 2 */
	unsigned long elct_cerr3;	/* Cbus Error Register 3 */
	unsigned long elct_perr1;	/* PCI Error Register 1 */
	unsigned long elct_perr2;	/* PCI Error Register 2 */
	unsigned long elct_hae0_1;	/* High Address Extension Register 1 */
	unsigned long elct_hae0_2;	/* High Address Extension Register 2 */
	unsigned long elct_hbase;	/* High Base Register */
	unsigned long elct_wbase1;	/* Window Base Register 1 */
	unsigned long elct_wmask1;	/* Window Mask Register 1 */
	unsigned long elct_tbase1;	/* Translated Base Register 1 */
	unsigned long elct_wbase2;	/* Window Base Register 2 */
	unsigned long elct_wmask2;	/* Window Mask Register 2 */
	unsigned long elct_tbase2;	/* Translated Base Register 2 */
	unsigned long elct_tdr0;	/* TLB Data Register 0 */
	unsigned long elct_tdr1;	/* TLB Data Register 1 */
	unsigned long elct_tdr2;	/* TLB Data Register 2 */
	unsigned long elct_tdr3;	/* TLB Data Register 3 */
	unsigned long elct_tdr4;	/* TLB Data Register 4 */
	unsigned long elct_tdr5;	/* TLB Data Register 5 */
	unsigned long elct_tdr6;	/* TLB Data Register 6 */
	unsigned long elct_tdr7;	/* TLB Data Register 7 */
};

/*
 * Sable error log data structure - sable pfms section 3.40
 */
struct el_t2_data_corrected {
	unsigned long elcpb_biu_stat;
	unsigned long elcpb_biu_addr;
	unsigned long elcpb_biu_ctl;
	unsigned long elcpb_fill_syndrome;
	unsigned long elcpb_fill_addr;
	unsigned long elcpb_bc_tag;
};

/* 
 * Sable error log data structure
 * Note there are 4 memory slots on sable (see t2.h)
 */
struct el_t2_frame_mcheck {
        struct el_t2_frame_header elfmc_header;	/* ID$P-FRAME_MCHECK */
	struct el_t2_logout_header elfmc_hdr;
	struct el_t2_procdata_mcheck elfmc_procdata;
	struct el_t2_sysdata_mcheck elfmc_sysdata;
	struct el_t2_data_t2 elfmc_t2data;
	struct el_t2_data_memory elfmc_memdata[4]; 
        struct el_t2_frame_header elfmc_footer;	/* empty */
};


/* 
 * Sable error log data structures on memory errors
 */
struct el_t2_frame_corrected {
        struct el_t2_frame_header elfcc_header;	/* ID$P-BC-COR */
	struct el_t2_logout_header elfcc_hdr;
	struct el_t2_data_corrected elfcc_procdata; 
/*	struct el_t2_data_t2 elfcc_t2data;		*/
/*	struct el_t2_data_memory elfcc_memdata[4];	*/
        struct el_t2_frame_header elfcc_footer;	/* empty */
};


#define RTC_PORT(x)	(0x70 + (x))
#define RTC_ADDR(x)	(0x80 | (x))
#define RTC_ALWAYS_BCD	0

#endif /* __ALPHA_T2__H__ */
