#pragma once

#include "treyarch/shared/mash/container_base.hh"
#include "treyarch/shared/memory/memory.hh"
#include "util/types.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace mash {
    template <class T>
    class vector_basic : public container_base {

    public:
        static const i32 block_alloc_size = 8;

    public:
        T*  data;
        u32 capacity;

        // sub_971580
        ~vector_basic() {
            clear();
        }

        // sub_4A45B0
        // sub_5CD470
        // sub_620B30
        // sub_7C2B00
        // sub_7C2B70
        // sub_7C2BE0
        // sub_7C2C50
        // sub_A75710
        void custom_unmash(mash_info_struct* mash_info,
                           void*,
                           buffer_type       buffer) {

            capacity = size;

            // the elements are plain data and are used where they lie
            if (size)
                data = (T*)mash_info->read_from_buffer(buffer, (i32)(size * sizeof(T)), (i32)alignof(T));
            else
                data = nullptr;

            container_base::custom_unmash_base(mash_info, buffer);
        }

        void unmash(mash_info_struct* mash_info,
                    void*             containing_class_ptr,
                    buffer_type       buffer) {

            container_base::unmash(mash_info, containing_class_ptr, buffer);
            custom_unmash(mash_info, containing_class_ptr, buffer);
        }

        void construct_mashed_class() {
            container_base::construct_mashed_class();
        }

        void destruct_mashed_class() {
            clear();
            container_base::destruct_mashed_class();
        }

        void clear() {
            // unlike vector, the storage goes back through the plain allocator
            if (!is_pointer_in_mash_image(data))
                memory::free(data);

            data     = nullptr;
            capacity = 0;

            container_base::clear();
        }
    };

    ASSERT_SIZEOF  (vector_basic<void*>,                     0x10);
    ASSERT_OFFSETOF(vector_basic<void*>, mash_image_offset,  0x00);
    ASSERT_OFFSETOF(vector_basic<void*>, size,               0x04);
    ASSERT_OFFSETOF(vector_basic<void*>, data,               0x08);
    ASSERT_OFFSETOF(vector_basic<void*>, capacity,           0x0C);
}} // treyarch::mash
