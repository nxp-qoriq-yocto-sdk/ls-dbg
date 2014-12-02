/*
 * Copyright (C) 2010, 2011, 2012 Freescale Semiconductor, Inc.
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

//#include "dcsr_epu.h"
#include "vir_cntrs.h"
#include "reg_access.h"
#include <linux/interrupt.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>

#include <asm/irq_regs.h>

#ifdef DEBUG_LS_DBG
#include <linux/irqdesc.h>
#endif

struct vcounters ls_dbg_vcnt;

/*
 * EPU counter interrupt (perfmon) handler
 */

/* interrupt information used by the EPU perfmon interrupt */
struct intr {
	int num;
	const char *name;
	irqreturn_t (*handler)(int irq, void *ptr);
};

static int ls_dbg_epu_bind_irq(struct intr *in_intr);
int ls_dbg_epu_unbind_irq(void);

DEFINE_SPINLOCK(vc_lock);

/* Increment the virtual counter and handle roll over */
static int v_counter_inc(int ctr)
{
	int ret;

	/* callers of v_counter_inc() are responsible for spin_lock */
	
	++ls_dbg_vcnt.vcntr[ctr];

	ret = (ls_dbg_vcnt.vcntr[ctr] & LS_DBG_V_COUNTER_OVFLOW_VALUE) ? 1 : 0;
	
	if (unlikely(ret)) {
		ls_dbg_vcnt.vcntr[ctr] = 0;
	}

	return ret;
}
u32 update_vcounters(u32 ctrisr)
{
	int ctr = EPU_V2_NO_OF_COUNTERS - 1;
	u32 cntr_ind;
	u32 ret;

	/* callers responsible for spinlock */
	
	/* clear all the interrupt bits (w1c) */
	ret = ls_dbg_write_reg_internal(EPU, LS_DBG_EPU_EPCTRISR_OFFSET,
																	ctrisr);
	if (unlikely( ret != 0)) {
		printk(KERN_ERR "Write to epctrisr failed.\n");
		return ret;
	}

	/* Shift through all the pending overflows */
	while (ctrisr) {
		if (ctrisr & 1) {
			cntr_ind = EPU_V2_NO_OF_COUNTERS - 1 - ctr;
			(void)v_counter_inc(cntr_ind);
			ret = ls_dbg_write_reg_internal(EPU,
									LS_DBG_EPU_EPCTR_OFFSET + cntr_ind*4, 0);
			if (unlikely(ret != 0)) {
				printk(KERN_ERR "Write to epctrisr failed.\n");
				return ret;
			}
		}
		ctr--;
		ctrisr >>= 1;
	}

	return 0;
}

static irqreturn_t cntr_overflow_interrupt(int irq, void *ptr)
{
	unsigned long flags;
	u32 ctrisr;
	__u32 ret;
	u64 val;
	

	ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPCTRISR_OFFSET, &val);

	ctrisr = (u32)val;
	
	if (!ctrisr && !ret) {
		return IRQ_NONE;
	}
	
	if (unlikely(ret)) {
		printk(KERN_INFO
		 "Failed to read register EPCTRISR in counter overflow interrupt\n");
		return IRQ_HANDLED;
	}

	spin_lock_irqsave(&vc_lock, flags);

	ret = update_vcounters(ctrisr);

	spin_unlock_irqrestore(&vc_lock, flags);

	if (unlikely(ret)) {
		printk(KERN_INFO "Failed to update vcounters\n");
	}

	return IRQ_HANDLED;
}

struct intr intr_overflow = {
		.name = "epu-perfmon-overfl",
		.handler = &cntr_overflow_interrupt,
};

static int vcntr_get(void *data, u64 *val)
{
	*val = *(u64 *)data;
	return 0;
}

static int vcntr_set(void *data, u64 val)
{
	*(u64 *)data = val;
	return 0;
}

