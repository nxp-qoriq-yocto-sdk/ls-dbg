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

#define MAX_TRACE_MODULES 16

static long reg_access_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static void __iomem *reg_find_map(enum TRACEIP_MODULE id, unsigned long offset);

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

static long reg_access_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct reg_access ra;
	struct reg_access __user *p = (struct reg_access __user *) arg;
	void __iomem *reg_addr;
	__u32 val;
	unsigned long buf_addr;

	if (copy_from_user(&ra, p, sizeof(struct reg_access))) {
		ret = -EFAULT;
		goto out;
	}

	if (ra.reg_size != 32) {
		ret = -EINVAL;
		goto out;
	}

	buf_addr = (unsigned long) ra.buffer;

	reg_addr = reg_find_map(ra.module_id, ra.offset);

	if (IS_ERR(reg_addr)) {
		ret = PTR_ERR(reg_addr);
		goto out;
	}

	switch (cmd) {
	case DBG_READ_REG: {
		val = ioread32(reg_addr);

		if (put_user(val, (__u32 *)buf_addr)) {
			ret = -EFAULT;
			goto out;
		}

		break;
	}

	case DBG_WRITE_REG: {
		if (get_user(val, (__u32 *)buf_addr)) {
			ret = -EFAULT;
			goto out;
		}

		iowrite32(val, reg_addr);

		break;
	}

	default:
		return -ENOTTY;
	}

out:
	return ret;
}

int reg_add_map(enum TRACEIP_MODULE id, unsigned long addr, unsigned long size)
{
	struct map_info *mi;

	if (id < 0 || id >= NUM_IDS)
		return -EINVAL;

	mi = &maps[id];

	mi->base_addr = addr;
	mi->size = size;

	return 0;
}

static void __iomem *reg_find_map(enum TRACEIP_MODULE id, unsigned long offset)
{
	struct map_info *mi;

	if (id < 0 || id >= NUM_IDS)
		return ERR_PTR(-EINVAL);

	mi = &maps[id];

	if (offset >= mi->size)
		return ERR_PTR(-EFAULT);

	return (void __iomem *)(mi->base_addr + offset);
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
