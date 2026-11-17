/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef ANDROID_BACKPORT_RESERVED
#include <linux/types.h>

#include <linux/rpmsg/qcom_glink.h>

struct device;
struct device_node;
struct qcom_glink;
struct qcom_glink_smem;

struct qcom_glink *qcom_glink_spss_register(struct device *parent,
						    struct device_node *node);
void qcom_glink_spss_unregister(struct qcom_glink *glink);

#ifdef CONFIG_ANDROID_VENDOR_OEM_DATA
#define ANDROID_BACKPORT_RESERVED(n) u64 android_backport_reserved##n
#else
#define ANDROID_BACKPORT_RESERVED(n)
#endif
#endif
