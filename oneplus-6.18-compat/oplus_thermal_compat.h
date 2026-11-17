/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef OPLUS_THERMAL_COMPAT_H
#define OPLUS_THERMAL_COMPAT_H

#include <linux/thermal.h>

#define thermal_zone_device_register(type, trips, mask, devdata, ops, tzp, \
				     passive_delay, polling_delay) \
	thermal_tripless_zone_device_register(type, devdata, ops, tzp)

#endif
