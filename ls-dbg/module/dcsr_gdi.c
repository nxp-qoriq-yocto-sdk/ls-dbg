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

#include "dcsr_gdi.h"

/* Driver Initialization Functions */
int dcsr_gdi_ls1_init(struct dentry *parent_dentry, struct dbg_device *dev)
{
	struct dentry *current_dentry;
	struct dentry *de;
	struct gdi *ptr = (struct gdi *)dev->mem_ptr[0];
	char reg_name[DBFS_REG_NAME_LEN];
	int i;

	CREATE_CURRENT_DBGFS_DIR(parent_dentry, dev, DEBUGFS_GDI_NAME);

	DBGFS_CREATE_RW_X32("gdcr", current_dentry, &ptr->gdcr);
	DBGFS_CREATE_RW_X32("gdsr", current_dentry, &ptr->gdsr);
	DBGFS_CREATE_RW_X32("gdrescr1", current_dentry, &ptr->gdrescr1);
	DBGFS_CREATE_RW_X32("gdrescr2", current_dentry, &ptr->gdrescr2);


	for (i = 0; i < GDI_LS1_NO_OF_GDI_EVNTS; ++i) {
		sprintf(reg_name, "gdpescr%d", i);
		DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->gdpescr[i]);
	}

	for (i = 1; i <= GDI_LS1_NO_CMP_CNTRL_REG; ++i) {
		sprintf(reg_name, "gdgpccr%d", i);
		DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->gdgpc[i].cr);

		sprintf(reg_name, "gdgpcmr%d", i);
		DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->gdgpc[i].mr);

		sprintf(reg_name, "gdgpcvr%d", i);
		DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->gdgpc[i].vr);
	}

	return 0;
}

