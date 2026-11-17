// SPDX-License-Identifier: GPL-2.0-only
#include <linux/device.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/soc/qcom/pmic_glink.h>
#include <linux/workqueue.h>

#define PMIC_GLINK_COMPAT_IMPLEMENTATION
#include "pmic_glink_legacy.h"

struct legacy_pmic_glink_client {
	struct pmic_glink_client *upstream;
	struct pmic_glink_client_data data;
	struct workqueue_struct *rx_wq;
};

struct legacy_pmic_glink_message {
	struct work_struct work;
	struct legacy_pmic_glink_client *client;
	size_t length;
	u8 data[];
};

static void pmic_glink_compat_rx_work(struct work_struct *work)
{
	struct legacy_pmic_glink_message *message =
		container_of(work, struct legacy_pmic_glink_message, work);
	struct legacy_pmic_glink_client *client = message->client;

	client->data.msg_cb(client->data.priv, message->data, message->length);
	kfree(message);
}

static void pmic_glink_compat_message(const void *message, size_t length,
					      void *private_data)
{
	struct legacy_pmic_glink_client *client = private_data;
	struct legacy_pmic_glink_message *pending;

	if (!client->data.msg_cb)
		return;

	pending = kmalloc(struct_size(pending, data, length), GFP_ATOMIC);
	if (!pending) {
		pr_err_ratelimited("pmic_glink_compat: cannot queue message for %s\n",
				   client->data.name);
		return;
	}
	pending->client = client;
	pending->length = length;
	memcpy(pending->data, message, length);
	INIT_WORK(&pending->work, pmic_glink_compat_rx_work);
	queue_work(client->rx_wq, &pending->work);
}

static void pmic_glink_compat_state(void *private_data, int state)
{
	struct legacy_pmic_glink_client *client = private_data;

	if (client->data.state_cb)
		client->data.state_cb(client->data.priv,
					 state ? PMIC_GLINK_STATE_UP : PMIC_GLINK_STATE_DOWN);
}

static void pmic_glink_compat_release(void *data)
{
	struct legacy_pmic_glink_client *client = data;

	if (client->upstream) {
		pmic_glink_client_unregister(client->upstream);
		client->upstream = NULL;
	}
	if (client->rx_wq) {
		destroy_workqueue(client->rx_wq);
		client->rx_wq = NULL;
	}
}

struct pmic_glink_client *pmic_glink_register_client(
	struct device *dev, const struct pmic_glink_client_data *client_data)
{
	struct legacy_pmic_glink_client *client;
	int ret;

	client = devm_kzalloc(dev, sizeof(*client), GFP_KERNEL);
	if (!client)
		return ERR_PTR(-ENOMEM);

	client->data = *client_data;
	client->rx_wq = alloc_ordered_workqueue("pmic-%s", WQ_MEM_RECLAIM,
					      client_data->name);
	if (!client->rx_wq)
		return ERR_PTR(-ENOMEM);
	client->upstream = devm_pmic_glink_client_alloc(
		dev, client_data->id, pmic_glink_compat_message,
		pmic_glink_compat_state, client);
	if (IS_ERR(client->upstream)) {
		ret = PTR_ERR(client->upstream);
		client->upstream = NULL;
		pmic_glink_compat_release(client);
		return ERR_PTR(ret);
	}
	ret = devm_add_action_or_reset(dev, pmic_glink_compat_release, client);
	if (ret)
		return ERR_PTR(ret);

	pmic_glink_client_register(client->upstream);
	return (struct pmic_glink_client *)client;
}
EXPORT_SYMBOL_GPL(pmic_glink_register_client);

int pmic_glink_unregister_client(struct pmic_glink_client *opaque_client)
{
	struct legacy_pmic_glink_client *client =
		(struct legacy_pmic_glink_client *)opaque_client;

	if (!client || !client->upstream)
		return -EINVAL;

	pmic_glink_compat_release(client);
	return 0;
}
EXPORT_SYMBOL_GPL(pmic_glink_unregister_client);

int pmic_glink_write(struct pmic_glink_client *opaque_client, void *data,
			    size_t length)
{
	struct legacy_pmic_glink_client *client =
		(struct legacy_pmic_glink_client *)opaque_client;

	if (!client || !client->upstream)
		return -EINVAL;
	return pmic_glink_send(client->upstream, data, length);
}
EXPORT_SYMBOL_GPL(pmic_glink_write);

MODULE_DESCRIPTION("Legacy Qualcomm PMIC Glink API compatibility");
MODULE_LICENSE("GPL");
