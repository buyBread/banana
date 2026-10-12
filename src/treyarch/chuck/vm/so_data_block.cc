#include <cstring>

#include "treyarch/chuck/vm/so_data_block.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// inlined @ sub_A248D0
template<typename block_t>
void so_data_block::setup_fixed_block() {
    block_t* block = new block_t;

    // a failed allocation leaves the buffer at 4
    buffer      = (u8*)((fixed_so_data_block_base*)block + 1);
    fixed_block = block;
}

// sub_A248D0
void so_data_block::setup(i32 size) {
    destroy();

    blocksize = size;

    if (!size)
        buffer = nullptr;
    else if (size <= 32)
        setup_fixed_block<fixed_so_data_block_32>();
    else if (size <= 128)
        setup_fixed_block<fixed_so_data_block_128>();
    else if (size <= 512)
        setup_fixed_block<fixed_so_data_block_512>();
    else if (size <= 1400)
        setup_fixed_block<fixed_so_data_block_1400>();
    else if (size <= 3000)
        setup_fixed_block<fixed_so_data_block_3000>();
    else {
        flags  = (e_so_data_block_flags)(flags | so_data_block_flag_buffer_allocated);
        buffer = (u8*)memory::heap::allocate(size);
    }

    if (blocksize > 0)
        std::memset(buffer, 0, blocksize);
}

// sub_A24380
void so_data_block::destroy() {
    if (flags & so_data_block_flag_buffer_allocated) {
        memory::heap::free(buffer);

        flags     = (e_so_data_block_flags)(flags & ~so_data_block_flag_buffer_allocated);
        blocksize = 0;
        buffer    = nullptr;
    } else if (fixed_block) {
        delete fixed_block;

        fixed_block = nullptr;
        blocksize   = 0;
        buffer      = nullptr;
    }
}

// sub_A248C0
void so_data_block::finalize(mash::allocation_scope scope) {
    if (scope == mash::ALLOCATED)
        destroy();
}

// sub_A24A30
void so_data_block::custom_unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type stream) {
    buffer = (u8*)mash_info->read_from_buffer(stream, blocksize, 4);
}

// sub_A243D0
void so_data_block::set_to_zero() {
    if (blocksize > 0)
        std::memset(buffer, 0, blocksize);
}
