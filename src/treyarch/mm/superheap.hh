#pragma once

#include "treyarch/mm/mm.hh"
#include "treyarch/mm/superblock.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace mm {
    struct superblock_queue_node {
        volatile i64                next;
                 superblock_entry_t* value;
                 u32                 reserved_00c;
    };

    // a lock-free queue of superblocks; head and tail are tagged pointers
    struct superblock_queue {
        volatile i64 head;
        volatile i64 tail;

        bool dequeue(superblock_entry_t** output);
        void enqueue(superblock_entry_t* value);
    };

    // the superblocks of one block size
    struct size_class {
        u32                 block_size;
        superblock_entry_t* active;
        superblock_queue    available;
        superblock_entry_t* unused_entries;
        u32                 reserved_01c;
    };

    struct superblock_entry_slab {
        superblock_entry_slab* previous;
        u32                    used;
        u8                     storage[0x4000];
    };

    // the small-block allocator: blocks up to 0x100 bytes in four-byte size classes
    struct superheap_t {
        memory_space*          space;
        u32                    reserved_004;
        size_class             size_classes[64];
        superblock_entry_slab* entry_allocator;

        void* allocate(u32 size);
        void  deallocate(void* allocation);

        size_class* get_size_class(u32 block_size);

        superblock_entry_t* create_superblock_entry(u32 block_size, u8* superblock);
        void                destroy_superblock_entry(superblock_entry_t* entry);

        superblock_entry_t* create_superblock(u32 block_size);
        void                destroy_superblock(superblock_entry_t* entry);

        void register_superblock(superblock_entry_t* entry);
        void unregister_superblock(superblock_entry_t* entry);
    };

    superblock_entry_t* find_superblock_entry(void* allocation);

    // the general storage behind the superheap and the large allocations
    void* allocate_storage(u32 size, u32 alignment);
    void  free_storage(void* allocation);

    ASSERT_SIZEOF  (superblock_queue_node,        0x10);
    ASSERT_OFFSETOF(superblock_queue_node, next,  0x00);
    ASSERT_OFFSETOF(superblock_queue_node, value, 0x08);

    ASSERT_SIZEOF  (superblock_queue,       0x10);
    ASSERT_OFFSETOF(superblock_queue, head, 0x00);
    ASSERT_OFFSETOF(superblock_queue, tail, 0x08);

    ASSERT_SIZEOF  (size_class,                 0x20);
    ASSERT_OFFSETOF(size_class, block_size,     0x00);
    ASSERT_OFFSETOF(size_class, active,         0x04);
    ASSERT_OFFSETOF(size_class, available,      0x08);
    ASSERT_OFFSETOF(size_class, unused_entries, 0x18);

    ASSERT_SIZEOF  (superblock_entry_slab,           0x4008);
    ASSERT_OFFSETOF(superblock_entry_slab, previous, 0x0000);
    ASSERT_OFFSETOF(superblock_entry_slab, used,     0x0004);
    ASSERT_OFFSETOF(superblock_entry_slab, storage,  0x0008);

    ASSERT_SIZEOF  (superheap_t,                  0x810);
    ASSERT_OFFSETOF(superheap_t, space,           0x000);
    ASSERT_OFFSETOF(superheap_t, size_classes,    0x008);
    ASSERT_OFFSETOF(superheap_t, entry_allocator, 0x808);
}} // treyarch::mm
