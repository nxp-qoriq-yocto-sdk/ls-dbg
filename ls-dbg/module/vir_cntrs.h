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
#ifndef VIR_CNTRS_H
#define VIR_CNTRS_H

#include "dbg_private.h"
#include "dbg_device.h"
#include "dcsr_epu_v2.h"

#define LS_DBG_EPU_EPCTRISR_OFFSET	0x030
#define LS_DBG_EPU_EPECR_OFFSET 0x300
#define LS_DBG_EPU_EPGACR_OFFSET 0x480
#define LS_DBG_EPU_EPCTRGCR_OFFSET 0x540
#define LS_DBG_EPU_EPCCR_OFFSET 0x800
#define LS_DBG_EPU_EPCTR_OFFSET 0x0A00
#define LS_DBG_EPU_EPCAPR_OFFSET 0xB00

#define LS_DBG_V_COUNTER_SIZE 64
#define LS_DBG_REAL_COUNTER_SIZE 32

/* Vcounter register summary:
 *
 * vnctrctrl -- global configuration for vcount regs
 * vcntepecrI -- mirror epecr0-15 registers. When STS is set, infer signal,
 * 					and group. Initiate capture, detecting overflow. Write
 * 					vcntrcaptI regs accordingly
 * vnctrI -- 32b registers holding the OVERFLOW (only) for epctrI
 * vnctrcaptI -- 64b registers that contain the combined overflow count
 * 					(in high order 32b) and captured value (low order 32b)
 *
 * Reservation Registers
 * Set to indicate "in use"; clear to relinquish
 * 	vcntrres -- counter usage reservation register. (32b used)
 * 	vcntrevtres -- event usage reservation register; (16b used)
 * 	vcntrgrpres -- group reservation register; (4b used)
 *
 * 	Notes:
 * 	 1. groups are defined by caller by writing to appropriate epu regs
 * 	 2. "Reservation Registers" are to facilitate cooperative sharing of the
 * 	 	counters. There is no enforcement to prevent someone from "taking"
 * 	 	counters already being used. Instead, users should check vcntrres for
 * 	 	availability.
 * 
 */

/* vcntrctrl description
 * Controls function of virtual counter
 * Bit Field		Description
 *     	 0			0 -> Enable; Binds ISR to epctr overflow
 *     				1 -> Disable; Unbinds ISR from epctr overflow
 *     2-31			Unused
 */

#define VCNTR_ENABLE_MASK 0x00000001

/* TODO: determine the correct shift value (1 or 24) */
#define EPGACR_CTCC_SHIFT 20
#define EPGACR_CTCC_MASK 0x00000070
#define EPECR_STS_MASK 0x01000000

/* EPCCR */
#define EPCCR_ENABLE_MASK 0x00000080
#define EPCCR_LEV_MASK    0x00008000
#define EPCCR_AC_MASK     0x00000F00

#define EPCCR_AC_INTR_REQ_VAL 0x1
#define EPCCR_AC_SHIFT  8
#define DEBUGFS_VCOUNTERS_NAME "vcounters"
#define LS_DBG_V_COUNTER_OVFLOW_VALUE (1ULL << (LS_DBG_V_COUNTER_SIZE - 1))

struct vcounters {
	u32 vcntctrl; /* vcounter control register */
	u32 vcntrres; /* vcntr usage; must be >= EPU_V2_NO_OF_COUNTERS bits */
	u32 vcntrevtres; /* event usage */
	u32 vcntrgrpres; /* group usage */
	u32 vcntrepecr[EPU_V2_NO_OF_SCU_EVENTS];
	u64 vcntr[EPU_V2_NO_OF_COUNTERS];
	u64 vcntrcapt[EPU_V2_NO_OF_COUNTERS];
};

/* Driver Initialization Functions */
extern int vcounters_init(struct dentry *parent_dentry);
extern u32 vcounter_reg_read(void *data, u64 *val);
extern u32 vcounter_reg_write(void *data, u64 val);

#endif /* VIR_CNTRS_H */
