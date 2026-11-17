/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LEGACY_TIMER_COMPAT_H
#define _LEGACY_TIMER_COMPAT_H

#include <linux/timer.h>
#include <linux/netdevice.h>

#define android_kabi_reserved2 __kabi_reserved2

#if !defined(CONFIG_DEBUG_OBJECTS_TIMERS)
#define destroy_timer_on_stack(timer) do { } while (0)
#endif

#endif
