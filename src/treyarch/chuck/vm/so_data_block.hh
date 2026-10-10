#pragma once

#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/memory/fixed_pool.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    enum e_so_data_block_flags : u32 {
        so_data_block_flag_from_mash        = 0x01,
        so_data_block_flag_buffer_allocated = 0x02
    };

    // a retail fixed_so_data_block<size, pool, ...>; the data follows the vtable
    struct fixed_so_data_block_base {
        void** vtable;
    };

    namespace references {
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_32_pool   { 0x011249C8 };
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_128_pool  { 0x011248F0 };
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_512_pool  { 0x01124938 };
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_1400_pool { 0x01124980 };
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_3000_pool { 0x01124A10 };

        inline util::memory_reference<void*> fixed_so_data_block_32_vtable   { 0x00DBEC50 };
        inline util::memory_reference<void*> fixed_so_data_block_128_vtable  { 0x00DBEC58 };
        inline util::memory_reference<void*> fixed_so_data_block_512_vtable  { 0x00DBEC60 };
        inline util::memory_reference<void*> fixed_so_data_block_1400_vtable { 0x00DBEC68 };
        inline util::memory_reference<void*> fixed_so_data_block_3000_vtable { 0x00DBEC70 };
    } // references

    // instance member data and the master script-variable storage both live in one of these
    struct so_data_block {
        u8*                       buffer;
        i32                       blocksize;
        e_so_data_block_flags     flags;
        fixed_so_data_block_base* fixed_block; // 32/128/512/1400/3000-byte pools; null for heap fallback

        // sub_A248A0
        so_data_block() : buffer(nullptr),
                          blocksize(0),
                          flags((e_so_data_block_flags)0),
                          fixed_block(nullptr) {}

        ~so_data_block() {
            finalize(mash::ALLOCATED);
        }

        // sub_A248B0
        void construct_mashed_class() {
            flags = (e_so_data_block_flags)(flags | so_data_block_flag_from_mash);
        }

        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       stream) {

            custom_unmash(mash_info, containing_class_ptr, stream);
        }

        // the block's bytes follow in the stream
        void custom_unmash(mash::mash_info_struct* mash_info,
                           void*                   containing_class_ptr,
                           mash::buffer_type       stream);

        void setup(i32 size);
        void destroy();
        void finalize(mash::allocation_scope scope);

        void set_to_zero();

    private:
        void setup_fixed_block(memory::fixed_pool &pool, void** vtable);
    };

    ASSERT_SIZEOF  (fixed_so_data_block_base, 0x04);

    ASSERT_SIZEOF  (so_data_block,              0x10);
    ASSERT_OFFSETOF(so_data_block, buffer,      0x00);
    ASSERT_OFFSETOF(so_data_block, blocksize,   0x04);
    ASSERT_OFFSETOF(so_data_block, flags,       0x08);
    ASSERT_OFFSETOF(so_data_block, fixed_block, 0x0C);
}}} // treyarch::chuck::vm
