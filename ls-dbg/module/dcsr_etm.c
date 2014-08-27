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

#include "dcsr_etm.h"

/* Driver Initialization Functions */
int dcsr_etm_ls1_init(struct dentry *parent_dentry, struct dbg_device *dev)
{
	struct dentry *current_dentry;
	struct dentry *de;
	struct etm *ptr = (struct etm *)dev->mem_ptr[0];
	char reg_name[DBFS_REG_NAME_LEN];
	int i;
	int k;

	for (k = 0; k < ETM_NUM_DEVICES; k++) {
		CREATE_CURRENT_DBGFS_DIR_INDEXED(parent_dentry, dev,
						 DEBUGFS_ETM_NAME, k);

		DBGFS_CREATE_RW_X32("etmcr", current_dentry, &ptr->etmcr);
		DBGFS_CREATE_RO_X32("etmccr", current_dentry, &ptr->etmccr);
		DBGFS_CREATE_RW_X32("etmtrigger", current_dentry, &ptr->etmtrigger);
		DBGFS_CREATE_RW_X32("etmasicctlr", current_dentry, &ptr->etmasicctlr);
		DBGFS_CREATE_RW_X32("etmsr", current_dentry, &ptr->etmsr);
		DBGFS_CREATE_RO_X32("etmscr", current_dentry, &ptr->etmscr);
		DBGFS_CREATE_RW_X32("etmtsscr", current_dentry, &ptr->etmtsscr);
		DBGFS_CREATE_RW_X32("etmtecr2", current_dentry, &ptr->etmtecr2);
		DBGFS_CREATE_RW_X32("etmteevr", current_dentry, &ptr->etmteevr);
		DBGFS_CREATE_RW_X32("etmtecr1", current_dentry, &ptr->etmtecr1);
		DBGFS_CREATE_RW_X32("etmfflr", current_dentry, &ptr->etmfflr);
		DBGFS_CREATE_RW_X32("etmvdevr", current_dentry, &ptr->etmvdevr);
		DBGFS_CREATE_RW_X32("etmvdcr1", current_dentry, &ptr->etmvdcr1);
		DBGFS_CREATE_RW_X32("etmvdcr3", current_dentry, &ptr->etmvdcr3);

		for ( i = 1; i <= ETM_ADDRESS_COMPARATOR_VALUE_REGISTERS; i++) {
			sprintf(reg_name, "etmacvr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->etmacvr[i - 1]);
		}

		for ( i = 1; i <= ETM_ADDRESS_COMPARATOR_ACCESS_REGISTERS; i++) {
			sprintf(reg_name, "etmactr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->etmactr[i - 1]);
		}

		DBGFS_CREATE_RW_X32("etmdcvr1", current_dentry, &ptr->etmdcvr1);
		DBGFS_CREATE_RW_X32("etmdcvr3", current_dentry, &ptr->etmdcvr3);
		DBGFS_CREATE_RW_X32("etmdcmr1", current_dentry, &ptr->etmdcmr1);
		DBGFS_CREATE_RW_X32("etmdcmr3", current_dentry, &ptr->etmdcmr3);

		for ( i = 1; i <= ETM_RELOAD_VALUE_REGISTERS; i++) {
			sprintf(reg_name, "etmcntrldvr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->etmcntrldvr[i - 1]);
		}

		for ( i = 1; i <= ETM_COUNTER_ENABLE_REGISTERS; i++) {
			sprintf(reg_name, "etmcntenr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->etmcntenr[i - 1]);
		}

		for ( i = 1; i <= ETM_COUNTER_RELOAD_EVENT_REGISTERS; i++) {
			sprintf(reg_name, "etmcntrldevr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->etmcntrldevr[i - 1]);
		}

		for ( i = 1; i <= ETM_COUNTER_VALUE_REGISTERS; i++) {
			sprintf(reg_name, "etmcntvr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->etmcntvr[i - 1]);
		}

		for ( i = 1; i <= ETM_SEQUENCER_STATE_REGISTERS; i++) {
			sprintf(reg_name, "etmsqabevr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->etmsqabevr[i - 1]);
		}

		DBGFS_CREATE_RW_X32("etmsqr", current_dentry, &ptr->etmsqr);

		for ( i = 1; i <= ETM_EXTERNAL_OUTPUT_EVENT_REGISTERS; i++) {
			sprintf(reg_name, "etmextoutevr%d", i);
			DBGFS_CREATE_RW_X32(reg_name, current_dentry, &ptr->etmextoutevr[i - 1]);
		}

		DBGFS_CREATE_RW_X32("etmcidcvr1", current_dentry, &ptr->etmcidcvr1);
		DBGFS_CREATE_RW_X32("etmcidcmr", current_dentry, &ptr->etmcidcmr);
		DBGFS_CREATE_RW_X32("etmsyncfr", current_dentry, &ptr->etmsyncfr);
		DBGFS_CREATE_RO_X32("etmidr", current_dentry, &ptr->etmidr);
		DBGFS_CREATE_RO_X32("etmccer", current_dentry, &ptr->etmccer);
		DBGFS_CREATE_RW_X32("etmextinselr", current_dentry, &ptr->etmextinselr);
		DBGFS_CREATE_RW_X32("etmtsevr", current_dentry, &ptr->etmtsevr);
		DBGFS_CREATE_RW_X32("etmauxcr", current_dentry, &ptr->etmauxcr);
		DBGFS_CREATE_RW_X32("etmtraceidr", current_dentry, &ptr->etmtraceidr);
		DBGFS_CREATE_RO_X32("etmidr2", current_dentry, &ptr->etmidr2);
		DBGFS_CREATE_WO_X32("etmvmidcvr", current_dentry, &ptr->etmvmidcvr);
		DBGFS_CREATE_WO_X32("etmoslar", current_dentry, &ptr->etmoslar);
		DBGFS_CREATE_RO_X32("etmoslsr", current_dentry, &ptr->etmoslsr);
		DBGFS_CREATE_RW_X32("etmossrr", current_dentry, &ptr->etmossrr);
		DBGFS_CREATE_RW_X32("etmpdcr", current_dentry, &ptr->etmpdcr);
		DBGFS_CREATE_RO_X32("etmpdsr", current_dentry, &ptr->etmpdsr);
		DBGFS_CREATE_WO_X32("itmiscout", current_dentry, &ptr->itmiscout);
		DBGFS_CREATE_RO_X32("itmiscin", current_dentry, &ptr->itmiscin);
		DBGFS_CREATE_WO_X32("ittriggerreq", current_dentry, &ptr->ittriggerreq);
		DBGFS_CREATE_WO_X32("itatbdata0", current_dentry, &ptr->itatbdata0);
		DBGFS_CREATE_RO_X32("itatbctr2", current_dentry, &ptr->itatbctr2);
		DBGFS_CREATE_WO_X32("itatbctr1", current_dentry, &ptr->itatbctr1);
		DBGFS_CREATE_WO_X32("itatbctr0", current_dentry, &ptr->itatbctr0);
		DBGFS_CREATE_RW_X32("etmitctrl", current_dentry, &ptr->etmitctrl);
		DBGFS_CREATE_RW_X32("etmclaimset", current_dentry, &ptr->etmclaimset);
		DBGFS_CREATE_RW_X32("etmclaimclr", current_dentry, &ptr->etmclaimclr);
		DBGFS_CREATE_WO_X32("etmlar", current_dentry, &ptr->etmlar);
		DBGFS_CREATE_RO_X32("etmlsr", current_dentry, &ptr->etmlsr);
		DBGFS_CREATE_RO_X32("etmauthstatus", current_dentry, &ptr->etmauthstatus);
		DBGFS_CREATE_RO_X32("etmdevid", current_dentry, &ptr->etmdevid);
		DBGFS_CREATE_RO_X32("etmdevtype", current_dentry, &ptr->etmdevtype);
		DBGFS_CREATE_RO_X32("etmpidr4", current_dentry, &ptr->etmpidr4);
		DBGFS_CREATE_RO_X32("etmpidr5", current_dentry, &ptr->etmpidr5);
		DBGFS_CREATE_RO_X32("etmpidr6", current_dentry, &ptr->etmpidr6);
		DBGFS_CREATE_RO_X32("etmpidr7", current_dentry, &ptr->etmpidr7);
		DBGFS_CREATE_RO_X32("etmpidr0", current_dentry, &ptr->etmpidr0);
		DBGFS_CREATE_RO_X32("etmpidr1", current_dentry, &ptr->etmpidr1);
		DBGFS_CREATE_RO_X32("etmpidr2", current_dentry, &ptr->etmpidr2);
		DBGFS_CREATE_RO_X32("etmpidr3", current_dentry, &ptr->etmpidr3);

		for ( i = 1; i <= ETM_COMPONENT_IDENT_REGISTERS; i++) {
			sprintf(reg_name, "etmcidr%d", i);
			DBGFS_CREATE_RO_X32(reg_name, current_dentry, &ptr->etmcidr[i - 1]);
		}

	}

	return 0;
}

