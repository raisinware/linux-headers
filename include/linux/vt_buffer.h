/*
 *	include/linux/vt_buffer.h -- Access to VT screen buffer
 *
 *	(c) 1998 Martin Mares <mj@ucw.cz>
 *
 *	This is a set of macros and functions which are used in the
 *	console driver and related code to access the screen buffer.
 *	In most cases the console works with simple in-memory buffer,
 *	but when handling hardware text mode consoles, we store
 *	the foreground console directly in video memory.
 */

#ifndef _LINUX_VT_BUFFER_H_
#define _LINUX_VT_BUFFER_H_

#include <linux/config.h>

#if defined(CONFIG_VGA_CONSOLE) || defined(CONFIG_MDA_CONSOLE)
#include <asm/vga.h>
#endif

#ifndef VT_BUF_HAVE_RW
#define scr_writew(val, addr) (*(addr) = (val))
#define scr_readw(addr) (*(addr))
#define scr_memcpyw(d, s, c) memcpy(d, s, c)
#define scr_memmovew(d, s, c) memmove(d, s, c)
#define VT_BUF_HAVE_MEMCPYW
#define VT_BUF_HAVE_MEMMOVEW
#define scr_memcpyw_from(d, s, c) memcpy(d, s, c)
#define scr_memcpyw_to(d, s, c) memcpy(d, s, c)
#define VT_BUF_HAVE_MEMCPYF
#endif

#ifndef VT_BUF_HAVE_MEMSETW
extern inline void scr_memsetw(u16 *s, u16 c, unsigned int count)
{
	count /= 2;
	while (count--)
		scr_writew(c, s++);
}
#endif

#ifndef VT_BUF_HAVE_MEMCPYW
extern inline void scr_memcpyw(u16 *d, const u16 *s, unsigned int count)
{
	count /= 2;
	while (count--)
		scr_writew(scr_readw(s++), d++);
}
#endif

#ifndef VT_BUF_HAVE_MEMMOVEW
extern inline void scr_memmovew(u16 *d, const u16 *s, unsigned int count)
{
	if (d < s)
		scr_memcpyw(d, s, count);
	else {
		count /= 2;
		d += count;
		s += count;
		while (count--)
			scr_writew(scr_readw(--s), --d);
	}
}
#endif

#ifndef VT_BUF_HAVE_MEMCPYF
extern inline void scr_memcpyw_from(u16 *d, const u16 *s, unsigned int count)
{
	count /= 2;
	while (count--)
		*d++ = scr_readw(s++);
}

extern inline void scr_memcpyw_to(u16 *d, const u16 *s, unsigned int count)
{
	count /= 2;
	while (count--)
		scr_writew(*s++, d++);
}
#endif

#endif
