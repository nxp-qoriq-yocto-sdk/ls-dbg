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
#ifndef DCSR_ETF_LS1_H
#define DCSR_ETF_LS1_H

#include "common.h"

#define DEBUGFS_ETF_NAME	"etf"

/* structure defines the memory map of the ETF control block */
struct etf {
    u8 reserved0[0x004-0x000];
    /* RAM Size Register */
    u32 rsz;
    u8 reserved1[0x00c-0x008];
    /* Status Register */
    u32 sts;
    /* RAM Read Data Register */
    u32 rrd;
    /* RAM Read Pointer Register */
    u32 rrp;
    /* RAM Write Pointer Register */
    u32 rwp;
    /* Trigger Counter Register */
    u32 trg;
    /* Control Register */
    u32 ctl;
    /* RAM Write Data Register */
    u32 rwd;
    /* Mode Register */
    u32 mode;
    /* Latched Buffer Fill Level */
    u32 lbuflevel;
    /* Current Buffer Fill Level */
    u32 cbuflevel;
    /* Buffer Level Water Mark */
    u32 bufwm;
    /* RAM Read Pointer High Register */
    u32 rrphi;
    /* RAM Write Pointer High Register */
    u32 rwphi;
    u8 reserved2[0x110-0x040];
    /* AXI Control Register */
    u32 axictl;
    u8 reserved3[0x118-0x114];
    /* Data Buffer Address Low Register */
    u32 dbalo;
    /* Data Buffer Address High Register */
    u32 dbahi;
    u8 reserved4[0x300-0x0120];
    /* Formatter and Flush Status Register */
    u32 ffsr;
    /* Formatter and Flush Control Register */
    u32 ffcr;
    /* Periodic Synchronization Counter Register */
    u32 pscr;
    u8 reserved5[0xed0-0x030c];
    /* Integration Test ATB Master Data Register 0 */
    u32 itatbmdata0;
    /* Integration Test ATB Master Interface Control 2 Register */
    u32 itatbmctr2;
    /* Integration Test ATB Master Control Register 1 */
    u32 itatbmctr1;
    /* Integration Test ATB Master Interface Control 0 Register */
    u32 itatbmctr0;
    /* Integration Test Miscellaneous Output Register 0 */
    u32 itmiscop0;
    u8 reserved6[0xee8-0xee4];
    /* Integration Test Trigger In and Flush In Register */
    u32 ittrflin;
    /* Integration Test ATB Data Register 0 */
    u32 itatbdata0;
    /* Integration Test ATB Control 2 Register */
    u32 itatbctr2;
    /* Integration Test ATB Control 1 Register */
    u32 itatbctr1;
    /* Integration Test ATB Control 0 Register */
    u32 itatbctr0;
    u8 reserved7[0xf00-0xefc];
    /* Integration Mode Control Register */
    u32 itctrl;
    u8 reserved8[0xfa0-0xf04];
    /* Claim Tag Set Register */
    u32 claimset;
    /* Claim Tag Clear Register */
    u32 claimclr;
    u8 reserved9[0xfb0-0xfa8];
    /* Lock Access Register */
    u32 lar;
    /* Lock Status Register */
    u32 lsr;
    /* Authentication Status Register */
    u32 authstatus;
    u8 reserved10[0xfc8-0xfbc];
    /* Device Configuration Register */
    u32 devid;
    /* Device Type Identifier Register */
    u32 devtype;
    /* Peripheral ID4 Register */
    u32 periphid4;
    /* Peripheral ID5 Register */
    u32 periphid5;
    /* Peripheral ID6 Register */
    u32 periphid6;
    /* Peripheral ID7 Register */
    u32 periphid7;
    /* Peripheral ID0 Register */
    u32 periphid0;
    /* Peripheral ID1 Register */
    u32 periphid1;
    /* Peripheral ID2 Register */
    u32 periphid2;
    /* Peripheral ID3 Register */
    u32 periphid3;
    /* Component ID0 Register */
    u32 compid0;
    /* Component ID1 Register */
    u32 compid1;
    /* Component ID2 Register */
    u32 compid2;
    /* Component ID3 Register */
    u32 compid3;
} PACKED;
CTASSERT(sizeof(struct etf) == 0x1000);

#endif /* DCSR_ETF_LS1_H */
