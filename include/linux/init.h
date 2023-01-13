#ifndef _LINUX_INIT_H
#define _LINUX_INIT_H

/* These macros are used to mark some functions or 
 * initialized data (doesn't apply to uninitialized data)
 * as `initialization' functions. The kernel can take this
 * as hint that the function is used only during the initialization
 * phase and free up used memory resources after
 *
 * Usage:
 * For functions:
 * you can surround the whole function declaration 
 * just before function body into __initfunc() macro, like:
 *
 * __initfunc (static void initme(int x, int y))
 * {
 *    extern int z; z = x * y;
 * }
 *
 * if the function has a prototype somewhere, you can also add
 * __init between closing brace of the prototype and semicolon:
 *
 * extern int initialize_foobar_device(int, int, int) __init;
 *
 * For initialized data:
 * you should insert __initdata between the variable name and equal
 * sign followed by value, e.g.:
 *
 * static int init_variable __initdata = 0;
 * static char linux_logo[] __initdata = { 0x32, 0x36, ... };
 */

/*
 * Disable the __initfunc macros if a file that is a part of a
 * module attempts to use them. We do not want to interfere
 * with module linking.
 */

#ifndef MODULE
#include <asm/init.h>
#else
#define __init
#define __initdata
#define __initfunc(__arginit) __arginit
/* For assembly routines */
#define __INIT
#define __FINIT
#define __INITDATA
#endif

#endif
