/*
 * Copyright (C) 2010, 2011 Freescale Semiconductor, Inc.
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

#ifndef DBG_PRIVATE_H
#define DBG_PRIVATE_H

#include <linux/types.h>
#include <linux/debugfs.h>

/* max size of register names */
#define DBFS_REG_NAME_LEN 16

#define DBGFS_RO_MODE	(S_IRUSR | S_IRGRP | S_IROTH)
#define DBGFS_WO_MODE	(S_IWUSR | S_IWGRP | S_IWOTH)
#define DBGFS_RW_MODE	(DBGFS_RO_MODE | DBGFS_WO_MODE)

#define DBGFS_CREATE_X32(f_name, mode, parent_de, data) \
	do { \
		de = debugfs_create_x32(f_name, mode, parent_de, data); \
		if (IS_ERR_OR_NULL(de)) \
			return -ENOMEM; \
	} while (0)

#define DBGFS_CREATE_RW_X32(f_name, parent_de, data) \
		DBGFS_CREATE_X32(f_name, DBGFS_RW_MODE, parent_de, data)

#define DBGFS_CREATE_RO_X32(f_name, parent_de, data) \
		DBGFS_CREATE_X32(f_name, DBGFS_RO_MODE, parent_de, data)

#define DBGFS_CREATE_WO_X32(f_name, parent_de, data) \
		DBGFS_CREATE_X32(f_name, DBGFS_WO_MODE, parent_de, data)

/* Create the named directory in the parent dentry */
#define CREATE_CURRENT_DBGFS_DIR(parent_dentry, dev, name) \
	do { \
		current_dentry = debugfs_create_dir(name, parent_dentry); \
		if (IS_ERR_OR_NULL(current_dentry)) { \
			printk(KERN_ERR "Creating name @ \"%s\" failed\n", \
					name); \
			return -ENOMEM; \
		} \
		(dev)->current_dentry = current_dentry; \
	} while (0)

/* Create the named index directory in the parent dir*/
#define CREATE_CURRENT_DBGFS_DIR_INDEXED(parent_dentry, dev, name, index) \
	do { \
		char dir_name[DBFS_REG_NAME_LEN]; \
		snprintf(dir_name, DBFS_REG_NAME_LEN, \
				"%s%d", name, index); \
		CREATE_CURRENT_DBGFS_DIR(parent_dentry, dev, dir_name); \
	} while (0)

#endif /* DBG_PRIVATE_H */
