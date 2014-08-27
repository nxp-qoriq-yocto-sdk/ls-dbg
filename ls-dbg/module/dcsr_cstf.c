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

#include "dcsr_cstf.h"

/* Driver Initialization Functions */
int dcsr_cstf_ls1_init(struct dentry *parent_dentry, struct dbg_device *dev)
{
	struct dentry *current_dentry;
	struct dentry *de;
	struct funnel *ptr = (struct funnel *)dev->mem_ptr[0];
	char reg_name[DBFS_REG_NAME_LEN];
	int i;
	int k;

	for (k = 0; k < CSTF_NUM_DEVICES; k++) {
		CREATE_CURRENT_DBGFS_DIR_INDEXED(parent_dentry, dev,
						 DEBUGFS_CSTF_NAME, k);

		DBGFS_CREATE_RW_X32("cr", current_dentry, &ptr->cr);
		DBGFS_CREATE_RW_X32("pcr", current_dentry, &ptr->pcr);
		DBGFS_CREATE_RW_X32("itatbdata0", current_dentry, &ptr->itatbdata0);
		DBGFS_CREATE_RW_X32("itatbctr2", current_dentry, &ptr->itatbctr2);
		DBGFS_CREATE_RW_X32("itatbctr1", current_dentry, &ptr->itatbctr1);
		DBGFS_CREATE_RW_X32("itatbctr0", current_dentry, &ptr->itatbctr0);
		DBGFS_CREATE_RW_X32("imcr", current_dentry, &ptr->imcr);
		DBGFS_CREATE_RW_X32("ctsr", current_dentry, &ptr->ctsr);
		DBGFS_CREATE_RW_X32("ctcr", current_dentry, &ptr->ctcr);
		DBGFS_CREATE_WO_X32("lar", current_dentry, &ptr->lar);
		DBGFS_CREATE_RO_X32("lsr", current_dentry, &ptr->lsr);
		DBGFS_CREATE_RO_X32("asr", current_dentry, &ptr->asr);
		DBGFS_CREATE_RO_X32("dir", current_dentry, &ptr->dir);
		DBGFS_CREATE_RO_X32("dtir", current_dentry, &ptr->dtir);
		DBGFS_CREATE_RO_X32("pidr4", current_dentry, &ptr->pidr4);
		DBGFS_CREATE_RO_X32("pidr5", current_dentry, &ptr->pidr5);
		DBGFS_CREATE_RO_X32("pidr6", current_dentry, &ptr->pidr6);
		DBGFS_CREATE_RO_X32("pidr7", current_dentry, &ptr->pidr7);
		DBGFS_CREATE_RO_X32("pidr0", current_dentry, &ptr->pidr0);
		DBGFS_CREATE_RO_X32("pidr1", current_dentry, &ptr->pidr1);
		DBGFS_CREATE_RO_X32("pidr2", current_dentry, &ptr->pidr2);
		DBGFS_CREATE_RO_X32("pidr3", current_dentry, &ptr->pidr3);

		for ( i = 0; i < FUNNEL_COMPONENTS; i++ ) {
			sprintf(reg_name, "cidr%d", i);
			DBGFS_CREATE_RO_X32(reg_name, current_dentry, &ptr->cidr[i]);
		}
	}

	return 0;
}

