/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef PMIC_GLINK_LEGACY_COMPAT_H
#define PMIC_GLINK_LEGACY_COMPAT_H

#include <linux/types.h>

struct device;

enum pmic_glink_state {
	PMIC_GLINK_STATE_DOWN,
	PMIC_GLINK_STATE_UP,
};

struct pmic_glink_client_data {
	const char *name;
	u32 id;
	void *priv;
	int (*msg_cb)(void *priv, void *data, size_t len);
	void (*state_cb)(void *priv, enum pmic_glink_state state);
};

struct pmic_glink_client *pmic_glink_register_client(
	struct device *dev, const struct pmic_glink_client_data *client_data);
int pmic_glink_unregister_client(struct pmic_glink_client *client);
int pmic_glink_write(struct pmic_glink_client *client, void *data, size_t len);

#endif