static int vcntepecr_set(void *data, u64 val)
{
	u32 event_i;
	u32 epctrgcr;
	u32 epctrisr;
	u32 group_j;
	u32 regVal, regVal2;
	u64 reg64;
	int ret = 0;
	u32 i;
	unsigned long flags;

	/*
	 * -From input EPECRi, determine:
	 * 		-event number "i"
	 * -Read EPGACR_i to determine group to be captured (CTCC), j
	 * -Read EPCTRGCR_j to determine which counters are in the group
	 * -For each counter k in the group
	 * 		-Read epctr_k and record original value in vcntr_k
	 * -Send signal to capture (write EPECRi[STS])
	 * -Read captured registers 
	 * -For each counter k in group
	 * 		-read epctr_k
	 * 		- if captured_val_k < orig_reg_val_k
	 * 			-ASSERT(overflow_status_i == 1)
	 * 			-v_counter_i++
	 * 			-clear overflow_status_i
	 *		- vcapt = v_counter_i prepended to captured_val_i
	 *
	 */	
	spin_lock_irqsave(&vc_lock, flags);
	
	 /* -From input EPECRi, determine:
	  * 		-event number "i"
	  */
	event_i = (data - (void *)&ls_dbg_vcnt.vcntrepecr)/
								sizeof(ls_dbg_vcnt.vcntrepecr[0]);

	 /* -Read EPGACR_i to determine group to be captured (CTCC), j */
	ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPGACR_OFFSET +
													event_i*4, &reg64);
	regVal = (u32)reg64;
	
	if (unlikely(ret)) {
		goto vcntepecr_set_exit;
	}

	group_j = (regVal & EPGACR_CTCC_MASK) << EPGACR_CTCC_SHIFT;

	/* -Read EPCTRGCR_j to determine which counters are in the group */
	ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPCTRGCR_OFFSET +
			group_j*4, &reg64);

	regVal = (u32)reg64;
	
	if (unlikely(ret)) {
		goto vcntepecr_set_exit;
	}

	i = 0;
	epctrgcr = regVal;
	regVal2 = regVal;
	while (epctrgcr) {
		if (epctrgcr & 0x1) {
			ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPCAPR_OFFSET +
					i*4, &reg64);
			regVal = (u32)reg64;
			
			if (unlikely(ret)) {
				goto vcntepecr_set_exit;
			}

			ls_dbg_vcnt.vcntrcapt[i] = regVal; /* note that this does not have the
												* high order bits */
		}
		++i;
		epctrgcr >>= 1;
	}

	epctrgcr = regVal2; /* reset for examination after capture triggered */

	 /* -Send signal to capture (write EPECRi[STS]) */

	ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPECR_OFFSET +
			event_i*4, &reg64);

	regVal = (u32)reg64;
	
	if (unlikely(ret)) {
		goto vcntepecr_set_exit;
	}

	regVal |= EPECR_STS_MASK;

	/* handle any "last cycle" overflows */
	ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPCTRISR_OFFSET +
							i*4, &reg64);

	epctrisr = (u32)reg64;
	
	if (unlikely(ret)) {
		goto vcntepecr_set_exit;
	}

	if (epctrisr) {
		update_vcounters(epctrisr); 
	}

	ret = ls_dbg_write_reg_internal(EPU, LS_DBG_EPU_EPECR_OFFSET + event_i*4,
									regVal);

	if (unlikely(ret)) {
		goto vcntepecr_set_exit;
	}

	/* handle any late overflows */
	ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPCTRISR_OFFSET, &reg64);

	epctrisr = (u32)reg64;
	
	if (unlikely(ret)) {
		goto vcntepecr_set_exit;
	}

	if (epctrisr) {
		ret = update_vcounters(epctrisr);
		if (unlikely(ret)) {
			goto vcntepecr_set_exit;
		}
	}

	i = 0;
	while (epctrgcr) {
		if (epctrgcr & 0x1) {
			ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPCAPR_OFFSET +
					i*4, &reg64);

			regVal = (u32)reg64;
			
			if (unlikely(ret)) {
				goto vcntepecr_set_exit;
			}

			if (regVal < ls_dbg_vcnt.vcntrcapt[i]) {	
				printk(
				 KERN_INFO
				  "Counter %d overflowed. Incremented overflow (vcntr%d). epctrisr=0x%x\n",
				  	  i, i, epctrisr);
				++ls_dbg_vcnt.vcntr[i];
			}

			ls_dbg_vcnt.vcntrcapt[i] = ls_dbg_vcnt.vcntr[i] << (sizeof(u64)*8/2);
			ls_dbg_vcnt.vcntrcapt[i] |= regVal;
		}

		++i;
		epctrgcr >>= 1;
	}

	spin_unlock_irqrestore(&vc_lock, flags);
	return 0;

vcntepecr_set_exit:
	printk(KERN_ERR "Failure in vcntepecr_set\n");
	spin_unlock_irqrestore(&vc_lock, flags);
	return ret;
}

/* Write only; initiate latch on specified group */
static int vcntepecr_get(void *data, u64 *val)
{
	u32 counter;
	u32 epecr_val;
	u64 reg64;
	int ret = 0;
	unsigned long flags;

	spin_lock_irqsave(&vc_lock, flags);

	counter = (data - (void *)&ls_dbg_vcnt.vcntrepecr[0])/
								sizeof(ls_dbg_vcnt.vcntrepecr[0]);
	ret = ls_dbg_read_reg_internal(EPU, LS_DBG_EPU_EPECR_OFFSET +
													counter*4, &reg64);
	epecr_val = (u32)reg64;

	if (unlikely(ret)) {
		printk(KERN_ERR "Failure in vcntepecr_get\n");
		return ret;
	}

	*(u32 *)data = epecr_val;

	*val = (u32)epecr_val;

	spin_unlock_irqrestore(&vc_lock, flags);

	return ret;
}

