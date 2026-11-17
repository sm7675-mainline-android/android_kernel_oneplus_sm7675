/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef MHI_CNSS2_COMPAT_H
#define MHI_CNSS2_COMPAT_H

#include <linux/mhi.h>

static inline void mhi_device_get(struct mhi_device *mhi_dev)
{
	mhi_device_get_sync(mhi_dev);
}

#endif
