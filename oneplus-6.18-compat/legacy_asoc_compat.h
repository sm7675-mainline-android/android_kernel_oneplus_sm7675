#ifndef _LEGACY_ASOC_COMPAT_H
#define _LEGACY_ASOC_COMPAT_H

#include <sound/soc.h>

#ifndef asoc_rtd_to_codec
#define asoc_rtd_to_codec(rtd, n) snd_soc_rtd_to_codec((rtd), (n))
#endif

#ifndef asoc_substream_to_rtd
#define asoc_substream_to_rtd(substream) snd_soc_substream_to_rtd(substream)
#endif

#ifndef soc_multi_mixer_control
#define soc_multi_mixer_control soc_mixer_control
#endif

static inline int legacy_snd_soc_info_multi_ext(struct snd_kcontrol *kcontrol,
						 struct snd_ctl_elem_info *uinfo)
{
	struct soc_mixer_control *control =
		(struct soc_mixer_control *)kcontrol->private_value;

	uinfo->type = control->platform_max == 1 &&
		!strnstr(kcontrol->id.name, " Volume", 30) ?
		SNDRV_CTL_ELEM_TYPE_BOOLEAN : SNDRV_CTL_ELEM_TYPE_INTEGER;
	uinfo->count = control->num_channels;
	uinfo->value.integer.min = 0;
	uinfo->value.integer.max = control->platform_max ?
		control->platform_max : control->max;
	return 0;
}

#ifndef SOC_SINGLE_MULTI_EXT
#define SOC_SINGLE_MULTI_EXT(xname, xreg, xshift, xmax, xinvert, xcount, \
			     xhandler_get, xhandler_put) \
	{ .iface = SNDRV_CTL_ELEM_IFACE_MIXER, .name = (xname), \
	  .info = legacy_snd_soc_info_multi_ext, .get = (xhandler_get), \
	  .put = (xhandler_put), \
	  .private_value = (unsigned long)&(struct soc_mixer_control) { \
		.reg = (xreg), .shift = (xshift), .rreg = (xreg), \
		.rshift = (xshift), .max = (xmax), \
		.platform_max = (xmax), .invert = (xinvert), \
		.num_channels = (xcount) } }
#endif

#endif
