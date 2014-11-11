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

#include "dcsr_core.h"

/* Driver Initialization Functions */
int dcsr_core_ls1_init(struct dentry *parent_dentry, struct dbg_device *dev)
{
	struct dentry *current_dentry;
	struct dentry *de;
	struct core_pmu *ptr = (struct core_pmu *)dev->mem_ptr[0];
	char reg_name[DBFS_REG_NAME_LEN];
	int i, j;

	for (j = 0; j < LS_DBG_NUM_CORES; ++j) {
		sprintf(reg_name, "%s%d", DEBUGFS_CORE_NAME, j);
		CREATE_CURRENT_DBGFS_DIR(parent_dentry, dev, reg_name);

		for (i = 0; i < LS_DBG_CORE_NUM_CNTRS; ++i) {
			sprintf(reg_name, "pmxevcntr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->pmu_regs[j].pmxevcntr[i]);
		}
		
		DBGFS_CREATE_RW_X32("pmccntr", current_dentry, &ptr->pmu_regs[j].pmccntr);

		for (i = 0; i < LS_DBG_CORE_NUM_CNTRS; ++i) {
			sprintf(reg_name, "pmxevtyper%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->pmu_regs[j].pmxevtyper[i]);
		}

		DBGFS_CREATE_RW_X32("pmxevtyper31", current_dentry, &ptr->pmu_regs[j].pmxevtyper31);
		DBGFS_CREATE_RW_X32("pmctenset", current_dentry, &ptr->pmu_regs[j].pmctenset);
		DBGFS_CREATE_RW_X32("pmcntenclr", current_dentry, &ptr->pmu_regs[j].pmcntenclr);
		DBGFS_CREATE_RW_X32("pmintenset", current_dentry, &ptr->pmu_regs[j].pmintenset);
		DBGFS_CREATE_RW_X32("pmintenclr", current_dentry, &ptr->pmu_regs[j].pmintenclr);
		DBGFS_CREATE_RW_X32("pmovsr", current_dentry, &ptr->pmu_regs[j].pmovsr);
		DBGFS_CREATE_WO_X32("pmswinc", current_dentry, &ptr->pmu_regs[j].pmswinc);
		DBGFS_CREATE_RO_X32("pmcfgr", current_dentry, &ptr->pmu_regs[j].pmcfgr);
		DBGFS_CREATE_RW_X32("pmcr", current_dentry, &ptr->pmu_regs[j].pmcr);
		DBGFS_CREATE_RW_X32("pmuserenr", current_dentry, &ptr->pmu_regs[j].pmuserenr);

		for (i = 0; i < LS_DBG_CORE_NUM_CM_EV_IDS; ++i) {
			sprintf(reg_name, "pmceid%d", i);
			DBGFS_CREATE_RO_X32(reg_name, current_dentry, &ptr->pmu_regs[j].pmceid[i]);
		}

		DBGFS_CREATE_WO_X32("pmlar", current_dentry, &ptr->pmu_regs[j].pmlar);
		DBGFS_CREATE_RO_X32("pmlsr", current_dentry, &ptr->pmu_regs[j].pmlsr);
		DBGFS_CREATE_RO_X32("pmauthstatus", current_dentry, &ptr->pmu_regs[j].pmauthstatus);
		DBGFS_CREATE_RO_X32("pmdevtype", current_dentry, &ptr->pmu_regs[j].pmdevtype);
		DBGFS_CREATE_RO_X32("pmpid4", current_dentry, &ptr->pmu_regs[j].pmpid4);
		DBGFS_CREATE_RO_X32("pmpid5", current_dentry, &ptr->pmu_regs[j].pmpid5);
		DBGFS_CREATE_RO_X32("pmpid6", current_dentry, &ptr->pmu_regs[j].pmpid6);
		DBGFS_CREATE_RO_X32("pmpid7", current_dentry, &ptr->pmu_regs[j].pmpid7);
		DBGFS_CREATE_RO_X32("pmpid0", current_dentry, &ptr->pmu_regs[j].pmpid0);
		DBGFS_CREATE_RO_X32("pmpid1", current_dentry, &ptr->pmu_regs[j].pmpid1);
		DBGFS_CREATE_RO_X32("pmpid2", current_dentry, &ptr->pmu_regs[j].pmpid2);
		DBGFS_CREATE_RO_X32("pmpid3", current_dentry, &ptr->pmu_regs[j].pmpid3);
		
		for (i = 0; i < LS_DBG_CORE_NUM_PMC_IDS; ++i) {
			sprintf(reg_name, "pmcid%d", i);
			DBGFS_CREATE_RO_X32(reg_name, current_dentry, &ptr->pmu_regs[j].pmcid[i]);
		}
	}
	
	return 0;
}

