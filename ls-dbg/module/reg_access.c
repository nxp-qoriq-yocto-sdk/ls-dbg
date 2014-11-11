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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/miscdevice.h>

#include "reg_access.h"
#include "reg_access_ioctl.h"
#include "dbg_private.h"
#include "vir_cntrs.h"

#define MAX_TRACE_MODULES 16

static long reg_access_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static void __iomem *reg_find_map_io(enum TRACEIP_MODULE id, unsigned long offset);
static void *reg_find_map_ram(enum TRACEIP_MODULE id, unsigned long offset);

static const struct file_operations reg_access_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = reg_access_ioctl,
};

struct miscdevice reg_access_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ls-dbg",
	.fops = &reg_access_fops,
};

struct map_info {
	enum TRACEIP_MODULE id;
	unsigned long base_addr;
	unsigned long size;
};

static struct map_info maps[MAX_TRACE_MODULES];

extern struct vcounters ls_dbg_vcnt;

__u32 NOINLINE ls_dbg_read_reg_internal(enum TRACEIP_MODULE module_id,
												__u32 offset, __u64 *out_val)
{
	void __iomem *reg_io_addr;
	void *reg_ram_addr;
	
	if (IS_IO_MAPPED_MODULE(module_id)) {
		reg_io_addr = reg_find_map_io(module_id, offset);

		if (unlikely(IS_ERR(reg_io_addr))) {
			printk(KERN_ERR
					"ls_dbg_read_reg_internal failed to read register at"
					" module id %d, offset 0x%x\n",
 	 	 	 	 	 	 module_id, offset);
			return PTR_ERR(reg_io_addr);
		}

		*out_val = (__u64)ioread32(reg_io_addr);
	}
	else {
		
		reg_ram_addr = reg_find_map_ram(module_id, offset);

		if (unlikely(IS_ERR(reg_ram_addr))) {
				printk(KERN_ERR
						"ls_dbg_read_reg_internal failed to read register at"
						" module id %d, offset 0x%x\n",
	 	 	 	 	 	 	 module_id, offset);
			return PTR_ERR(reg_ram_addr);
		}

		return vcounter_reg_read(reg_ram_addr, out_val);		
	}
	
	return 0;
}

__u32 NOINLINE ls_dbg_write_reg_internal(enum TRACEIP_MODULE module_id,
												__u32 offset, __u64 in_val)
{
	void __iomem *reg_io_addr;
	void *reg_ram_addr;

	if (IS_IO_MAPPED_MODULE(module_id)) {

		reg_io_addr = reg_find_map_io(module_id, offset);

		if (unlikely(IS_ERR(reg_io_addr))) {
			printk(KERN_ERR
					"ls_dbg_write_reg_internal failed to write register at"
					" module id %d, offset 0x%x\n",
 	 	 	 	 	 	 module_id, offset);

			return PTR_ERR(reg_io_addr);
		}

		iowrite32(in_val, reg_io_addr);
	}
	else {
		reg_ram_addr = reg_find_map_ram(module_id, offset);

		if (unlikely(IS_ERR(reg_ram_addr))) {
			printk(KERN_ERR
					"ls_dbg_write_reg_internal failed to write register at"
					" module id %d, offset 0x%x\n",
 	 	 	 	 	 	 module_id, offset);
			return PTR_ERR(reg_ram_addr);
		}
		return vcounter_reg_write(reg_ram_addr, in_val);		
	}
	return 0;
}

static long reg_access(unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct reg_access ra;
	struct reg_access __user *p;
	__u64 val;
	unsigned long buf_addr;

	p = (struct reg_access __user *) arg;

	if (copy_from_user(&ra, p, sizeof(struct reg_access))) {
		ret = -EFAULT;
		goto out;
	}

	if (ra.reg_size != 32 && ra.reg_size != 64) {
		ret = -EINVAL;
		goto out;
	}

	buf_addr = (unsigned long) ra.buffer;

	
	switch (cmd) {
	case DBG_READ_REG: {
		
		ret = ls_dbg_read_reg_internal(ra.module_id, ra.offset, &val);
		
		if (!ret) {
			if (ra.reg_size == 32) {
				ret = put_user(val, (__u32 *)buf_addr);
			}
			else { /* 64 */
				ret = put_user(val, (__u64 *)buf_addr);
			}

			if (ret) {
				ret = -EFAULT;
			}
		}

		goto out;
	}

	case DBG_WRITE_REG: {

		if (ra.reg_size == 32) {
			ret = get_user(val, ((__u32 *)buf_addr));
		}
		else { /* 64 */
			/* This causes a link warning and failure to load
			 * ret = get_user(val, ((__u64 *)buf_addr));
			 * Must use the less efficient copy_from_user()
			 */
			ret = copy_from_user(&val, (__u64 *)buf_addr, sizeof(__u64));
		}

		if (ret) {
			ret = -EFAULT;
		}
		else {
			ret = ls_dbg_write_reg_internal(ra.module_id, ra.offset, val);
		}

		goto out;
	}

	default:
		return -ENOTTY;
	}

out:
	return ret;
}