u32 vcounter_reg_read(void *data, u64 *val)
{
	u32 ret = 0;
	
	/* note that spinlocks should not be needed here since we are 
	 * referring to addresses of global data, and not the data itself.
	 * For example &ls_dbg_vcnt.vcntrepecr
	 */
	if (data >= (void *)&ls_dbg_vcnt.vcntctrl &&
									data < (void *)&ls_dbg_vcnt.vcntrepecr) {
		*val = *(u32 *)data;
	}
	else if (data >= (void *)&ls_dbg_vcnt.vcntrepecr &&
										data < (void *)&ls_dbg_vcnt.vcntr) {
		ret = vcntepecr_get(data, val);
	}
	else if (data >= (void *)&ls_dbg_vcnt.vcntr && 
									data < (void *)&ls_dbg_vcnt.vcntrcapt) {
		ret = vcntr_get(data, val);
	}
	else if (data >= (void *)&ls_dbg_vcnt.vcntrcapt &&
						data < (void *)(ls_dbg_vcnt.vcntrcapt +
					EPU_V2_NO_OF_COUNTERS*sizeof(ls_dbg_vcnt.vcntrcapt[0]))) {
		*val = *(u64 *)data;
	}
	else {
		ret = -ENOMEM;
	}

	return ret;
}

u32 vcounter_reg_write(void *data, u64 val)
{
	u32 ret = 0;
	
	if (data >= (void *)&ls_dbg_vcnt.vcntctrl && 
									data < (void *)&ls_dbg_vcnt.vcntrepecr) {
		*(u32 *)data = val;
	}
	else if (data >= (void *)&ls_dbg_vcnt.vcntrepecr &&
										data < (void *)&ls_dbg_vcnt.vcntr) {
		ret = vcntepecr_set(data,val);
	}
	else if (data >= (void *)&ls_dbg_vcnt.vcntr &&
									data < (void *)&ls_dbg_vcnt.vcntrcapt) {
		ret = vcntr_set(data, val);
	}
	else if (data >= (void *)&ls_dbg_vcnt.vcntrcapt &&
					data < ((void *)ls_dbg_vcnt.vcntrcapt +
					EPU_V2_NO_OF_COUNTERS*sizeof(ls_dbg_vcnt.vcntrcapt[0]))) {
		*(u64 *)data = val;
	}
	else {
		ret = -ENOMEM;
	}
	return ret;
}

static int vcntrctrl_get(void *data, u64 *val)
{
	*val = *(u32 *)data;
	return 0;
}

static int vcntrctrl_set(void *data, u64 val)
{
	u32 orig_val = *(u32 *)data;
	u32 new_val = val;
	int ret = 0;
	
	if (new_val & VCNTR_ENABLE_MASK) {
		if ((orig_val & VCNTR_ENABLE_MASK) == 0 ) {
			ret = ls_dbg_epu_bind_irq(&intr_overflow);

		}
	}
	else {
		if (orig_val & VCNTR_ENABLE_MASK) {
			ret = ls_dbg_epu_unbind_irq();
		}
	}

	*(u32 *)data = val;
	return ret;
}

DEFINE_SIMPLE_ATTRIBUTE(fops_vcntctrl, vcntrctrl_get, vcntrctrl_set,
							"0x%08llx\n");
DEFINE_SIMPLE_ATTRIBUTE(fops_vcntrepecr, vcntepecr_get, vcntepecr_set,
							"0x%08llx\n");
DEFINE_SIMPLE_ATTRIBUTE(fops_vcntrs, vcntr_get, vcntr_set, "0x%016llx\n");


