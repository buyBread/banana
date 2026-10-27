#pragma once

#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

// Doug Lea's dlmalloc 2.8.3 as mm builds it: mspaces with footers and checks, no locks, no MORECORE,
// segments mapped through the memory space's callbacks
namespace treyarch { namespace mm {
    struct malloc_chunk {
        u32           prev_foot;
        u32           head;
        malloc_chunk* fd;
        malloc_chunk* bk;
    };

    struct malloc_tree_chunk {
        u32                prev_foot;
        u32                head;
        malloc_tree_chunk* fd;
        malloc_tree_chunk* bk;
        malloc_tree_chunk* child[2];
        malloc_tree_chunk* parent;
        u32                index;
    };

    struct malloc_segment {
        u8*             base;
        u32             size;
        malloc_segment* next;
        u32             sflags;
    };

    struct malloc_state {
        u32                smallmap;
        u32                treemap;
        u32                dvsize;
        u32                topsize;
        u8*                least_addr;
        malloc_chunk*      dv;
        malloc_chunk*      top;
        u32                trim_check;
        u32                magic;
        malloc_chunk*      smallbins[(32 + 1) * 2];
        malloc_tree_chunk* treebins[32];
        u32                footprint;
        u32                max_footprint;
        u32                mflags;
        memory_callbacks   callbacks; // where the mutex would be
        malloc_segment     seg;
    };

    struct malloc_params {
        u32 magic;
        u32 page_size;
        u32 granularity;
        u32 mmap_threshold;
        u32 trim_threshold;
        u32 default_mflags;
    };

    malloc_state* create_mspace(u32 capacity, i32 locked, const memory_callbacks* callbacks);
    malloc_state* create_mspace_with_base(void* base, u32 capacity, i32 locked, const memory_callbacks* callbacks);
    u32           destroy_mspace(malloc_state* msp);

    void* mspace_malloc(malloc_state* msp, u32 bytes);
    void  mspace_free(malloc_state* msp, void* mem);
    void* mspace_memalign(malloc_state* msp, u32 alignment, u32 bytes);
    u32   mspace_usable_size(void* mem);

    namespace references {
        inline util::memory_reference<malloc_params> mparams { 0x00FFD264 };
        inline util::memory_reference<malloc_state>  gm      { 0x00FFD680 };
    } // references

    ASSERT_SIZEOF  (malloc_chunk,            0x10);
    ASSERT_OFFSETOF(malloc_chunk, head,      0x04);
    ASSERT_OFFSETOF(malloc_chunk, fd,        0x08);
    ASSERT_OFFSETOF(malloc_chunk, bk,        0x0C);

    ASSERT_SIZEOF  (malloc_tree_chunk,         0x20);
    ASSERT_OFFSETOF(malloc_tree_chunk, child,  0x10);
    ASSERT_OFFSETOF(malloc_tree_chunk, parent, 0x18);
    ASSERT_OFFSETOF(malloc_tree_chunk, index,  0x1C);

    ASSERT_SIZEOF  (malloc_segment,         0x10);
    ASSERT_OFFSETOF(malloc_segment, sflags, 0x0C);

    ASSERT_SIZEOF  (malloc_state,                0x1D4);
    ASSERT_OFFSETOF(malloc_state, topsize,       0x00C);
    ASSERT_OFFSETOF(malloc_state, least_addr,    0x010);
    ASSERT_OFFSETOF(malloc_state, dv,            0x014);
    ASSERT_OFFSETOF(malloc_state, top,           0x018);
    ASSERT_OFFSETOF(malloc_state, trim_check,    0x01C);
    ASSERT_OFFSETOF(malloc_state, magic,         0x020);
    ASSERT_OFFSETOF(malloc_state, smallbins,     0x024);
    ASSERT_OFFSETOF(malloc_state, treebins,      0x12C);
    ASSERT_OFFSETOF(malloc_state, footprint,     0x1AC);
    ASSERT_OFFSETOF(malloc_state, max_footprint, 0x1B0);
    ASSERT_OFFSETOF(malloc_state, mflags,        0x1B4);
    ASSERT_OFFSETOF(malloc_state, callbacks,     0x1B8);
    ASSERT_OFFSETOF(malloc_state, seg,           0x1C4);

    ASSERT_SIZEOF  (malloc_params,                 0x18);
    ASSERT_OFFSETOF(malloc_params, granularity,    0x08);
    ASSERT_OFFSETOF(malloc_params, trim_threshold, 0x10);
    ASSERT_OFFSETOF(malloc_params, default_mflags, 0x14);
}} // treyarch::mm
