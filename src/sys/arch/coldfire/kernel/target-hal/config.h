#ifndef COLDFIRE_HAL_CONFIG_H
#define COLDFIRE_HAL_CONFIG_H
/*
 * Copyright (C) 2007, The EROS Group, LLC.
 *
 * This file is part of the Coyotos Operating System runtime library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, 59 Temple Place - Suite 330 Boston, MA 02111-1307, USA.
 */

/** @file
 * @brief Definitions of various kernel constants.
 */

#ifndef MAX_NCPU
/** @brief Maximum (static) number of CPUs we will support */
#define MAX_NCPU 1
#endif

#if MAX_NCPU > 1
#error "This kernel implementation does not support more than one CPU"
#endif

/* Note that spacing is significant in the TRANSMAP_xxx macros. These
 * macros are referenced by the linker script ldscript.S, and the
 * linker script parser is very picky about tokenization. */

/** @brief Number of transmap entries reserved for each CPU. */
#define TRANSMAP_ENTRIES_PER_CPU 64

/** @brief Whether we have a console.
 */
#define HAVE_CONSOLE 0

/** @brief Number of entries in the physical region list.
 */
#define PHYSMEM_NREGION 64

/** @brief Set to the number of bits needed to index a particular PTE
 * in an arbitrary mapping table.
 *
 * This should be the width, in bits, of the index into the @em widest
 * page table on the current architecture.
 *
 * Coldfire is currently soft-translated, so this should be zero.
 */
#define MAPPING_INDEX_BITS 0

/** @brief Alignment value used for cache aligned data structures.
 *
 * This alignment value cannot be architecture-specific, because on
 * many architectures (e.g. Pentium) it is specific to each
 * implementation. In reality, this is an alignment value chosen to do
 * a ``good enough'' job of reducing cache line misses in practice.
 */
#define CACHE_LINE_SIZE         32

/** @brief Number of pages in each per-CPU stack.
 *
 * This value <em>must</em> be a power of two, and the per-CPU stack
 * <em>must</em> be aligned at a boundary that is the same as its
 * size. This is necessary in order for the curCPU macro to work. 
 */
#define KSTACK_NPAGES   0x1

/** @brief Starting virtual address for the kernel */
#define KVA 0xC0000000

/** @brief Limit above which heap must not grow.
 *
 * This should be selected to fall *below* any other magic virtual
 * addresses.
 *
 * For IA-32, we re-map the local APIC at 0xFF000000, so don't go
 * above that.
 */

#define HEAP_LIMIT_VA            0xFF000000

/** @brief Virtual base address of transient map. MUST be a multiple of 4M */
#define TRANSMAP_WINDOW_KVA      0xFF800000

#endif /* COLDFIRE_HAL_CONFIG_H */
