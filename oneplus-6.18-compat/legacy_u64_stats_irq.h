#ifndef LEGACY_U64_STATS_IRQ_H
#define LEGACY_U64_STATS_IRQ_H

#include <linux/u64_stats_sync.h>

#define u64_stats_fetch_begin_irq u64_stats_fetch_begin
#define u64_stats_fetch_retry_irq u64_stats_fetch_retry

#endif