static long resource_reserve(unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct resource_reservation reserv;
	struct resource_reservation __user *p;
	__u32 val_32b;
	__u64 prev_val;
	__u64 indx_mask;

	p = (struct resource_reservation __user *) arg;

	if (copy_from_user(&reserv, p, sizeof(struct resource_reservation))) {
		ret = -EFAULT;
		goto reserv_out;
	}

	indx_mask = 1 << reserv.res_index;

	switch (cmd) {
		case DBG_RESERVE_RES: {

			/* the common case is the reservation field is 32 bits,
			 * even if not all are used. If we add a 64b reservation
			 * field, this should be taken care of in the appro. case below */

			val_32b = (__u32)indx_mask;


			switch (reserv.resource_grp_id) {
				case EPU0_COUNTER: {

					if (reserv.res_index >= EPU_V2_NO_OF_COUNTERS) {
						ret = -EINVAL;
						goto reserv_out;
					}

					prev_val =
						__sync_fetch_and_or(&ls_dbg_vcnt.vcntrres, val_32b);

					break;
				}
				case EPU0_GROUP: {

					if (reserv.res_index >= EPU_V2_NO_OF_COUNTER_GROUPS) {
						ret = -EINVAL;
						goto reserv_out;
					}

					prev_val =
						__sync_fetch_and_or(&ls_dbg_vcnt.vcntrgrpres, val_32b);

					break;
				}
				case EPU_SCU_EVENT: {

					if (reserv.res_index >= EPU_V2_NO_OF_SCU_EVENTS) {
						ret = -EINVAL;
						goto reserv_out;
					}

					prev_val =
						__sync_fetch_and_or(&ls_dbg_vcnt.vcntrevtres, val_32b);

					break;
				}
				
				default: {
					ret = -EINVAL;
					goto reserv_out;
				}
			}

			/* assumes all reservation fields are 32b; if 64b field is added
			 * take care of this below*/
			if (prev_val & val_32b) {
				/* It was already set, so resource is not available and we 
				 * should return error */
				ret = -EBUSY;
			}

			break;
		}

		case DBG_RELINQ_RES: {
			/* the common case is the reservation field is 32 bits,
			 * even if not all are used. If we add a 64b reservation
			 * field, this should be taken care of in the appro. case below */

			val_32b = (__u32)indx_mask;
			val_32b = ~val_32b;

			switch (reserv.resource_grp_id) {
				case EPU0_COUNTER: {

					if (reserv.res_index >= EPU_V2_NO_OF_COUNTERS) {
						ret = -EINVAL;
						goto reserv_out;
					}

					prev_val =
						__sync_fetch_and_and(&ls_dbg_vcnt.vcntrres, val_32b);

					break;
				}
				case EPU0_GROUP: {

					if (reserv.res_index >= EPU_V2_NO_OF_COUNTER_GROUPS) {
						ret = -EINVAL;
						goto reserv_out;
					}

					prev_val =
						__sync_fetch_and_and(&ls_dbg_vcnt.vcntrgrpres, val_32b);

					break;
				}
				case EPU_SCU_EVENT: {

					if (reserv.res_index >= EPU_V2_NO_OF_SCU_EVENTS) {
						ret = -EINVAL;
						goto reserv_out;
					}

					prev_val =
						__sync_fetch_and_and(&ls_dbg_vcnt.vcntrevtres, val_32b);

					break;
				}
				default: {
					ret = -EINVAL;
					goto reserv_out;
				}

			}

			/* assumes all reservation fields are 32b; if 64b field is added
			 * take care of this below*/

			val_32b = ~val_32b;
			
			if ( (prev_val & val_32b) == 0) {
				/* It was already cleared, so we should return error */
				ret = -EPERM;
			}
			
			break;
		}
	
		default:
			return -ENOTTY;
	}

reserv_out:
	return ret;	
}

static long reg_access_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	if (cmd == DBG_READ_REG  || cmd == DBG_WRITE_REG) {
		return reg_access(cmd, arg);
	}
	else {
		/* resource operations */
		return resource_reserve(cmd, arg);
	}
	
}

int reg_add_map(enum TRACEIP_MODULE id, unsigned long addr,
														unsigned long size)
{
	struct map_info *mi;

	if (id < 0 || id >= NUM_IDS)
		return -EINVAL;

	mi = &maps[id];

	mi->base_addr = addr;
	mi->size = size;

	return 0;
}

static void __iomem *reg_find_map_io(enum TRACEIP_MODULE id,
														unsigned long offset)
{
	struct map_info *mi;

	if (id < 0 || id >= NUM_IO_IDS)
		return ERR_PTR(-EINVAL);

	mi = &maps[id];

	if (offset >= mi->size)
		return ERR_PTR(-EFAULT);

	return (void __iomem *)(mi->base_addr + offset);
}

static void *reg_find_map_ram(enum TRACEIP_MODULE id, unsigned long offset)
{
	struct map_info *mi;

	if (id < VCOUNT || id >= NUM_IDS)
		return ERR_PTR(-EINVAL);

	mi = &maps[id];

	if (offset >= mi->size)
		return ERR_PTR(-EFAULT);

	return (void *)(mi->base_addr + offset);
}

void reg_access_cleanup(void)
{
	misc_deregister(&reg_access_dev);
}

int reg_access_init(void)
{
	int ret = 0;

	ret = misc_register(&reg_access_dev);
	if (ret)
		goto out;

	return 0;

out:
	return ret;
}
