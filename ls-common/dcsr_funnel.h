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
#ifndef DCSR_FUNNEL_H
#define DCSR_FUNNEL_H

#include "common.h"

#define MERGE_(a,b)  a##b

#define FUNNEL_RESERVED_(id, begin, end)\
    u8 MERGE_(reserved, id) [(end) - (begin) - sizeof(u32)]

#define FUNNEL_RESERVED(begin, end)\
    FUNNEL_RESERVED_(__LINE__, begin, end)

#define FUNNEL_COMPONENTS       4

/* structure defines the memory map of the FUNNEL control block */
struct funnel {
    /* Funnel Control Register */
    u32 cr;
    /* Priority Control Register */
    u32 pcr;
    FUNNEL_RESERVED(0x004, 0xEEC);
    /* Integration Register */
    u32 itatbdata0;
    /* Integration Register */
    u32 itatbctr2;
    /* Integration Register */
    u32 itatbctr1;
    /* Integration Register */
    u32 itatbctr0;
    FUNNEL_RESERVED(0xEF8, 0xF00);
    /* Integration Mode Control Register */
    u32 imcr;
    FUNNEL_RESERVED(0xF00, 0xFA0);
    /* Claim Tag Set Register */
    u32 ctsr;
    /* Claim Tag Clear Register */
    u32 ctcr;
    FUNNEL_RESERVED(0xFA4, 0xFB0);
    /* Lock Access Register */
    u32 lar;
    /* Lock Status Registter */
    u32 lsr;
    /* Authentication status */
    u32 asr;
    FUNNEL_RESERVED(0xFB8, 0xFC8);
    /* Device ID Register */
    u32 dir;
    /* Device Type Identifier Register */
    u32 dtir;
    /* Peripheral ID4 Register */
    u32 pidr4;
    /* Peripheral ID5 Register */
    u32 pidr5;
    /* Peripheral ID6 Register */
    u32 pidr6;
    /* Peripheral ID7 Register */
    u32 pidr7;
    /* Peripheral ID0 Register */
    u32 pidr0;
    /* Peripheral ID1 Register */
    u32 pidr1;
    /* Peripheral ID2 Register */
    u32 pidr2;
    /* Peripheral ID3 Register */
    u32 pidr3;
    /* Component ID 0-3 Registers */
    u32 cidr[FUNNEL_COMPONENTS];
} PACKED;
CTASSERT(sizeof(struct funnel) == 0x1000);

#endif /* DCSR_FUNNEL_H */
