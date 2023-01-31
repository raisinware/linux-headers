/*
 * Copyright (C) Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_MICROBLAZE_TYPES_H
#define _ASM_MICROBLAZE_TYPES_H

/*
 * This file is never included by application software unless
 * explicitly requested (e.g., via linux/types.h) in which case the
 * application is Linux specific so (user-) name space pollution is
 * not a major issue.  However, for interoperability, libraries still
 * need to be careful to avoid a name clashes.
 */

#include <asm-generic/int-ll64.h>

# ifndef __ASSEMBLY__

typedef unsigned short umode_t;

/*
 * These aren't exported outside the kernel to avoid name space clashes
 */
# endif /* __ASSEMBLY__ */
#endif /* _ASM_MICROBLAZE_TYPES_H */
