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

#ifndef LS_DBG_REG_ACCESS_H
#define LS_DBG_REG_ACCESS_H

#include <linux/kernel.h>
#include "reg_access_ioctl.h"

/* EPU Reservation Register group offsets */
#define LS_DBG_EPU_CNTR_RES_OFFSET_IND 0
#define LS_DBG_EPU_GRP_RES_OFFSET_IND 4
#define LS_DBG_EPU_SCU_EV_RES_OFFSET_IND 8
#define LS_DBG_EPU_GDPESCR_RES_OFFSET_IND 32

int reg_access_init(void);
int reg_add_map(enum TRACEIP_MODULE id, unsigned long addr, unsigned long size);
void reg_access_cleanup(void);
__u32 ls_dbg_read_reg_internal(enum TRACEIP_MODULE module_id, __u32 offset,
									__u64 *out_val);

__u32 ls_dbg_write_reg_internal(enum TRACEIP_MODULE module_id, __u32 offset,
									__u64 in_val);

#endif  /* LS_DBG_REG_ACCESS_H */
