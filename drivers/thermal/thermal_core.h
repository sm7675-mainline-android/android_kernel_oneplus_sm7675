/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Use the target kernel thermal layout for the QTI driver modules.
 */
#include <drivers/thermal/thermal_core.h>

#ifndef __QTI_THERMAL_CORE_COMPAT_H__
#define __QTI_THERMAL_CORE_COMPAT_H__

#ifndef THERMAL_NO_TARGET
#define THERMAL_NO_TARGET -1UL
#endif

#define to_thermal_zone(_dev) \
	container_of(_dev, struct thermal_zone_device, device)

#define to_cooling_device(_dev)	\
	container_of(_dev, struct thermal_cooling_device, device)

#endif /* __QTI_THERMAL_CORE_COMPAT_H__ */
