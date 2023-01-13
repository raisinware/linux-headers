/*********************************************************************
 *                
 * Filename:      irmod.h
 * Version:       0.3
 * Description:   IrDA module and utilities functions
 * Status:        Experimental.
 * Author:        Dag Brattli <dagb@cs.uit.no>
 * Created at:    Mon Dec 15 13:58:52 1997
 * Modified at:   Tue Mar 16 22:27:41 1999
 * Modified by:   Dag Brattli <dagb@cs.uit.no>
 *
 *     Copyright (c) 1998 Dag Brattli, All Rights Reserved.
 *      
 *     This program is free software; you can redistribute it and/or 
 *     modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation; either version 2 of 
 *     the License, or (at your option) any later version.
 *  
 *     Neither Dag Brattli nor University of Troms� admit liability nor
 *     provide warranty for any of this software. This material is 
 *     provided "AS-IS" and at no charg.
 *     
 ********************************************************************/

#ifndef IRMOD_H
#define IRMOD_H

#include <linux/skbuff.h>
#include <linux/miscdevice.h>

#include <net/irda/irqueue.h>

#define IRMGR_IOC_MAGIC 'm'
#define IRMGR_IOCTNPC     _IO(IRMGR_IOC_MAGIC, 1)
#define IRMGR_IOC_MAXNR   1 

/*
 *  Events that we pass to the user space manager
 */
typedef enum {
	EVENT_DEVICE_DISCOVERED = 0,
	EVENT_REQUEST_MODULE,
	EVENT_IRLAN_START,
	EVENT_IRLAN_STOP,
	EVENT_IRLPT_START,
	EVENT_IRLPT_STOP,
	EVENT_IROBEX_START,
	EVENT_IROBEX_STOP,
	EVENT_IRDA_STOP,
	EVENT_NEED_PROCESS_CONTEXT,
} IRMGR_EVENT;

/*
 *  Event information passed to the IrManager daemon process
 */
struct irmanager_event {
	IRMGR_EVENT event;
	char devname[10];
	char info[32];
	int service;
	__u32 saddr;
	__u32 daddr;
};

typedef void (*TODO_CALLBACK)( void *self, __u32 param);

/*
 *  Same as irmanager_event but this one can be queued and inclueds some
 *  addtional information
 */
struct irda_event {
	QUEUE q; /* Must be first */
	
	struct irmanager_event event;
};

/*
 *  Funtions with needs to be called with a process context
 */
struct irda_todo {
	QUEUE q; /* Must be first */

	void *self;
	TODO_CALLBACK callback;
	__u32 param;
};

/*
 *  Main structure for the IrDA device (not much here :-)
 */
struct irda_cb {
	struct miscdevice dev;	
	struct wait_queue *wait_queue;

	int in_use;

	QUEUE *event_queue; /* Events queued for the irmanager */
	QUEUE *todo_queue;  /* Todo list */
};

int irmod_init_module(void);
void irmod_cleanup_module(void);

inline int irda_lock(int *lock);
inline int irda_unlock(int *lock);

void irda_notify_init(struct notify_t *notify);

void irda_execute_as_process(void *self, TODO_CALLBACK callback, __u32 param);
void irmanager_notify(struct irmanager_event *event);

extern void irda_proc_modcount(struct inode *, int);
void irda_mod_inc_use_count(void);
void irda_mod_dec_use_count(void);

#endif /* IRMOD_H */









