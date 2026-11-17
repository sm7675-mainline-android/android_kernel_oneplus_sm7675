#include <linux/mod_devicetable.h>
#include <linux/mhi.h>
#include <linux/of.h>
#include <linux/random.h>

#ifndef MHI_MAX_MTU
#define MHI_MAX_MTU 0xffff
#endif

#define MHI_CB_DTR_SIGNAL 10
#define MHI_CB_DTR_START_CHANNELS 11

#ifndef prandom_u32_max
#define prandom_u32_max(max) get_random_u32_below(max)
#endif
