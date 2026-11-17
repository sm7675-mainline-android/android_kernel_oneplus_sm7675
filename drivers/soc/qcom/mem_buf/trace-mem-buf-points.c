// SPDX-License-Identifier: GPL-2.0-only
/*
 * Tracepoint definitions for the mem_buf msgq module.
 *
 * mem-buf-msgq.c uses trace_send_alloc_req(), but the translation unit that
 * normally carries CREATE_TRACE_POINTS (mem-buf-dev-gh.c) is not part of the
 * shipped mem_buf_dev build.  Provide them here so mem_buf_msgq has a local
 * definition instead of importing an undefined tracepoint.
 */
#define CREATE_TRACE_POINTS
#include "trace-mem-buf.h"
