/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef ANDROID_LEGACY_NAPI_COMPAT_H
#define ANDROID_LEGACY_NAPI_COMPAT_H

#include <linux/netdevice.h>

static inline bool vendor_napi_reschedule(struct napi_struct *napi)
{
	if (!napi_schedule_prep(napi))
		return false;

	__napi_schedule(napi);
	return true;
}

#define napi_reschedule vendor_napi_reschedule

#endif
