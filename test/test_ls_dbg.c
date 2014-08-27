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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "reg_access_ioctl.h"

#define DEFINE_TEST(name) int name(void)

#define MODULE_NAME "ls-dbg"
#define DEVPATH "/dev/ls-dbg"

#define FAIL_IF(cond)							\
do {									\
	if (cond) {							\
		fprintf(stderr, "Assertion failed at %s:%d\n", __FILE__, __LINE__); \
		return 0;						\
	}								\
} while (0)

#define FAIL_UNLESS(cond)						\
do {									\
	if (!(cond)) {							\
		fprintf(stderr, "Assertion failed at %s:%d\n", __FILE__, __LINE__); \
		return 0;						\
	}								\
} while (0)


typedef int (*test_func)(void);

typedef struct test {
	test_func func;
	char *name;
} test_t;

int dev_fd;

#define CMD_SIZE 128

static int insert_module(void)
{
	int ret;
	char cmd[CMD_SIZE];

	snprintf(cmd, CMD_SIZE, "insmod %s.ko", MODULE_NAME);

	ret = system(cmd);

	if (WIFEXITED(ret) && WEXITSTATUS(ret) == 0)
		return 1;
	else
		return 0;

}

static int remove_module(void)
{
	int ret;
	char cmd[CMD_SIZE];

	snprintf(cmd, CMD_SIZE, "rmmod %s", MODULE_NAME);

	ret = system(cmd);

	if (WIFEXITED(ret) && WEXITSTATUS(ret) == 0)
		return 1;
	else
		return 0;

}

static inline void init_reg_access_struct(struct reg_access *ra,
					  enum TRACEIP_MODULE id, __u32 offset,
					  void *buffer, __u32 reg_size)
{
	ra->module_id = id;
	ra->offset = offset;
	ra->buffer = (intptr_t)buffer;
	ra->reg_size = reg_size;
}

static int read_reg(int fd, enum TRACEIP_MODULE id, __u32 offset,
		    void *buffer, __u32 reg_size)
{
	struct reg_access ra;
	int ret;

	init_reg_access_struct(&ra, id, offset, buffer, reg_size);

	ret = ioctl(fd, DBG_READ_REG, &ra);

	return ret >= 0;
}

static int write_reg(int fd, enum TRACEIP_MODULE id, __u32 offset,
		     void *buffer, __u32 reg_size)
{
	struct reg_access ra;
	int ret;

	init_reg_access_struct(&ra, id, offset, buffer, reg_size);

	ret = ioctl(fd, DBG_WRITE_REG, &ra);

	return ret >= 0;
}

static int test_setup()
{
	FAIL_IF(insert_module() == 0);

	dev_fd = open(DEVPATH, O_RDWR);
	FAIL_IF(dev_fd < 0);

	return 1;
}

static int test_cleanup()
{
	int ret;

	ret = close(dev_fd);
	FAIL_IF(ret < 0);

	FAIL_IF(remove_module() == 0);

	return 1;
}

DEFINE_TEST(test_open_close)
{
	/*
	 * nothing to test here, the entire work for this test is done in
	 * test_setup/test_cleanup
	 */
	return 1;
}

DEFINE_TEST(test_invalid_id)
{
	int ret;
	__u32 val;

	/* all of these should fail with EINVAL */
	ret = read_reg(dev_fd, NUM_IDS, 0, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EINVAL);

	ret = read_reg(dev_fd, -5, 0, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EINVAL);

	ret = read_reg(dev_fd, 1000, 0, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EINVAL);

	return 1;
}