static int ls_dbg_epu_bind_irq(struct intr *in_intr)
{
	int ret;
	struct device_node *epu_node;
	u32 val;
	int i;

#ifdef DEBUG_LS_DBG
	struct irq_desc *desc;
#endif

	epu_node = of_find_compatible_node(NULL, NULL, "fsl,ls1021a-dcsr-epu");

	/* clear all the interrupt bits (w1c) */
	ret = ls_dbg_write_reg_internal(EPU, LS_DBG_EPU_EPCTRISR_OFFSET,
																0xffffffff);

	if (unlikely(ret)) {
		goto vcntrctrl_set_fail;
	}

	in_intr->num = irq_of_parse_and_map(epu_node, 0);

	if (likely(in_intr->num)) {
		set_irq_flags(in_intr->num, IRQF_VALID);

		ret = request_irq(in_intr->num, in_intr->handler,
					IRQF_SHARED, in_intr->name, epu_node);

		if (unlikely(ret)) {
			printk(KERN_ERR
				"request_irq() of irq_num: %d failed with status %d.\n",
				in_intr->num, ret);
#ifdef DEBUG_LS_DBG
			for_each_irq_desc(i, desc) {
				if (!desc)
					continue;
				printk(KERN_INFO "%d: status=%08x, chip=%08x, handle_irq=%08x\n",
			        i, (u32) desc->status_use_accessors,
			        (u32) desc->irq_data.chip, (u32) desc->handle_irq );
			 }
#endif
			goto vcntrctrl_set_fail;
		}

		/* Turn on overflow interrupts now that request_irq() has been called */
		val = EPCCR_AC_INTR_REQ_VAL << EPCCR_AC_SHIFT;
		val |= EPCCR_ENABLE_MASK;

		for (i = 0; i < EPU_V2_NO_OF_COUNTERS; ++i) {
			ret = ls_dbg_write_reg_internal(EPU, LS_DBG_EPU_EPCCR_OFFSET +
														i*4, val);
			if (unlikely(ret)) {
				goto vcntrctrl_set_fail;
			}
		}
	}
	else {
		printk(KERN_ERR "irq_of_parse_and_map() failed to find irq.");
		goto vcntrctrl_set_fail;
	}

	return 0;

vcntrctrl_set_fail:
	printk(KERN_ERR "vcntrctrl_set failed.\n");
	return ret;
}

int ls_dbg_epu_unbind_irq(void)
{
	int ret = 0;
	int i;

	struct device_node *epu_node;
	epu_node = of_find_compatible_node(NULL, NULL, "fsl,ls1021a-dcsr-epu");

	/* Turn off overflow interrupts before calling free_irq()*/
	for (i = 0; i < EPU_V2_NO_OF_COUNTERS; ++i) {
		ret = ls_dbg_write_reg_internal(EPU, LS_DBG_EPU_EPCCR_OFFSET +
													i*4, 0);
		if (unlikely(ret)) {
			printk(KERN_ERR "Failure in ls_dbg_epu_unbind_irq()\n");
			break;
		}
	}

	/* free the irq number */
	if (intr_overflow.num) {
		/* clean out the pending work queue */
		flush_scheduled_work();
		free_irq(intr_overflow.num, epu_node);
		intr_overflow.num = 0;
	}

	return ret;
}

int vcounters_init(struct dentry *parent_dentry)
{
	int i;
	struct dentry *current_dentry;
	struct dentry *de;
	char reg_name[DBFS_REG_NAME_LEN];
	
	current_dentry = debugfs_create_dir(DEBUGFS_VCOUNTERS_NAME, parent_dentry); 
	if (IS_ERR_OR_NULL(current_dentry)) { 
		printk(KERN_ERR "Creating name @ \"%s\" failed\n",
				DEBUGFS_VCOUNTERS_NAME);
		return -ENOMEM;
	}

	/* note that spinlocks should not be needed here since we are 
	 * referring to addresses of global data, and not the data itself.
	 * For example &ls_dbg_vcnt.vcntrepecr
	 */

	for (i = 0; i < EPU_V2_NO_OF_COUNTERS; ++i) {
		sprintf(reg_name, "%s%d", "vcntr", i);
		debugfs_create_file(reg_name, DBGFS_RW_MODE, current_dentry,
									&ls_dbg_vcnt.vcntr[i], &fops_vcntrs);

		sprintf(reg_name, "%s%d", "vcntrcapt", i);
		DBGFS_CREATE_RW_X64(reg_name, current_dentry, &ls_dbg_vcnt.vcntrcapt[i]);
	}

	for (i = 0; i < EPU_V2_NO_OF_SCU_EVENTS; ++i) {
		sprintf(reg_name, "%s%d", "vcntrepecr", i);
		debugfs_create_file(reg_name, DBGFS_RW_MODE, current_dentry,
								&ls_dbg_vcnt.vcntrepecr[i], &fops_vcntrepecr);

	}

	debugfs_create_file("vcntrctrl", DBGFS_RW_MODE, current_dentry,
									&ls_dbg_vcnt.vcntctrl, &fops_vcntctrl);

	reg_add_map(VCOUNT, (unsigned long)&ls_dbg_vcnt, sizeof(struct vcounters));

	return 0;
}
