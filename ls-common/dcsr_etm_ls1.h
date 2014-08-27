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
#ifndef DCSR_ETM_LS1_H
#define DCSR_ETM_LS1_H

#include "common.h"

#define MERGE_(a,b)  a##b

#define ETM_RESERVED_(id, begin, end)\
    u32 MERGE_(reserved, id) [(end) - (begin) - 1]

#define ETM_RESERVED(begin, end)\
    ETM_RESERVED_(__LINE__, begin, end)

#define ETM_ADDRESS_COMPARATOR_VALUE_REGISTERS      8
#define ETM_ADDRESS_COMPARATOR_ACCESS_REGISTERS     8
#define ETM_RELOAD_VALUE_REGISTERS                  2
#define ETM_COUNTER_ENABLE_REGISTERS                2
#define ETM_COUNTER_RELOAD_EVENT_REGISTERS          2
#define ETM_COUNTER_VALUE_REGISTERS                 2
#define ETM_EXTERNAL_OUTPUT_EVENT_REGISTERS         2
#define ETM_COMPONENT_IDENT_REGISTERS               4
#define ETM_SEQUENCER_STATE_REGISTERS               6


/* structure defines the memory map of the ETM control block */
struct etm {
    /* Main Control Register */
    u32 etmcr;
    /* Configuration Code Register */
    u32 etmccr;
    /* Trigger Event Register */
    u32 etmtrigger;
    /* ASIC Control Register */
    u32 etmasicctlr;
    /* ETM Status Register */
    u32 etmsr;
    /* System Configuration Register */
    u32 etmscr;
    /* TraceEnable Start/Stop Control Register */
    u32 etmtsscr;
    /* TraceEnable Control 2 Register */
    u32 etmtecr2;
    /* TraceEnable Event Register */
    u32 etmteevr;
    /* TraceEnable Control 1 Register */
    u32 etmtecr1;
    ETM_RESERVED(0x009, 0x00b);
    /* FIFOFULL Level Register */
    u32 etmfflr;
    /* ViewData Event Register */
    u32 etmvdevr;
    /* ViewData Control 1 Register */
    u32 etmvdcr1;
    ETM_RESERVED(0x00D, 0x00F);
    /* ViewData Control 3 Register */
    u32 etmvdcr3;
    /* Address Comparator Value Registers 1-8 */
    u32 etmacvr[ETM_ADDRESS_COMPARATOR_VALUE_REGISTERS];
    ETM_RESERVED(0x017, 0x020);
    /* Address Comparator Access Type Registers 1-8 */
    u32 etmactr[ETM_ADDRESS_COMPARATOR_ACCESS_REGISTERS];
    ETM_RESERVED(0x027, 0x030);
    /* Data Comparator Value Register 1 */
    u32 etmdcvr1;
    ETM_RESERVED(0x030, 0x032);
    /* Data Comparator Value Register 3 */
    u32 etmdcvr3;
    ETM_RESERVED(0x032, 0x040);
    /* Data Comparator Mask Register 1 */
    u32 etmdcmr1;
    ETM_RESERVED(0x040, 0x042);
    /* Data Comparator Mask Register 3 */
    u32 etmdcmr3;
    ETM_RESERVED(0x042, 0x050);
    /* Counter Reload Value Registers 1-2 */
    u32 etmcntrldvr[ETM_RELOAD_VALUE_REGISTERS];
    ETM_RESERVED(0x051, 0x054);
    /* Counter Enable Registers 1-2 */
    u32 etmcntenr[ETM_COUNTER_ENABLE_REGISTERS];
    ETM_RESERVED(0x055, 0x058);
    /* Counter Reload Event Registers 1-2 */
    u32 etmcntrldevr[ETM_COUNTER_RELOAD_EVENT_REGISTERS];
    ETM_RESERVED(0x059, 0x05C);
    /* Counter Value Registers 1-2 */
    u32 etmcntvr[ETM_COUNTER_VALUE_REGISTERS];
    ETM_RESERVED(0x05D, 0x060);
    /* Sequencer State Transition Event Registers */
    u32 etmsqabevr[ETM_SEQUENCER_STATE_REGISTERS];
    ETM_RESERVED(0x065, 0x067);
    /* Current Sequencer State Register */
    u32 etmsqr;
    /* External Output Event Registers 1-2 */
    u32 etmextoutevr[ETM_EXTERNAL_OUTPUT_EVENT_REGISTERS];
    ETM_RESERVED(0x069, 0x06C);
    /* Context ID Comparator Value 1 Register */
    u32 etmcidcvr1;
    ETM_RESERVED(0x06C, 0x06F);
    /* Context ID Comparator Mask Register */
    u32 etmcidcmr;
    ETM_RESERVED(0x06F, 0x078);
    /* Synchronization Frequency Register */
    u32 etmsyncfr;
    /* ID Register */
    u32 etmidr;
    /* Configuration Code Extension Register */
    u32 etmccer;
    /* Extended External Input Selection Register */
    u32 etmextinselr;
    ETM_RESERVED(0x07B, 0x07E);
    /* Timestamp Event Register */
    u32 etmtsevr;
    /* Auxiliary Control Register */
    u32 etmauxcr;
    /* CoreSight Trace ID Register */
    u32 etmtraceidr;
    ETM_RESERVED(0x080, 0x082);
    /* ETM ID Register 2 */
    u32 etmidr2;
    ETM_RESERVED(0x082, 0x090);
    /* VMID Comparator Value Register */
    u32 etmvmidcvr;
    ETM_RESERVED(0x090, 0x0C0);
    /* OS Lock Access Register */
    u32 etmoslar;
    /* OS Lock Status Register */
    u32 etmoslsr;
    /* OS Save and Restore Register */
    u32 etmossrr;
    ETM_RESERVED(0x0C2, 0x0C4);
    /* Power-Down Control Register */
    u32 etmpdcr;
    /* Power-Down Status Register */
    u32 etmpdsr;
    ETM_RESERVED(0x0C5, 0x3B7);
    /* Integration Test Miscellaneous Outputs register */
    u32 itmiscout;
    /* Integration Test Miscellaneous Input Register */
    u32 itmiscin;
    ETM_RESERVED(0x3B8, 0x3BA);
    /* Integration Test Trigger Request Register */
    u32 ittriggerreq;
    /* Integration Test ATB Data 0 Register */
    u32 itatbdata0;
    /* Integration Test ATB Control Register 2 */
    u32 itatbctr2;
    /* Integration Test ATB Control Register 1 */
    u32 itatbctr1;
    /* Integration Test ATB Control Register 0 */
    u32 itatbctr0;
    ETM_RESERVED(0x3BE, 0x3C0);
    /* Integration Mode Control Register */
    u32 etmitctrl;
    ETM_RESERVED(0x3C0, 0x3E8);
    /* Claim Tag Set Register */
    u32 etmclaimset;
    /* Claim Tag Clear Register */
    u32 etmclaimclr;
    ETM_RESERVED(0x3E9, 0x3EC);
    /* Lock Access Register */
    u32 etmlar;
    /* Lock Status Register */
    u32 etmlsr;
    /* Authentication Status Register */
    u32 etmauthstatus;
    ETM_RESERVED(0x3EE, 0x3F2);
    /* CoreSight Device Configuration Register */
    u32 etmdevid;
    /* CoreSight Device Type Register */
    u32 etmdevtype;
    /* Peripheral identification registers 4-7 */
    u32 etmpidr4;
    u32 etmpidr5;
    u32 etmpidr6;
    u32 etmpidr7;
    /* Peripheral identification registers 0-3 */
    u32 etmpidr0;
    u32 etmpidr1;
    u32 etmpidr2;
    u32 etmpidr3;
    /* Component identification registers */
    u32 etmcidr[ETM_COMPONENT_IDENT_REGISTERS];
} PACKED;
CTASSERT(sizeof(struct etm) == (0x400 * sizeof(u32)));

#endif /* DCSR_ETM_LS1_H */
