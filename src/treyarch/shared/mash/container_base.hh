#pragma once

#include "treyarch/shared/mash/mash_info.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace mash {
    class container_base {

public:
        u32 mash_image_offset;
        u32 size;

        container_base() : mash_image_offset(0),
                           size(0) {}

        ~container_base() {
            clear();
        }

        u32 get_mash_image_offset() const {
            return mash_image_offset;
        }

        bool empty() const {
            return size == 0;
        }

        bool from_mash() const {
            return mash_image_offset > 0;
        }

        bool is_pointer_in_mash_image(const void* pointer) const {
            /*
                mash containers borrow their object storage from one contiguous image;
                the offset points from the container itself to the cursor after its last unmash allocation
            */

            u32 image_begin = (u32)this;
            u32 image_end   = image_begin + mash_image_offset;
            u32 address     = (u32)pointer;

            // retail treats both ends as part of the image
            return address >= image_begin && address <= image_end;
        }

        void custom_unmash_base(mash_info_struct* mash_info, buffer_type buffer) {
            // keep this relative; the whole image is free to move before it gets unmash'd
            mash_image_offset =
                (u32)(mash_info->get_curr_buffer_pos(buffer) - (u8*)this);
        }

        void clear() {
            mash_image_offset = 0;
            size              = 0;
        }

        void construct_mashed_class() {}

        void destruct_mashed_class() {
            clear();
        }

        void unmash(mash_info_struct* mash_info, void*, buffer_type) {
            // the in-object count is only a placeholder; every generated container takes the real one from shared
            mash_info->read_from_buffer(SHARED_BUFFER, size);
        }

        i32 get_mash_sizeof() const {
            return (i32)sizeof(*this);
        }
    };

    ASSERT_SIZEOF  (container_base,                    0x08);
    ASSERT_OFFSETOF(container_base, mash_image_offset, 0x00);
    ASSERT_OFFSETOF(container_base, size,              0x04);
}} // treyarch::mash
