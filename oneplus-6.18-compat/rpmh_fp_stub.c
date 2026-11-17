// SPDX-License-Identifier: GPL-2.0-only
/*
 * 6.18 does not provide the vendor RPMH fast-path TCS interface. Return
 * -EOPNOTSUPP so dependent drivers can fall back without fast-path support.
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>

struct device;
struct tcs_cmd;

int rpmh_init_fast_path(const struct device *dev, struct tcs_cmd *cmd, int n);
int rpmh_update_fast_path(const struct device *dev, struct tcs_cmd *cmd,
			  int n, u32 update_mask);

int rpmh_init_fast_path(const struct device *dev, struct tcs_cmd *cmd, int n)
{
	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(rpmh_init_fast_path);

int rpmh_update_fast_path(const struct device *dev, struct tcs_cmd *cmd,
			  int n, u32 update_mask)
{
	return -EOPNOTSUPP;
}
EXPORT_SYMBOL(rpmh_update_fast_path);

MODULE_DESCRIPTION("RPMH fast-path compatibility stubs for 6.18");
MODULE_LICENSE("GPL");
