#include <linux/device/bus.h>
#include <linux/hrtimer.h>
#include <linux/kstrtox.h>

#define strtobool kstrtobool
#define kport_bus_dev_root bus_get_dev_root
#define hrtimer_init(timer, clock_id, mode) \
	hrtimer_setup(timer, hrtimer_dummy_timeout, clock_id, mode)
