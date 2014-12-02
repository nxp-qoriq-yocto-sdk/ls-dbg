/*
 * Copyright (C) 2014 Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * This software may be distributed under the terms of the
 * GNU General Public License ("GPL") as published by the Free Software
 * Foundation, either version 2 of that License or (at your option) any
 * later version.
 *
 * THIS SOFTWARE IS PROVIDED BY Freescale Semiconductor ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Freescale Semiconductor BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DBG_REG_ACCESS_IOCTL_H
#define DBG_REG_ACCESS_IOCTL_H

#include <linux/types.h>
#include <linux/ioctl.h>
#include "ls_dbg.h"


#define IS_IO_MAPPED_MODULE(ID) ((ID >= ETM0) && (ID < NUM_IO_IDS))

struct reg_access {
	/* the trace module whose register will be accessed */
	enum TRACEIP_MODULE module_id;

	/* the offset of the register, with respect with the module's base address */
	__u32 offset;

	/*
	 * for a read operation: the address of a buffer in userspace where
	 * the data should be put
	 *
	 * for a write operation: the address of a buffer in userspace holding
	 * the data to be written
	 */
	__u64 buffer;

	/* register's size in bits */
	__u32 reg_size;
} __attribute__((packed));


struct resource_reservation {
	/* the resource set ID */
	enum RESOURCE_GROUP_ID resource_grp_id;

	/* index within resource group that is being queried/requested */
	__u32 res_index;

	__u32 res_usage;
} __attribute__((packed));


/* ioctl numbers */
#define LS_DBG_MAGIC 'Z'
#define LS_DBG_RES_GET_USAGE	_IOWR(LS_DBG_MAGIC, 95, struct resource_reservation)
#define LS_DBG_RES_RESERVE	_IOWR(LS_DBG_MAGIC, 96, struct resource_reservation)
#define LS_DBG_RES_RELINQ	_IOWR(LS_DBG_MAGIC, 97, struct resource_reservation)


#define DBG_READ_REG	_IOWR(LS_DBG_MAGIC, 98, struct reg_access)
#define DBG_WRITE_REG	_IOWR(LS_DBG_MAGIC, 99, struct reg_access)

#define DBG_DEVICE_PATH	"/dev/ls-dbg"

#endif
