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

#include "dcsr_etf.h"

/* Driver Initialization Functions */
int dcsr_etf_ls1_init(struct dentry *parent_dentry, struct dbg_device *dev)
{
	struct dentry *current_dentry;
	struct dentry *de;
	struct etf *ptr;
	int i;

	for (i = 0; i < ETF_NUM_DEVICES; i++) {
		ptr = (struct etf *)dev->mem_ptr[i];

		CREATE_CURRENT_DBGFS_DIR_INDEXED(parent_dentry, dev,
						 DEBUGFS_ETF_NAME, i);

		DBGFS_CREATE_RO_X32("rsz", current_dentry, &ptr->rsz);
		DBGFS_CREATE_RO_X32("sts", current_dentry, &ptr->sts);
		DBGFS_CREATE_RO_X32("rrd", current_dentry, &ptr->rrd);
		DBGFS_CREATE_RW_X32("rrp", current_dentry, &ptr->rrp);
		DBGFS_CREATE_RW_X32("rwp", current_dentry, &ptr->rwp);
		DBGFS_CREATE_RW_X32("trg", current_dentry, &ptr->trg);
		DBGFS_CREATE_RW_X32("ctl", current_dentry, &ptr->ctl);
		DBGFS_CREATE_WO_X32("rwd", current_dentry, &ptr->rwd);
		DBGFS_CREATE_RW_X32("mode", current_dentry, &ptr->mode);
		DBGFS_CREATE_RO_X32("lbuflevel", current_dentry, &ptr->lbuflevel);
		DBGFS_CREATE_RO_X32("cbuflevel", current_dentry, &ptr->cbuflevel);
		DBGFS_CREATE_RW_X32("bufwm", current_dentry, &ptr->bufwm);
		DBGFS_CREATE_RW_X32("rrphi", current_dentry, &ptr->rrphi);
		DBGFS_CREATE_RW_X32("rwphi", current_dentry, &ptr->rwphi);
		DBGFS_CREATE_RW_X32("axictl", current_dentry, &ptr->axictl);
		DBGFS_CREATE_RW_X32("dbalo", current_dentry, &ptr->dbalo);
		DBGFS_CREATE_RW_X32("dbahi", current_dentry, &ptr->dbahi);
		DBGFS_CREATE_RO_X32("ffsr", current_dentry, &ptr->ffsr);
		DBGFS_CREATE_RW_X32("ffcr", current_dentry, &ptr->ffcr);
		DBGFS_CREATE_RW_X32("pscr", current_dentry, &ptr->pscr);

		DBGFS_CREATE_WO_X32("itatbmdata0", current_dentry, &ptr->itatbmdata0);

		DBGFS_CREATE_RO_X32("itatbmctr2", current_dentry, &ptr->itatbmctr2);
		DBGFS_CREATE_WO_X32("itatbmctr1", current_dentry, &ptr->itatbmctr1);
		DBGFS_CREATE_WO_X32("itatbmctr0", current_dentry, &ptr->itatbmctr0);

		DBGFS_CREATE_WO_X32("itmiscop0", current_dentry, &ptr->itmiscop0);
		DBGFS_CREATE_RO_X32("ittrflin", current_dentry, &ptr->ittrflin);
		DBGFS_CREATE_RO_X32("itatbdata0", current_dentry, &ptr->itatbdata0);

		DBGFS_CREATE_WO_X32("itatbctr2", current_dentry, &ptr->itatbctr2);
		DBGFS_CREATE_RO_X32("itatbctr1", current_dentry, &ptr->itatbctr1);
		DBGFS_CREATE_RO_X32("itatbctr0", current_dentry, &ptr->itatbctr0);

		DBGFS_CREATE_RW_X32("itctrl", current_dentry, &ptr->itctrl);
		DBGFS_CREATE_RW_X32("claimset", current_dentry, &ptr->claimset);
		DBGFS_CREATE_RW_X32("claimclr", current_dentry, &ptr->claimclr);

		DBGFS_CREATE_WO_X32("lar", current_dentry, &ptr->lar);
		DBGFS_CREATE_RO_X32("lsr", current_dentry, &ptr->lsr);
		DBGFS_CREATE_RO_X32("authstatus", current_dentry, &ptr->authstatus);
		DBGFS_CREATE_RO_X32("devid", current_dentry, &ptr->devid);
		DBGFS_CREATE_RO_X32("devtype", current_dentry, &ptr->devtype);

		DBGFS_CREATE_RO_X32("periphid4", current_dentry, &ptr->periphid4);
		DBGFS_CREATE_RO_X32("periphid5", current_dentry, &ptr->periphid5);
		DBGFS_CREATE_RO_X32("periphid6", current_dentry, &ptr->periphid6);
		DBGFS_CREATE_RO_X32("periphid7", current_dentry, &ptr->periphid7);
		DBGFS_CREATE_RO_X32("periphid0", current_dentry, &ptr->periphid0);
		DBGFS_CREATE_RO_X32("periphid1", current_dentry, &ptr->periphid1);
		DBGFS_CREATE_RO_X32("periphid2", current_dentry, &ptr->periphid2);
		DBGFS_CREATE_RO_X32("periphid3", current_dentry, &ptr->periphid3);

		DBGFS_CREATE_RO_X32("compid0", current_dentry, &ptr->compid0);
		DBGFS_CREATE_RO_X32("compid1", current_dentry, &ptr->compid1);
		DBGFS_CREATE_RO_X32("compid2", current_dentry, &ptr->compid2);
		DBGFS_CREATE_RO_X32("compid3", current_dentry, &ptr->compid3);
	}

	return 0;
}

