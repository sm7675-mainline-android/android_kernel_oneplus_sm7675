/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __VENDOR_QCOM_PM_H__
#define __VENDOR_QCOM_PM_H__

#include <linux/types.h>

#ifndef VENDOR_LEGACY_MSM_CPUIDLE_DEFINED
static inline s32 msm_cpuidle_get_deep_idle_latency(void)
{
	return 0;
}
#endif

#endif
