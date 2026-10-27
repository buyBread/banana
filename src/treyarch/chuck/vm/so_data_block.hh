#pragma once

#include <cstddef>

#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/ae_base/fixed_pool.hh"
#include "treyarch/mm/heap.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    enum e_so_data_block_flags : u32 {
        so_data_block_flag_from_mash        = 0x01,
        so_data_block_flag_buffer_allocated = 0x02
    };

    namespace references {
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_32_pool   { 0x011249C8 };
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_128_pool  { 0x011248F0 };
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_512_pool  { 0x01124938 };
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_1400_pool { 0x01124980 };
        inline util::memory_reference<memory::fixed_pool> fixed_so_data_block_3000_pool { 0x01124A10 };
    } // references

    class fixed_so_data_block_base {

    public:
        // sub_A24360
        virtual ~fixed_so_data_block_base() {}

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    // the data follows the vtable
    template<u32 size, util::memory_reference<memory::fixed_pool> &pool>
    class fixed_so_data_block : public fixed_so_data_block_base {

    public:
        u8 data[size];

        // sub_A24710
        // sub_A24760
        // sub_A247B0
        // sub_A24800
        // sub_A24850
        ~fixed_so_data_block() override {}

        // sub_A243F0
        // sub_A24490
        // sub_A24530
        // sub_A245D0
        // sub_A24670
        void* operator new(std::size_t) noexcept {
            return pool.get().allocate();
        }

        void operator delete(void* allocation) noexcept {
            pool.get().release(allocation);
        }
    };

    using fixed_so_data_block_32   = fixed_so_data_block<32,   references::fixed_so_data_block_32_pool>;
    using fixed_so_data_block_128  = fixed_so_data_block<128,  references::fixed_so_data_block_128_pool>;
    using fixed_so_data_block_512  = fixed_so_data_block<512,  references::fixed_so_data_block_512_pool>;
    using fixed_so_data_block_1400 = fixed_so_data_block<1400, references::fixed_so_data_block_1400_pool>;
    using fixed_so_data_block_3000 = fixed_so_data_block<3000, references::fixed_so_data_block_3000_pool>;

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
        template<typename block_t>
        void setup_fixed_block();
    };

    ASSERT_SIZEOF  (fixed_so_data_block_base, 0x04);
    ASSERT_SIZEOF  (fixed_so_data_block_32,   0x24);
    ASSERT_SIZEOF  (fixed_so_data_block_3000, 0xBBC);

    ASSERT_SIZEOF  (so_data_block,              0x10);
    ASSERT_OFFSETOF(so_data_block, buffer,      0x00);
    ASSERT_OFFSETOF(so_data_block, blocksize,   0x04);
    ASSERT_OFFSETOF(so_data_block, flags,       0x08);
    ASSERT_OFFSETOF(so_data_block, fixed_block, 0x0C);
}}} // treyarch::chuck::vm
