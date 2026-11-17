/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2023-2024, Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef __QTI_THERMAL_ZONE_INTERNAL_H
#define __QTI_THERMAL_ZONE_INTERNAL_H

#include <linux/thermal.h>
#include <trace/hooks/thermal.h>
#include "../thermal_core.h"


/*
 * 6.18 dropped thermal_zone_device::thermal_instances; cooling instances now
 * live on a per-trip list (thermal_trip_desc::thermal_instances) and
 * thermal_instance carries a trip pointer instead of a trip index.
 * These helpers keep the QTI code readable across both layouts.
 */
static inline struct list_head *qti_tz_trip_instances_head(struct thermal_zone_device *tz,
							   int trip)
{
	if (!tz || trip < 0 || trip >= tz->num_trips)
		return NULL;
	return &tz->trips[trip].thermal_instances;
}

static inline int qti_tz_instance_trip_id(struct thermal_zone_device *tz,
					  const struct thermal_instance *instance)
{
	int i;

	if (!tz || !instance || !instance->trip)
		return -1;

	for (i = 0; i < tz->num_trips; i++) {
		if (&tz->trips[i].trip == instance->trip)
			return i;
	}
	return -1;
}

/* Iterate every cooling instance bound to any trip of this zone. */
#define qti_tz_for_each_instance(tz, instance)				\
	for (int __qti_i = 0; __qti_i < (tz)->num_trips; __qti_i++)	\
		list_for_each_entry(instance,				\
				    &(tz)->trips[__qti_i].thermal_instances, \
				    trip_node)

/* head used by qti_tz_change_mode(): the first trip's list is not enough, so
 * use a macro-based walk there instead. */
static inline struct list_head *qti_tz_instances_head(struct thermal_zone_device *tz)
{
	return qti_tz_trip_instances_head(tz, 0);
}

/* Generic helpers for thermal zone -> change_mode ops */
static inline __maybe_unused int qti_tz_change_mode(struct thermal_zone_device *tz,
		enum thermal_device_mode mode)
{
	struct thermal_instance *instance;

	if (!tz)
		return 0;

	tz->passive = 0;
	tz->temperature = THERMAL_TEMP_INVALID;
	tz->prev_low_trip = -INT_MAX;
	tz->prev_high_trip = INT_MAX;
	for (int i = 0; i < tz->num_trips; i++) {
		list_for_each_entry(instance, &tz->trips[i].thermal_instances, trip_node) {
			instance->initialized = false;
			if (mode == THERMAL_DEVICE_DISABLED) {
				instance->target = THERMAL_NO_TARGET;
				instance->cdev->updated = false;
				thermal_cdev_update(instance->cdev);
			}
		}
	}

	return 0;
}

static inline __maybe_unused int qti_update_tz_ops(struct thermal_zone_device *tz, bool enable)
{
	if (!tz)
		return -EINVAL;

	mutex_lock(&tz->lock);
	if (enable) {
		if (!tz->ops.change_mode)
			tz->ops.change_mode = qti_tz_change_mode;
	} else {
		tz->ops.change_mode = NULL;
	}
	mutex_unlock(&tz->lock);
	return 0;
}

static __maybe_unused void disable_cdev_stats(void *unused,
		struct thermal_cooling_device *cdev, bool *disable)
{
	*disable = true;
}

/* Generic thermal vendor hooks initialization API.
 * 6.18 no longer carries android_vh_disable_thermal_cooling_stats, so the
 * cooling-device statistics stay enabled; the callback itself is unused. */
static inline __maybe_unused void thermal_vendor_hooks_init(void)
{
}

static inline __maybe_unused void thermal_vendor_hooks_exit(void)
{
}

/* Convert the trip pointer supplied by get_trend() to a zone index. */
static inline int qti_tz_trip_id(struct thermal_zone_device *tz,
				 const struct thermal_trip *trip)
{
	int i;

	if (!tz || !trip)
		return -1;

	for (i = 0; i < tz->num_trips; i++) {
		if (&tz->trips[i].trip == trip)
			return i;
	}
	return -1;
}

/*Generic helpers for thermal zone -> get_trend ops */
static __maybe_unused inline int qti_tz_get_trend(
				struct thermal_zone_device *tz,
				const struct thermal_trip *trip,
				enum thermal_trend *trend)
{
	int trip_temp = 0, trip_hyst = 0, temp, trip_id;
	struct thermal_instance *instance;
	bool monitor_trip_only = false;

	if (!tz || !trip || !trend)
		return -EINVAL;

	/* 6.18 stores trip temperature/hysteresis in the zone's trip array
	 * instead of exposing get_trip_temp()/get_trip_hyst() callbacks. */
	trip_id = qti_tz_trip_id(tz, trip);
	if (trip_id < 0)
		return -EINVAL;

	trip_temp = trip->temperature;
	trip_hyst = trip->hysteresis;
	if (!trip_hyst)
		return -EINVAL;

	temp = READ_ONCE(tz->temperature);

	/*
	 * Handle only monitor trip clear condition, fallback to default
	 * trend estimation for all other cases.
	 * If all the instances of a given trip are monitor type(upper == lower),
	 * then only treat this trip as monitor trip and consider hysterisis for
	 * clear condition
	 */
	list_for_each_entry(instance, qti_tz_trip_instances_head(tz, trip_id), trip_node) {
		if (qti_tz_instance_trip_id(tz, instance) != trip_id)
			continue;

		if (instance->lower != instance->upper)
			return -EINVAL;

		monitor_trip_only = true;
	}

	if (monitor_trip_only && temp < trip_temp &&
			(temp > (trip_temp - trip_hyst))) {
		*trend = THERMAL_TREND_STABLE;
		return 0;
	}

	return -EINVAL;
}

#endif  // __QTI_THERMAL_ZONE_INTERNAL_H
