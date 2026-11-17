/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _VENDOR_COMPAT_LINUX_SLIMBUS_SLIMBUS_H
#define _VENDOR_COMPAT_LINUX_SLIMBUS_SLIMBUS_H
#include <linux/slimbus.h>

/* Compatibility for Qualcomm audio drivers written for the pre-6.18 API. */
#ifndef SLIM_MSG_MT_CORE
#define SLIM_MSG_MT_CORE 0x0
#endif
#ifndef SLIM_MSG_MT_DEST_REFERRED_USER
#define SLIM_MSG_MT_DEST_REFERRED_USER 0x2
#endif

struct slim_ele_access {
	u16 start_offset;
	u8 num_bytes;
	struct completion *comp;
};

static inline int slim_request_val_element(struct slim_device *sdev,
		struct slim_ele_access *old_msg, void *dest, int bytes)
{
	struct slim_val_inf msg = {
		.start_offset = old_msg->start_offset,
		.num_bytes = bytes,
		.rbuf = dest,
		.comp = old_msg->comp,
	};

	return slim_xfer_msg(sdev, &msg, SLIM_MSG_MC_REQUEST_VALUE);
}

static inline int slim_change_val_element(struct slim_device *sdev,
		struct slim_ele_access *old_msg, const void *src, int bytes)
{
	struct slim_val_inf msg = {
		.start_offset = old_msg->start_offset,
		.num_bytes = bytes,
		.wbuf = src,
		.comp = old_msg->comp,
	};

	return slim_xfer_msg(sdev, &msg, SLIM_MSG_MC_CHANGE_VALUE);
}

static inline int slim_user_msg(struct slim_device *sdev, u8 laddr,
		u8 mt, u8 mc, struct slim_ele_access *old_msg,
		const void *src, int bytes)
{
	int ret = 0;
	int i;

	if (mt != SLIM_MSG_MT_DEST_REFERRED_USER || mc != 0x0)
		return -EOPNOTSUPP;

	/* The old repeat-value packet writes every byte to the same element. */
	for (i = 0; i < bytes; i++) {
		ret = slim_writeb(sdev, old_msg->start_offset,
				((const u8 *)src)[i]);
		if (ret)
			break;
	}

	return ret;
}

static inline int slim_bulk_msg_write(struct slim_device *sdev,
		u8 mt, u8 mc, struct slim_val_inf *msgs, int count,
		void *unused_rbuf, void *unused_comp)
{
	int ret = 0;
	int i;

	if (mt != SLIM_MSG_MT_CORE || mc != SLIM_MSG_MC_CHANGE_VALUE)
		return -EOPNOTSUPP;

	for (i = 0; i < count; i++) {
		ret = slim_xfer_msg(sdev, &msgs[i], mc);
		if (ret)
			break;
	}

	return ret;
}

static inline int slim_reservemsg_bw(struct slim_device *sdev,
		u32 bw_ops, bool commit)
{
	return -EOPNOTSUPP;
}

/* Legacy Qualcomm codec channel API removed when SLIMbus streams were
 * consolidated into slim_stream_*(). Keep the old module ABI source-buildable
 * while the codec path is migrated to the stream API. */
#ifndef SLIM_SINK
#define SLIM_SINK 0
#endif
#ifndef SLIM_SRC
#define SLIM_SRC 1
#endif
#ifndef SLIM_AUTO_ISO
#define SLIM_AUTO_ISO 0
#endif
#ifndef SLIM_RATE_11025HZ
#define SLIM_RATE_11025HZ 9
#endif
#ifndef SLIM_RATE_4000HZ
#define SLIM_RATE_4000HZ 16
#endif
#ifndef SLIM_CH_DATAF_NOT_DEFINED
#define SLIM_CH_DATAF_NOT_DEFINED 0
#endif
#ifndef SLIM_CH_AUXF_NOT_APPLICABLE
#define SLIM_CH_AUXF_NOT_APPLICABLE 0
#endif
#ifndef SLIM_CH_ACTIVATE
#define SLIM_CH_ACTIVATE 0
#endif
#ifndef SLIM_CH_SUSPEND
#define SLIM_CH_SUSPEND 1
#endif
#ifndef SLIM_CH_REMOVE
#define SLIM_CH_REMOVE 2
#endif

struct slim_ch {
	u8 prot;
	u8 baser;
	u16 ratem;
	u8 dataf;
	u8 auxf;
	u8 sampleszbits;
};

static inline int slim_get_slaveport(u8 laddr, u16 port, u32 *sph,
			u32 path)
{
	if (!sph)
		return -EINVAL;
	*sph = port;
	return 0;
}

static inline int slim_query_ch(struct slim_device *sdev, u32 ch_num,
			u16 *ch_h)
{
	if (!ch_h)
		return -EINVAL;
	*ch_h = ch_num;
	return 0;
}

static inline int slim_dealloc_ch(struct slim_device *sdev, u16 ch_h)
{
	return 0;
}

static inline int slim_define_ch(struct slim_device *sdev,
			struct slim_ch *prop, u16 *ch_h, u8 count,
			bool commit, u16 *grph)
{
	if (!prop || !ch_h || !count || !grph)
		return -EINVAL;
	*grph = ch_h[0];
	return 0;
}

static inline int slim_connect_sink(struct slim_device *sdev, u32 *sph,
			int count, u16 ch_h)
{
	return (!sph || count < 1) ? -EINVAL : 0;
}

static inline int slim_connect_src(struct slim_device *sdev, u32 sph,
			u16 ch_h)
{
	return 0;
}

static inline int slim_control_ch(struct slim_device *sdev, u16 grph,
			int control, bool commit)
{
	return 0;
}

static inline int slim_disconnect_ports(struct slim_device *sdev,
			u32 *sph, int count)
{
	return (!sph || count < 0) ? -EINVAL : 0;
}
#endif
