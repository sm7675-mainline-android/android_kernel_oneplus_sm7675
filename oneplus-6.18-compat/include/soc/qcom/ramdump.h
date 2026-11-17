/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _VENDOR_COMPAT_QCOM_RAMDUMP_H
#define _VENDOR_COMPAT_QCOM_RAMDUMP_H

#include <linux/errno.h>
#include <linux/types.h>

struct device;

struct ramdump_segment {
	unsigned long address;
	void *v_address;
	size_t size;
};

static inline void *create_ramdump_device(const char *name,
			struct device *parent)
{
	return NULL;
}

static inline void destroy_ramdump_device(void *dev)
{
}

static inline int do_ramdump(void *dev, struct ramdump_segment *segments,
			unsigned int count)
{
	return -ENODEV;
}

#endif
