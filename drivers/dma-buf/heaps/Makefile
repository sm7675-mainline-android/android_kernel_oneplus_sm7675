# SPDX-License-Identifier: GPL-2.0
obj-$(CONFIG_DMABUF_HEAPS_DEFERRED_FREE) += deferred-free-helper.o
obj-$(CONFIG_DMABUF_HEAPS_PAGE_POOL)	+= page_pool.o
obj-$(CONFIG_DMABUF_HEAPS_SYSTEM)	+= system_heap.o
obj-$(CONFIG_DMABUF_HEAPS_CMA)		+= cma_heap.o
obj-$(CONFIG_QCOM_DMABUF_HEAPS)		+= qcom_dma_heaps.o

ccflags-y  += -I$(srctree)/mm/mm_osvelte/
# 6.18 dropped CONFIG_DMABUF_HEAPS_DEFERRED_FREE; link the helper into this
# module so deferred_free() has a provider.
qcom_dma_heaps-y			:= qcom_dma_heap.o qcom_dt_parser.o qcom_sg_ops.o \
					   deferred-free-helper.o
# ...and tell the helper it is a member of that module, so its module_init() is
# compiled out (otherwise the link fails with "duplicate symbol: init_module").
# Defined here so an ad-hoc `make M=...` cannot forget the -D.
ifneq ($(CONFIG_QCOM_DMABUF_HEAPS),)
ccflags-y += -DQCOM_DMA_HEAPS_COMPOSITE=1
endif
qcom_dma_heaps-$(CONFIG_OPLUS_FEATURE_MM_BOOSTPOOL) += ./mm_boost_pool/oplus_boost_pool.o
qcom_dma_heaps-$(CONFIG_QCOM_DMABUF_HEAPS_SYSTEM) += qcom_system_heap.o \
						     qcom_dynamic_page_pool.o
qcom_dma_heaps-$(CONFIG_QCOM_DMABUF_HEAPS_SYSTEM_SECURE) += qcom_secure_system_heap.o \
							    qcom_dynamic_page_pool.o \
							    qcom_dma_heap_secure_utils.o
qcom_dma_heaps-$(CONFIG_QCOM_DMABUF_HEAPS_CMA)	+= qcom_cma_heap.o
qcom_dma_heaps-$(CONFIG_QCOM_DMABUF_HEAPS_CARVEOUT) += qcom_carveout_heap.o \
						       qcom_dma_heap_secure_utils.o
qcom_dma_heaps-$(CONFIG_QCOM_DMABUF_HEAPS_BITSTREAM_CONTIG) += qcom_bitstream_contig_heap.o
qcom_dma_heaps-$(CONFIG_QCOM_DMABUF_HEAPS_UBWCP) += qcom_ubwcp_heap.o
qcom_dma_heaps-$(CONFIG_QCOM_DMABUF_HEAPS_TUI_CARVEOUT) += qcom_tui_carveout_heap.o
qcom_dma_heaps-$(CONFIG_QCOM_DMABUF_HEAPS_SYSTEM_MOVABLE) += qcom_system_movable_heap.o
