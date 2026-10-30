#pragma once

#include <cstring>

#include "treyarch/mash/container_base.hh"
#include "treyarch/base/tl_system.hh"
#include "banana/util/types.hh"
#include "banana/util/macros/sanity_assert.hh"

namespace treyarch { namespace mash {
    template <class T>
    class vector_basic : public container_base {

    public:
        static const i32 block_alloc_size = 8;

    public:
        T*  array;
        u32 max_size;

                 vector_basic() = default;
        explicit vector_basic(generating_vtable_version so_do_nothing) : container_base(so_do_nothing) {}

        // sub_971580
        ~vector_basic() {
            clear();
        }

              T* begin()       { return array; }
        const T* begin() const { return array; }

              T* end()       { return array ? array + size : nullptr; }
        const T* end() const { return array ? array + size : nullptr; }

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

            max_size = size;

            // the elements are plain data and are used where they lie
            if (size)
                array = (T*)mash_info->read_from_buffer(buffer, (i32)(size * sizeof(T)), (i32)alignof(T));
            else
                array = nullptr;

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
            if (!is_pointer_in_mash_image(array))
                tl_mem_free(array);

            array    = nullptr;
            max_size = 0;

            container_base::clear();
        }

        // sub_588AA0
        // sub_6063B0
        // inlined @ sub_87A810
        void reserve(u32 new_size) {
            if (new_size <= max_size)
                return;

            auto new_array = (T*)tl_mem_alloc(new_size * sizeof(T), 4, 0);

            if (array) {
                if (size)
                    std::memcpy(new_array, array, size * sizeof(T));

                if (!is_pointer_in_mash_image(array))
                    tl_mem_free(array);
            }

            array    = new_array;
            max_size = new_size;
        }

        // sub_4397F0
        // inlined @ sub_879DA0
        // inlined @ sub_87A810
        void push_back(const T &value) {
            // an array still in the mash image can't grow in place
            if (size == max_size || is_pointer_in_mash_image(array))
                reserve((size / 8 + 1) * 8);

            ++size;
            array[size - 1] = value;
        }

        // sub_8434C0
        void resize(u32 count, const T &value) {
            reserve(count);

            for (; size < count; ++size)
                array[size] = value;

            size = count;
        }

        // sub_473630
        vector_basic &operator=(const vector_basic &other) {
            clear();

            if (other.size) {
                reserve(other.size);

                size = other.size;
                std::memcpy(array, other.array, size * sizeof(T));
            }

            return *this;
        }
    };

    ASSERT_SIZEOF  (vector_basic<void*>,                    0x10);
    ASSERT_OFFSETOF(vector_basic<void*>, mash_image_offset, 0x00);
    ASSERT_OFFSETOF(vector_basic<void*>, size,              0x04);
    ASSERT_OFFSETOF(vector_basic<void*>, array,             0x08);
    ASSERT_OFFSETOF(vector_basic<void*>, max_size,          0x0C);
}} // treyarch::mash
