#ifndef _CAMERA_LEGACY_GPIO_H
#define _CAMERA_LEGACY_GPIO_H

#include <linux/gpio.h>
#include <linux/of.h>

struct gpio {
	unsigned int gpio;
	unsigned long flags;
	const char *label;
};

#define of_gpio_count(node) of_count_phandle_with_args(node, "gpios", "#gpio-cells")

static inline void gpio_free_array(const struct gpio *gpios, size_t count)
{
	size_t i;

	for (i = 0; i < count; i++)
		gpio_free(gpios[i].gpio);
}

#endif
