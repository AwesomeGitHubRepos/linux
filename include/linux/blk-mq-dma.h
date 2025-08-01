/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef BLK_MQ_DMA_H
#define BLK_MQ_DMA_H

#include <linux/blk-mq.h>
#include <linux/pci-p2pdma.h>

struct blk_dma_iter {
	/* Output address range for this iteration */
	dma_addr_t			addr;
	u32				len;

	/* Status code. Only valid when blk_rq_dma_map_iter_* returned false */
	blk_status_t			status;

	/* Internal to blk_rq_dma_map_iter_* */
	struct req_iterator		iter;
	struct pci_p2pdma_map_state	p2pdma;
};

bool blk_rq_dma_map_iter_start(struct request *req, struct device *dma_dev,
		struct dma_iova_state *state, struct blk_dma_iter *iter);
bool blk_rq_dma_map_iter_next(struct request *req, struct device *dma_dev,
		struct dma_iova_state *state, struct blk_dma_iter *iter);

/**
 * blk_rq_dma_map_coalesce - were all segments coalesced?
 * @state: DMA state to check
 *
 * Returns true if blk_rq_dma_map_iter_start coalesced all segments into a
 * single DMA range.
 */
static inline bool blk_rq_dma_map_coalesce(struct dma_iova_state *state)
{
	return dma_use_iova(state);
}

/**
 * blk_rq_dma_unmap - try to DMA unmap a request
 * @req:	request to unmap
 * @dma_dev:	device to unmap from
 * @state:	DMA IOVA state
 * @mapped_len: number of bytes to unmap
 *
 * Returns %false if the callers need to manually unmap every DMA segment
 * mapped using @iter or %true if no work is left to be done.
 */
static inline bool blk_rq_dma_unmap(struct request *req, struct device *dma_dev,
		struct dma_iova_state *state, size_t mapped_len)
{
	if (req->cmd_flags & REQ_P2PDMA)
		return true;

	if (dma_use_iova(state)) {
		dma_iova_destroy(dma_dev, state, mapped_len, rq_dma_dir(req),
				 0);
		return true;
	}

	return !dma_need_unmap(dma_dev);
}

#endif /* BLK_MQ_DMA_H */