DEFINE_TEST(test_invalid_offset)
{
	int ret;
	__u32 val;

	/* all of these should fail with EFAULT */
	ret = read_reg(dev_fd, ETM0, 0x100000, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	ret = read_reg(dev_fd, ETM1, 0x100000, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	ret = read_reg(dev_fd, ETR, 0x100000, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	ret = read_reg(dev_fd, ETF0, 0x100000, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	ret = read_reg(dev_fd, ETF1, 0x100000, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	ret = read_reg(dev_fd, CSTF0, 0x100000, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	ret = read_reg(dev_fd, CSTF1, 0x100000, &val, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	return 1;
}

DEFINE_TEST(test_invalid_buffer)
{
	int ret;

	/* all of these should fail with EFAULT */
	ret = read_reg(dev_fd, ETM0, 0, NULL, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	ret = read_reg(dev_fd, ETM0, 0, (void *)0xaabbccdd, 32);
	FAIL_UNLESS(ret == 0 && errno == EFAULT);

	return 1;
}

DEFINE_TEST(test_invalid_regsize)
{
	int ret;
	__u32 val;

	/* all of these should fail with EINVAL */
	ret = read_reg(dev_fd, ETM0, 0, &val, 8);
	FAIL_UNLESS(ret == 0 && errno == EINVAL);

	ret = read_reg(dev_fd, ETM0, 0, &val, 16);
	FAIL_UNLESS(ret == 0 && errno == EINVAL);

	ret = read_reg(dev_fd, ETM0, 0, &val, 100);
	FAIL_UNLESS(ret == 0 && errno == EINVAL);

	return 1;
}

DEFINE_TEST(test_read_write)
{
	int ret;
	__u32 val = 0;

	/* reading/writing should fail with EINVAL */
	ret = read(dev_fd, &val, sizeof(val));
	FAIL_UNLESS(ret < 0 && errno == EINVAL);

	ret = write(dev_fd, &val, sizeof(val));
	FAIL_UNLESS(ret < 0 && errno == EINVAL);

	return 1;
}

#define ETM_DEVID_OFFSET	0xfcc
#define ETR_DEVID_OFFSET	0xfcc
#define ETF_DEVID_OFFSET	0xfcc
#define CSTF_DEVID_OFFSET	0xfcc

#define ETM_LOCKSTATUS_OFFSET	0xfb4
#define ETM_LOCKACCESS_OFFSET	0xfb0

DEFINE_TEST(test_simple_read)
{
	int ret;
	__u32 val;

	ret = read_reg(dev_fd, ETM0, ETM_DEVID_OFFSET, &val, 32);
	FAIL_IF(ret < 0);

	ret = read_reg(dev_fd, ETR, ETR_DEVID_OFFSET, &val, 32);
	FAIL_IF(ret < 0);

	ret = read_reg(dev_fd, ETF0, ETF_DEVID_OFFSET, &val, 32);
	FAIL_IF(ret < 0);

	ret = read_reg(dev_fd, CSTF0, CSTF_DEVID_OFFSET, &val, 32);
	FAIL_IF(ret < 0);

	return 1;
}

DEFINE_TEST(test_simple_write)
{
	int ret;
	__u32 val;

	val = 0xC5ACCE55;
	ret = write_reg(dev_fd, ETM0, ETM_LOCKACCESS_OFFSET, &val, 32);
	FAIL_IF(ret < 0);

	val = ~0xC5ACCE55;
	ret = write_reg(dev_fd, ETM0, ETM_LOCKACCESS_OFFSET, &val, 32);
	FAIL_IF(ret < 0);

	return 1;
}

static void run_tests(test_t *tests)
{
	int i = 0;
	int ret;
	int num_failures;

	while (tests[i].func) {
		printf("%s: ", tests[i].name);
		fflush(stdout);

		num_failures = 0;

		ret = test_setup();
		if (!ret)
			num_failures++;

		ret = tests[i].func();
		if (!ret)
			num_failures++;

		ret = test_cleanup();
		if (!ret)
			num_failures++;

		if (num_failures == 0)
			printf("PASS\n");
		else
			printf("FAIL\n");

		i++;
		printf("\n");
	}
}

test_t all_tests[] = {
	{
		.name = "Opening and closing the device",
		.func = test_open_close,
	},
	{
		.name = "Read invalid id",
		.func = test_invalid_id,
	},
	{
		.name = "Read invalid offset",
		.func = test_invalid_offset,
	},
	{
		.name = "Read invalid buffer",
		.func = test_invalid_buffer,
	},
	{
		.name = "Read invalid regsize",
		.func = test_invalid_regsize,
	},
	{
		.name = "Reading/Writing from/to the device",
		.func = test_read_write,
	},
	{
		.name = "Simple register read",
		.func = test_simple_read,
	},
	{
		.name = "Simple register write",
		.func = test_simple_write,
	},
	{
		.name = NULL,
		.func = NULL,
	}
};

int main()
{
	run_tests(all_tests);

	return 0;
}
