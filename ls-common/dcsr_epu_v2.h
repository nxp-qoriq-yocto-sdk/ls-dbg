/*
 * Copyright (C) 2013 Freescale Semiconductor, Inc.
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
#ifndef DCSR_EPU_V2_H
#define DCSR_EPU_V2_H

#include "common.h"

/* Number of counting units in the EPU */
#define EPU_V2_NO_OF_COUNTERS		32
#define EPU_V2_NO_OF_SCU_EVENTS		16
#define EPU_V2_NO_OF_COUNTER_GROUPS	4
#define EPU_V2_NO_OF_EVENT_GROUPS	3
#define EPU_V2_NO_PROC_INTRPTS		4

#define EPU_V2_NO_EVT_PIN_REGS		10
#define EPU_V2_NO_FSM_STATES		8
#define EPU_V2_NO_RESRV_REGS		60
#define EPU_V2_NO_SEMA_REGS		4

struct epu_v2 {
	/* Global Control Register */
	u32 epgcr;
	u8 reserved1[0x10-0x04];
	/* Event Status Register */
	u32 epesr;
	u8 reserved2[0x20-0x14];
	/* Interrupt Status Registers */
	u32 episr[EPU_V2_NO_PROC_INTRPTS];
	/* Counter Interrupt Status Register */
	u32 epctrisr;
	u8 reserved3[0x40-0x34];
	/* Counter Capture Status Register */
	u32 epctrcsr;
	u8 reserved4[0x50-0x44];
	/* EVT Pin Control Register */
	u32 epevtcr[EPU_V2_NO_EVT_PIN_REGS];
	u8 reserved5[0x90-0x78];
	/* Cross Trigger Control Register */
	u32 epxtrigcr;
	u8 reserved6[0x100-0x94];
	/* Event Processor Input Mux Control Register */
	u32 epimcr[EPU_V2_NO_OF_COUNTERS];
	u8 reserved7[0x200-0x180];
	/* Event Processor SCU MUX Control Registers */
	struct {
		u32 epsmcr;
		u32 reserved;
	} scu_ctrl[EPU_V2_NO_OF_SCU_EVENTS];
	u8 reserved8[0x300-0x280];
	/* Event Processor Event Control Registers */
	u32 epecr[EPU_V2_NO_OF_SCU_EVENTS];
	u8 reserved9[0x400-0x340];
	/* Event Processor Action Control Registers */
	u32 epacr[EPU_V2_NO_OF_SCU_EVENTS];
	u8 reserved10[0x480-0x440];
	/* Event Processor Group Action Control Registers */
	u32 epgacr[EPU_V2_NO_OF_SCU_EVENTS];
	u8 reserved11[0x540-0x4c0];
	/* Event Processor Counter Group Configuration Registers */
	u32 epctrgcr[EPU_V2_NO_OF_COUNTER_GROUPS];
	u8 reserved12[0x580-0x550];
	/* Event Processor Event Group Configuration Registers */
	u32 epegcr[EPU_V2_NO_OF_EVENT_GROUPS];
	u8 reserved13[0x600-0x58c];
	/* Event Processor FSM Status */
	u32 epfsmsr0;
	u8 reserved14[0x610-0x604];
	/* Event Processor FSM Compare */
	u32 epfsmcmpr[EPU_V2_NO_FSM_STATES];
	/* Event Processor FSM Control */
	u32 epfsmcr[EPU_V2_NO_FSM_STATES];
	u8 reserved15[0x800-0x650];
	/* Event Processor Counter Control Registers */
	u32 epccr[EPU_V2_NO_OF_COUNTERS];
	u8 reserved16[0x900-0x880];
	/* Event Processor Counter Compare Registers */
	u32 epcmpr[EPU_V2_NO_OF_COUNTERS];
	u8 reserved17[0xA00-0x980];
	/* Event Processor Counter Registers */
	u32 epctr[EPU_V2_NO_OF_COUNTERS];
	u8 reserved18[0xB00-0xA80];
	/* Event Processor Counter Capture Registers */
	u32 epcapr[EPU_V2_NO_OF_COUNTERS];
	u8 reserved19[0xF00-0xB80];
	/* Event Processor Debug Reservation Register */
	u32 eprsv[EPU_V2_NO_RESRV_REGS];
	/* Hardware Semaphore Register */
	u32 ephsr[EPU_V2_NO_SEMA_REGS];
} PACKED;
CTASSERT(sizeof(struct epu_v2) == 0x1000);

#endif /* DCSR_EPU_V2_H */
