/* -*- linux-c -*- --------------------------------------------------------- *
 *
 * linux/include/linux/devpts_fs.h
 *
 *  Copyright 1998 H. Peter Anvin -- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * ------------------------------------------------------------------------- */

/*
 * Prototypes for the pty driver <-> devpts filesystem interface.  Most
 * of this is really just a hack so we can exclude it or build it as a
 * module, and probably should go away eventually.
 */

#ifndef _LINUX_DEVPTS_FS_H
#define _LINUX_DEVPTS_FS_H 1

#include <linux/config.h>
#include <linux/kdev_t.h>

#ifdef CONFIG_DEVPTS_FS

void devpts_pty_new(int, kdev_t);
void devpts_pty_kill(int);

#elif defined(CONFIG_DEVPTS_FS_MODULE)

#ifdef BUILDING_PTY_C
void (*devpts_upcall_new)(int,kdev_t) = NULL;
void (*devpts_upcall_kill)(int)       = NULL;

EXPORT_SYMBOL(devpts_upcall_new);
EXPORT_SYMBOL(devpts_upcall_kill);
#else
extern void (*devpts_upcall_new)(int,kdev_t);
extern void (*devpts_upcall_kill)(int);
#endif

#ifndef BUILDING_DEVPTS
extern inline void
devpts_pty_new(int line, kdev_t device)
{
	if ( devpts_upcall_new )
		return devpts_upcall_new(line,device);
}

extern inline void
devpts_pty_kill(int line)
{
	if ( devpts_upcall_kill )
		return devpts_upcall_kill(line);
}
#endif

#else  /* No /dev/pts filesystem at all */

extern inline void
devpts_pty_new(int line, kdev_t device) { }

extern inline void
devpts_pty_kill(int line) { }

#endif

#endif /* _LINUX_DEVPTS_FS_H */