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
#ifndef DCSR_CORE_LS1_H
#define DCSR_CORE_LS1_H

#include "common.h"

#define LS_DBG_NUM_CORES 2
#define LS_DBG_CORE_NUM_CNTRS 4
#define LS_DBG_CORE_NUM_PMC_IDS 4
#define LS_DBG_CORE_NUM_CM_EV_IDS 2

struct core_pmu_struct {
	u8  reserved[0x1000];
	u32 pmxevcntr[LS_DBG_CORE_NUM_CNTRS];
	u8  reserved1[0x7B - 0xF];
	u32 pmccntr;
	u8  reserved2[0x3FF - 0x7F];
	u32 pmxevtyper[LS_DBG_CORE_NUM_CNTRS];
	u8  reserved3[0x47B - 0x40F];
	u32 pmxevtyper31;
	u8  reserved4[0xBFF - 0x47F];
	u32 pmctenset;
	u8  reserved5[0xC1F - 0xC03];
	u32 pmcntenclr;
	u8  reserved6[0xC3F - 0xC23];
	u32 pmintenset;
	u8  reserved7[0xC5F - 0xC43];
	u32 pmintenclr;
	u8  reserved8[0xC7F - 0xC63];
	u32	pmovsr;
	u8  reserved9[0xC9F - 0xC83];
   	u32	pmswinc;
	u8  reserved10[0xDFF - 0xCA3];
   	u32	pmcfgr;
   	u32 pmcr;
   	u32	pmuserenr;
	u8  reserved11[0xE1F - 0xE0B];
   	u32	pmceid[LS_DBG_CORE_NUM_CM_EV_IDS];
	u8  reserved12[0xFAF - 0xE27];
   	u32	pmlar;
   	u32	pmlsr;
   	u32 pmauthstatus;
	u8  reserved13[0xFCB - 0xFBB];
   	u32	pmdevtype;
	u32	pmpid4;
	u32	pmpid5;
	u32	pmpid6;
	u32	pmpid7;
	u32	pmpid0;
	u32	pmpid1;
	u32	pmpid2;
	u32	pmpid3;
	u32	pmcid[LS_DBG_CORE_NUM_PMC_IDS];
} PACKED;
CTASSERT(sizeof(struct core_pmu_struct) == 0x2000);

struct core_pmu {
	struct core_pmu_struct pmu_regs[LS_DBG_NUM_CORES];
} PACKED;
CTASSERT(sizeof(struct core_pmu) == LS_DBG_NUM_CORES*0x2000);


#endif /* DCSR_CORE_LS1_H */
