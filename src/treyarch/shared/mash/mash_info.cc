#include <cassert>
#include <cstring>

#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/memory/memory.hh"

using namespace treyarch;

mash::mash_info_struct::mash_info_struct(io_mode new_mode,
                                         u8*     buffer,
                                         i32     buffer_max,
                                         bool    has_header) {

    state         = CONSTRUCTED;
    mode          = new_mode;
    automash_crcs = nullptr;

    header = has_header ? (mash_header*)buffer : nullptr;

    // the header sits in normal; its shared_offset cuts the one allocation into two independent streams
    mash_image_ptr[NORMAL_BUFFER]   = buffer;
    buffer_size_used[NORMAL_BUFFER] = 0;

    u32 normal_size = has_header ? header->shared_offset : (u32)buffer_max;

    mash_image_ptr[SHARED_BUFFER] = buffer + normal_size;

    buffer_size_used[SHARED_BUFFER] = 0;
    buffer_size_max[NORMAL_BUFFER]  = normal_size;
    buffer_size_max[SHARED_BUFFER]  = (u32)buffer_max - normal_size;

    if (has_header)
        buffer_size_used[NORMAL_BUFFER] += (u32)sizeof(mash_header);
}

void mash::mash_info_struct::dupe_buffers_for_unmash_copies() {
    /*
        unmash replaces keys and serialized relationships in place;
        a second copy therefore needs untouched bytes, not another cursor over the already-bashed graph
    */

    u32 unread_size =
        buffer_size_max[NORMAL_BUFFER] - buffer_size_used[NORMAL_BUFFER];

    u8* source =
        mash_image_ptr[NORMAL_BUFFER] + buffer_size_used[NORMAL_BUFFER];

    u8* copy = (u8*)memory::allocate(unread_size, 0x40, 0);

    mash_image_ptr[NORMAL_BUFFER] = copy;
    std::memcpy(copy, source, unread_size);
    buffer_size_used[NORMAL_BUFFER] = 0;
}

void mash::mash_info_struct::set_buffer(buffer_type buffer,
                                        u8*         buffer_ram,
                                        i32         buffer_size) {

    mash_image_ptr[buffer]   = buffer_ram;
    buffer_size_max[buffer]  = buffer_size;
    buffer_size_used[buffer] = 0;
}

i32 mash::mash_info_struct::align_buffer(buffer_type buffer,
                                         i32         alignment_required) {
                                            
    assert(mode != MASH_MODE || state == INITIALIZED);

    bool shred_padding = mode == MASH_MODE || mode == ALIGN_MODE;

    return align_buffer(mash_image_ptr[buffer],
                       &buffer_size_used[buffer],
                        alignment_required,
                        shred_padding);
}

i32 mash::mash_info_struct::align_buffer(u8*  buffer_ptr,
                                         u32* buffer_ptr_used,
                                         i32  alignment_required,
                                         bool shred_padding) {

    assert(buffer_ptr != nullptr);
    assert(buffer_ptr_used != nullptr);
    assert(alignment_required > 0);
    assert((alignment_required & (alignment_required - 1)) == 0);

    u8* unaligned = buffer_ptr + *buffer_ptr_used;

    // align the real address rather than the relative cursor; the image base isn't promised to be aligned for us
    u8* aligned = (u8*)(((size_t)unaligned + alignment_required - 1) & ~(size_t)(alignment_required - 1));

    i32 padding_size = (i32)(aligned - unaligned);

    if (padding_size && shred_padding)
        std::memset(unaligned, alignment_padding, padding_size);

    *buffer_ptr_used += padding_size;

    return padding_size;
}

void mash::mash_info_struct::deductive_align_buffer(buffer_type buffer) {
    u8* buffer_ptr = mash_image_ptr[buffer];
    u8* cursor = buffer_ptr + buffer_size_used[buffer];

    while (*cursor == alignment_padding)
        ++cursor;

    // yes, down: generated polymorphic reads use 0xA1 to discover alignment and then return to a dword boundary
    u32 new_size_used = (u32)(cursor - buffer_ptr);

    if (new_size_used & 3)
        new_size_used &= ~3u;

    buffer_size_used[buffer] = new_size_used;
}

void mash::mash_info_struct::advance_buffer(buffer_type buffer, i32 byte_count) {
    assert(mode != MASH_MODE || state == INITIALIZED);
    assert(mash_image_ptr[buffer] != nullptr);
    assert(byte_count >= 0);

    buffer_size_used[buffer] += byte_count;
}

i32 mash::mash_info_struct::header_size() const {
    return header ? (i32)sizeof(mash_header) : 0;
}

void* mash::mash_info_struct::read_from_buffer(buffer_type buffer,
                                               i32         read_size,
                                               i32         alignment_required) {

    if (alignment_required)
        align_buffer(buffer, alignment_required);
    else
        // an alignment of 0 is the generated "figure it out from the padding" path, not "unaligned"
        deductive_align_buffer(buffer);

    void* result = mash_image_ptr[buffer] + buffer_size_used[buffer];

    advance_buffer(buffer, read_size);

    return result;
}
