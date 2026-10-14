#pragma once

#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/memory/heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace mash {
    // a fixed-size array behind a pointer; allocated ones come from new[], mashed ones live in the image
    template<typename T>
    class array_replacement_base {

    public:
        T* array;

        void initialize_helper_mash(u32 n) {
            if (!array)
                return;

            for (u32 index = 0; index < n; ++index)
                array[index].construct_mashed_class();
        }

        // delete[] through the engine heap; the element count sits in front of the array
        void finalize_helper_allocated(u32) {
            if (!array)
                return;

            u32* count = (u32*)array - 1;

            for (u32 index = *count; index > 0; --index)
                array[index - 1].~T();

            memory::heap::free(count);
        }

        // sub_6208D0
        void finalize_helper_mash(u32 n) {
            if (!array)
                return;

            for (u32 index = 0; index < n; ++index)
                array[index].destruct_mashed_class();
        }

        // sub_7D90D0
        void custom_unmash_helper(i32              n,
                                  mash_info_struct* mash_info,
                                  void*,
                                  buffer_type       buffer) {

            array = (T*)mash_info->read_from_buffer(buffer, n * (i32)sizeof(T), (i32)alignof(T));

            for (i32 index = 0; index < n; ++index)
                mash_info->unmash_class_in_place(array[index], nullptr, buffer);
        }
    };

    template<typename T, i32 n>
    class array_replacement : public array_replacement_base<T> {

    public:
        ~array_replacement() {
            finalize(ALLOCATED);
        }

        T &operator[](u32 index) {
            return this->array[index];
        }

        void construct_mashed_class() {
            this->initialize_helper_mash(n);
        }

        void destruct_mashed_class() {
            finalize(FROM_MASH);
        }

        void unmash(mash_info_struct* mash_info,
                    void*             containing_class_ptr,
                    buffer_type       buffer) {

            custom_unmash(mash_info, containing_class_ptr, buffer);
        }

        void custom_unmash(mash_info_struct* mash_info,
                           void*             containing_class_ptr,
                           buffer_type       buffer) {

            this->custom_unmash_helper(n, mash_info, containing_class_ptr, buffer);
        }

    private:
        void finalize(allocation_scope scope) {
            if (scope == ALLOCATED)
                this->finalize_helper_allocated(n);
            else
                this->finalize_helper_mash(n);
        }
    };

    ASSERT_SIZEOF(array_replacement_base<void*>, 0x04);
}} // treyarch::mash
