/*
 * Copyright (C) 2010-2014 Freescale Semiconductor, Inc.
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

#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/io.h>

#include "ls_dbg.h"
#include "reg_access.h"
#include "reg_access_ioctl.h"

/* platform_driver operations */
static int dcsr_probe(struct platform_device *pdev);
static int dcsr_remove(struct platform_device *pdev);

/* debugfs root directory entry */
static struct dentry *dbgfs_root_dentry;

/* dbg_device list */
static LIST_HEAD(dbg_devs);

const struct of_device_id dcsr_id_table[] = {
		{ .compatible = "fsl,ls1021a-dcsr"},
		{},
};
MODULE_DEVICE_TABLE(of, dcsr_id_table);

static struct platform_driver dcsr_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = dcsr_id_table,
	},
	.probe = dcsr_probe,
	.remove = dcsr_remove,
};

static struct dbg_device *alloc_device(void)
{
	struct dbg_device *dev;
	dev = kzalloc(sizeof(struct dbg_device),
		GFP_ATOMIC);
	if (dev)
		INIT_LIST_HEAD(&dev->list);
	return dev;
}

/* Allocate and add all compatible devices to the dbg_device list */
static int add_compatible_devices(const char* compatible,
	const char *name, enum TRACEIP_MODULE id,
	dev_init_fn dev_init, dbgfs_init_fn dbgfs_init)
{
	struct device_node *np;
	struct dbg_device *dev;
	int index = 0;
	int i;
	int num_found = 0;
	struct resource res;

	for_each_compatible_node(np, NULL, compatible) {
		dev = alloc_device();
		if (!dev) {
			printk(KERN_ERR DRIVER_NAME
			       ": Cannot allocate memory for %s.\n",
			       np->full_name);
			continue;
		}
		/* Try to map MAX_MEM_RANGES */
		for (i = 0; i < MAX_MEM_RANGE; i++) {
			dev->mem_id[i] = i;
			dev->mem_ptr[i] = of_iomap(np, i);

			/* record the new mapping */
			if (dev->mem_ptr[i]) {
				of_address_to_resource(np, i, &res);
				reg_add_map(id + index + i,
					    (unsigned long)dev->mem_ptr[i],
					    resource_size(&res));
			}
		}
		dev->dbgfs_dir_name = name;
		dev->dbgfs_init_fn = dbgfs_init;
		dev->dev_init_fn = dev_init;
		dev->np = np;
		dev->dt_idx = index++;
		list_add(&dev->list, &dbg_devs);
		num_found++;
	}

	return num_found;
}

static void remove_devices(void)
{
	struct dbg_device *dev;
	struct list_head *pos, *q;
	int i;
	void *mem;

	list_for_each_entry(dev, &dbg_devs, list) {
		/* unmap the memory ranges */
		for (i = 0; i < MAX_MEM_RANGE; i++) {
			if (dev->mem_ptr[i]) {
				mem = dev->mem_ptr[i];
				dev->mem_ptr[i] = 0;
				iounmap(mem);
			}
		}
		/* device specific cleanup on removal */
		if (dev->dev_remove_fn)
			dev->dev_remove_fn(dev);
	}

	list_for_each_safe(pos, q, &dbg_devs) {
		dev = list_entry(pos, struct dbg_device, list);
		list_del(pos);
		kfree(dev);
	}
}

static int dcsr_probe(struct platform_device *pdev)
{
	struct dbg_device *dev;
	int ret = 0;

	add_compatible_devices("fsl,ls1021a-dcsr-etf", DEBUGFS_ETF_NAME, ETF0,
			       NULL, dcsr_etf_ls1_init);
	add_compatible_devices("fsl,ls1021a-dcsr-etr", DEBUGFS_ETR_NAME, ETR,
			       NULL, dcsr_etr_ls1_init);
	add_compatible_devices("fsl,ls1021a-dcsr-etm", DEBUGFS_ETM_NAME, ETM0,
			       NULL, dcsr_etm_ls1_init);
	add_compatible_devices("fsl,ls1021a-dcsr-cstf", DEBUGFS_CSTF_NAME, CSTF0,
			       NULL, dcsr_cstf_ls1_init);

	/* create the root directory in debugfs */
	dbgfs_root_dentry = debugfs_create_dir(DBGFS_ROOT_NAME, NULL);
	if (IS_ERR_OR_NULL(dbgfs_root_dentry)) {
		ret = -ENOMEM;
		goto err;
	}

	ret = reg_access_init();
	if (ret) {
		goto err;
	}

	/* perform device specific initialization */
	list_for_each_entry(dev, &dbg_devs, list) {
		if (dev->dev_init_fn)
			dev->dev_init_fn(dev);
	}

	/* device specific creation of debugfs entries */
	list_for_each_entry(dev, &dbg_devs, list) {
		if (dev->dbgfs_init_fn)
			dev->dbgfs_init_fn(dbgfs_root_dentry, dev);
	}

	return ret;
err:
	remove_devices();
	return ret;
}

static int dcsr_remove(struct platform_device *pdev)
{
	debugfs_remove_recursive(dbgfs_root_dentry);
	remove_devices();
	reg_access_cleanup();
	return 0;
}

static int __init dcsr_module_init(void)
{
	return platform_driver_register(&dcsr_driver);
}
module_init(dcsr_module_init);

static void __exit dcsr_module_exit(void)
{
	platform_driver_unregister(&dcsr_driver);
}
module_exit(dcsr_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("freescale.com");
MODULE_DESCRIPTION("Layerscape Debug");
MODULE_SUPPORTED_DEVICE("Freescale LS1021A");
