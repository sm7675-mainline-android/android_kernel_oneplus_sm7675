/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef VENDOR_LEGACY_INCLUDES_H
#define VENDOR_LEGACY_INCLUDES_H

#include <linux/device.h>
#include <linux/container_of.h>
#include <linux/cpu.h>
#include <linux/dma-mapping.h>
#include <linux/dma-map-ops.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/hrtimer.h>
#include <linux/idr.h>
#include <linux/irqdomain.h>
#include <linux/kstrtox.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>
#include <linux/pm_wakeup.h>
#include <linux/pm_qos.h>
#include <linux/pwm.h>
#include <linux/serial_core.h>
#include <linux/spi/spi.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/u64_stats_sync.h>
#include <linux/vmalloc.h>
#include <linux/pinctrl/pinconf-generic.h>
#include <sound/soc-component.h>

/* Qualcomm audio modules still use the pre-6.18 SlimBus message helpers. */
#include "include/linux/slimbus/slimbus.h"

/* 6.18 removed the pfn_t typedef while vendor DRM still uses pfn_t.h. */
#ifndef VENDOR_PFN_T_DEFINED
#define VENDOR_PFN_T_DEFINED
typedef struct {
	unsigned long val;
} pfn_t;
#endif

#ifndef DRM_UNLOCKED
#define DRM_UNLOCKED 0
#endif

/* The target platform has an IOMMU-backed display path; 6.18 removed the
 * bus-wide iommu_present() query used by the vendor driver. */
#ifndef iommu_present
static inline bool vendor_iommu_present(const struct bus_type *bus)
{
	return true;
}
#define iommu_present(bus) vendor_iommu_present(bus)
#endif

#ifdef CONFIG_LEGACY_PMIC_GLINK_API
#define PMIC_GLINK_COMPAT_VENDOR_API
#include "pmic_glink_legacy.h"
#endif

#ifndef GPIOF_DIR_IN
#define GPIOF_DIR_IN 1
#endif
#ifndef GPIOF_DIR_OUT
#define GPIOF_DIR_OUT 0
#endif

#ifndef no_llseek
#define no_llseek noop_llseek
#endif

#ifndef hrtimer_init
#define hrtimer_init(timer, clock_id, mode) \
	hrtimer_setup(timer, hrtimer_dummy_timeout, clock_id, mode)
#endif

#ifndef from_timer
#define from_timer(var, callback_timer, timer_fieldname) \
	container_of(callback_timer, typeof(*var), timer_fieldname)
#endif

#ifndef del_timer
#define del_timer(timer) timer_delete(timer)
#endif
#ifndef del_timer_sync
#define del_timer_sync(timer) timer_delete_sync(timer)
#endif

#ifndef ida_simple_get
#define ida_simple_get(ida, start, end, gfp) ida_alloc_range(ida, start, (end) - 1, gfp)
#endif
#ifndef ida_simple_remove
#define ida_simple_remove(ida, id) ida_free(ida, id)
#endif

#define strtobool kstrtobool
#define of_get_gpio(node, index) of_get_named_gpio(node, NULL, index)
#ifndef devm_gpio_request
#define devm_gpio_request(dev, gpio, label) \
	devm_gpio_request_one(dev, gpio, 0, label)
#endif
#define spi_master_get_devdata(controller) spi_controller_get_devdata(controller)
#define SND_SOC_DAIFMT_CBS_CFS SND_SOC_DAIFMT_CBC_CFC
#define pwm_apply_state pwm_apply_might_sleep

#ifndef snd_soc_component_read32
#define snd_soc_component_read32(component, reg) \
	snd_soc_component_read((component), (reg))
#endif

#ifndef VENDOR_LEGACY_MSM_CPUIDLE_DEFINED
#define VENDOR_LEGACY_MSM_CPUIDLE_DEFINED
static inline int msm_cpuidle_get_deep_idle_latency(void)
{
	return 0;
}
#endif

/* The legacy PM QoS API is now the CPU latency QoS API. */
#ifndef PM_QOS_CPU_DMA_LATENCY
#define PM_QOS_CPU_DMA_LATENCY 0
#endif
#ifndef pm_qos_add_request
#define pm_qos_add_request(req, class, value) \
	cpu_latency_qos_add_request((req), (value))
#endif
#ifndef pm_qos_update_request
#define pm_qos_update_request(req, value) \
	cpu_latency_qos_update_request((req), (value))
#endif
#ifndef pm_qos_remove_request
#define pm_qos_remove_request(req) cpu_latency_qos_remove_request((req))
#endif

/* irq_linear_revmap() was removed after becoming an alias of this helper. */
#ifndef irq_linear_revmap
#define irq_linear_revmap(domain, hwirq) irq_find_mapping((domain), (hwirq))
#endif
#ifndef SLAB_MEM_SPREAD
#define SLAB_MEM_SPREAD 0
#endif

#ifndef nth_page
#define nth_page(page, n) ((page) + (n))
#endif

#ifndef SPI_MASTER_HALF_DUPLEX
#define SPI_MASTER_HALF_DUPLEX SPI_CONTROLLER_HALF_DUPLEX
#endif

static inline size_t vendor_compat_strlcpy(char *destination,
					   const char *source, size_t size)
{
	size_t source_length = strlen(source);

	if (size)
		strscpy(destination, source, size);
	return source_length;
}

#define strlcpy vendor_compat_strlcpy

#define VENDOR_CLASS_CREATE_SELECT(_1, _2, NAME, ...) NAME
#define VENDOR_CLASS_CREATE_ONE(name) class_create(name)
#define VENDOR_CLASS_CREATE_TWO(owner, name) class_create(name)
#define class_create(...) \
	VENDOR_CLASS_CREATE_SELECT(__VA_ARGS__, VENDOR_CLASS_CREATE_TWO, \
				   VENDOR_CLASS_CREATE_ONE)(__VA_ARGS__)

#ifndef u64_stats_fetch_begin_irq
#define u64_stats_fetch_begin_irq u64_stats_fetch_begin
#endif

#ifdef __assign_str
#undef __assign_str
#define VENDOR_ASSIGN_STR_SELECT(_1, _2, NAME, ...) NAME
#define VENDOR_ASSIGN_STR_ONE(name) __assign_str(name)
#define VENDOR_ASSIGN_STR_TWO(name, source) __assign_str(name)
#define __assign_str(...) \
	VENDOR_ASSIGN_STR_SELECT(__VA_ARGS__, VENDOR_ASSIGN_STR_TWO, \
				 VENDOR_ASSIGN_STR_ONE)(__VA_ARGS__)
#endif

#endif
