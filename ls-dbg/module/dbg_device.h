/*
 * Copyright (C) 2010, 2011, 2012 Freescale Semiconductor, Inc.
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
#ifndef DBG_DEVICE_H
#define DBG_DEVICE_H

#include <linux/debugfs.h>
#include <linux/of_platform.h>
#include <linux/list.h>

/* Only support a max of 2 memory ranges per device.
 * If this increases then should consider a dynamic list instead */
#define MAX_MEM_RANGE	2

struct dbg_device;

/* Operations on the device entry */
typedef int (*dev_init_fn)(struct dbg_device *dev);
typedef int (*dev_remove_fn)(struct dbg_device *dev);
typedef int (*dbgfs_init_fn)(struct dentry *parent_dentry,
		struct dbg_device *dev);

/* dbg_device structure */
struct dbg_device {
	struct list_head list;
	/*  device tree ordinal index */
	int dt_idx;
	struct device_node *np;
	/* base name /index for the dbgfs dir */
	const char *dbgfs_dir_name;
	int dbgfs_dir_index;
	/* current dbgfs directory */
	struct dentry *current_dentry;
	/* number of valid memory range ids */
	int num_mem_ids;
	/* memory range id */
	int mem_id[MAX_MEM_RANGE];
	/* mapped memory range */
	void __iomem *mem_ptr[MAX_MEM_RANGE];
	/* device dependent private data */
	void *private[MAX_MEM_RANGE];
	/* device operations */
	dev_init_fn dev_init_fn;
	dev_remove_fn dev_remove_fn;
	dbgfs_init_fn dbgfs_init_fn;
};

#endif /* DBG_DEVICE_H */
