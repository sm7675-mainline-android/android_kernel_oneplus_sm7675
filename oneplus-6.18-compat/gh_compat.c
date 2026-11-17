// SPDX-License-Identifier: GPL-2.0-only
/*
 * gh_compat - expose the 6.18 upstream gunyah resource-manager API under the
 * vendor symbol names used by Qualcomm modules.
 */
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/gunyah.h>

struct gunyah_rm;

int gunyah_rm_call(struct gunyah_rm *rsc_mgr, u32 message_id,
		   const void *req_buf, size_t req_buf_size, void **resp_buf,
		   size_t *resp_buf_size);
int gunyah_rm_notifier_register(struct gunyah_rm *rm,
				struct notifier_block *nb);
int gunyah_rm_notifier_unregister(struct gunyah_rm *rm,
				  struct notifier_block *nb);

int gh_rm_call(void *rsc_mgr, u32 message_id, const void *req_buf,
	       size_t req_buf_size, void **resp_buf, size_t *resp_buf_size)
{
	return gunyah_rm_call(rsc_mgr, message_id, req_buf, req_buf_size,
			      resp_buf, resp_buf_size);
}
EXPORT_SYMBOL_GPL(gh_rm_call);

int gh_rm_notifier_register(void *rm, struct notifier_block *nb)
{
	return gunyah_rm_notifier_register(rm, nb);
}
EXPORT_SYMBOL_GPL(gh_rm_notifier_register);

int gh_rm_notifier_unregister(void *rm, struct notifier_block *nb)
{
	return gunyah_rm_notifier_unregister(rm, nb);
}
EXPORT_SYMBOL_GPL(gh_rm_notifier_unregister);

int gh_rm_register_platform_ops(const struct gunyah_rm_platform_ops *ops)
{
	return gunyah_rm_register_platform_ops(ops);
}
EXPORT_SYMBOL_GPL(gh_rm_register_platform_ops);

void gh_rm_unregister_platform_ops(const struct gunyah_rm_platform_ops *ops)
{
	gunyah_rm_unregister_platform_ops(ops);
}
EXPORT_SYMBOL_GPL(gh_rm_unregister_platform_ops);

MODULE_DESCRIPTION("Qualcomm Gunyah symbol compatibility shim");
MODULE_LICENSE("GPL");
