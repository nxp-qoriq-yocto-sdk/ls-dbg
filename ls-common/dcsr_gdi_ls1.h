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
#ifndef DCSR_GDI_LS1_H
#define DCSR_GDI_LS1_H

#include "common.h"

#define GDI_LS1_NO_OF_GDI_EVNTS 32
#define GDI_LS1_NO_CMP_CNTRL_REG 8

struct gdgpc_struct {
	u32 cr;
	u32 mr;
	u32 vr;
	u32 reserved;
};
/* Note: GDI 4.0 block guide is incorrect. The following offsets match RTL. */
struct gdi {
	u32 gdcr;
	u32 gdsr;
	u8  reserved1[0xFF-0x07];
	u32 gdrescr1;
	u32 gdrescr2;
   	u8  reserved2[0x1FF-0x107];
	u32 gdpescr[GDI_LS1_NO_OF_GDI_EVNTS];
	u8  reserved3[0x2FF-0x27F];
	struct gdgpc_struct gdgpc[GDI_LS1_NO_CMP_CNTRL_REG];
}PACKED;
CTASSERT(sizeof(struct gdi) == 0x380);


#endif /* DCSR_GDI_LS1_H */
