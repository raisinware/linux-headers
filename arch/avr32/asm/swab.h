/*
 * AVR32 byteswapping functions.
 */
#ifndef __ASM_AVR32_SWAB_H
#define __ASM_AVR32_SWAB_H

#include <asm/types.h>


#define __SWAB_64_THRU_32__

#ifdef __CHECKER__
extern unsigned long __builtin_bswap_32(unsigned long x);
extern unsigned short __builtin_bswap_16(unsigned short x);
#endif

/*
 * avr32-linux-gcc versions earlier than 4.2 improperly sign-extends
 * the result.
 */
#if !(__GNUC__ == 4 && __GNUC_MINOR__ < 2)
static __inline__ __u16 __arch_swab16(__u16 val)
{
	return __builtin_bswap_16(val);
}
#define __arch_swab16 __arch_swab16

static __inline__ __u32 __arch_swab32(__u32 val)
{
	return __builtin_bswap_32(val);
}
#define __arch_swab32 __arch_swab32
#endif

#endif /* __ASM_AVR32_SWAB_H */
