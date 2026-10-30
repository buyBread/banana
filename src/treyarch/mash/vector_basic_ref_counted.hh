#pragma once

#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace mash {
    namespace references {
        inline util::memory_reference<bool> unk_01126c84 { 0x01126C84 };
    } // references

    template<typename T>
    class vector_basic_ref_count_internals {

    public:
        vector_basic<T>* data;
        u16              ref_count;
        u16              dynamic;

        ~vector_basic_ref_count_internals() {
            finalize(ALLOCATED);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        void inc_ref() {
            ++ref_count;
        }

        void dec_ref() {
            --ref_count;
        }

        // inlined @ sub_64CFB0
        void construct_mashed_class() {
            dynamic = 0;
            inc_ref();
        }

        void destruct_mashed_class() {
            finalize(FROM_MASH);
        }

        // inlined @ sub_6515E0
        // inlined @ sub_651640
        // inlined @ sub_6516A0
        // inlined @ sub_651700
        void unmash(mash_info_struct* mash_info,
                    void*             containing_class_ptr,
                    buffer_type       buffer) {

            custom_unmash(mash_info, containing_class_ptr, buffer);
        }

        void custom_unmash(mash_info_struct* mash_info,
                           void*,
                           buffer_type       buffer) {

            mash_info->unmash_class(data, this, buffer);
        }

    private:
        // sub_63D960
        void finalize(allocation_scope) { // dynamic decides, not the scope
            if (references::unk_01126c84.read())
                ref_count = 0;

            if (dynamic) {
                if (data) { // delete through the game heap
                    data->~vector_basic();
                    mm::free(data);
                }
            } else
                data->destruct_mashed_class();

            data = nullptr;
        }
    };

    // several owners share one vector through the internals
    template<typename T>
    class vector_basic_ref_counted {

    public:
        vector_basic_ref_count_internals<T>* internals;

        // sub_6584A0
        ~vector_basic_ref_counted() {
            finalize(ALLOCATED);
        }

        // sub_64C420
        void destroy_internals() {
            internals->dec_ref();

            if (internals->ref_count == 0) {
                if (internals->dynamic)
                    delete internals;
                else
                    internals->destruct_mashed_class();
            }

            internals = nullptr;
        }

        u32 size() const {
            return internals->data->size;
        }

        bool empty() const {
            return internals->data->empty();
        }

              T &operator[](u16 index)       { return internals->data->array[index]; }
        const T &operator[](u16 index) const { return internals->data->array[index]; }

              vector_basic<T> &get_vector()       { return *internals->data; }
        const vector_basic<T> &get_vector() const { return *internals->data; }

        // inlined @ sub_64CFB0
        void construct_mashed_class() {
            if (internals)
                mash_info_struct::construct_class(internals);
        }

        // inlined @ sub_667BA0
        // inlined @ sub_66A1B0
        void destruct_mashed_class() {
            finalize(FROM_MASH);

            // destroy_internals already nulled it, so this never runs
            if (internals) {
                internals->destruct_mashed_class();
                internals = nullptr;
            }
        }

        // sub_6515E0
        // sub_651640
        // sub_6516A0
        // sub_651700
        void unmash(mash_info_struct* mash_info, void*, buffer_type) {
            u8 class_mashed = 0xFF;
            mash_info->read_from_buffer(SHARED_BUFFER, class_mashed);

            if (class_mashed == member_class_mashed)
                mash_info->unmash_class(internals, this, SHARED_BUFFER);
            else
                internals = nullptr;
        }

    private:
        void finalize(allocation_scope) {
            destroy_internals();
        }
    };

    ASSERT_SIZEOF  (vector_basic_ref_count_internals<u32>,            0x08);
    ASSERT_OFFSETOF(vector_basic_ref_count_internals<u32>, ref_count, 0x04);
    ASSERT_OFFSETOF(vector_basic_ref_count_internals<u32>, dynamic,   0x06);
    ASSERT_SIZEOF  (vector_basic_ref_counted<u32>,                    0x04);
}} // treyarch::mash
