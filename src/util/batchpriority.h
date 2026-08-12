// Copyright (c) 2026 tessera core
// See COPYING for license.

#ifndef TESSERA_UTIL_BATCHPRIORITY_H
#define TESSERA_UTIL_BATCHPRIORITY_H

/**
 * On platforms that support it, tell the kernel the calling thread is
 * CPU-intensive and non-interactive. See SCHED_BATCH in sched(7) for details.
 *
 */
void ScheduleBatchPriority();

#endif // TESSERA_UTIL_BATCHPRIORITY_H