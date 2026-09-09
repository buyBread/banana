#pragma once

#include <cassert>
#include <type_traits>

#include "treyarch/shared/mash/container_base.hh"
#include "treyarch/shared/memory/game_heap.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace mash {
    template<typename T>
    class vector : public container_base {

public:
        T**  data;
        u32  capacity;
        bool destroy_elements;
        u8   pad[3];

        vector() : container_base(),
                   data(nullptr),
                   capacity(0),
                   destroy_elements(true),
                   pad{} {}

        ~vector() {
            clear();
        }

        T* &operator[](u32 index) {
            assert(index < size);
            assert(data != nullptr);

            return data[index];
        }

        T* const &operator[](u32 index) const {
            assert(index < size);
            assert(data != nullptr);

            return data[index];
        }

              T** begin()       { return data; }
        T* const* begin() const { return data; }

              T** end()       { return data ? data + size : nullptr; }
        T* const* end() const { return data ? data + size : nullptr; }

        void custom_unmash(mash_info_struct* mash_info,
                           void*             containing_class_ptr,
                           buffer_type       buffer) {
            capacity = size;

            if (size) {
                // serialized pointers are garbage until every table slot is replaced with its object address
                data = (T**)mash_info
                    ->read_from_buffer(buffer,
                                       (i32)(size * sizeof(T*)),
                                       (i32)alignof(T*));

                for (u32 index = 0; index < size; ++index) {
                    mash_info->unmash_class(data[index],
                                            containing_class_ptr,
                                            buffer);

                    assert(!std::is_polymorphic_v<T> ||
                        mash_virtual_base::verify_valid_vtable(data[index]));
                }
            } else
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

            if (!data)
                return;

            for (u32 index = 0; index < size; ++index)
                mash_info_struct::construct_class(data[index]);
        }

        void destruct_mashed_class() {
            clear();
            container_base::destruct_mashed_class();
        }

        i32 get_mash_sizeof() const {
            return (i32)sizeof(*this);
        }

        void clear() {
            if (destroy_elements) {
                while (size) {
                    T* element = data[--size];

                    // borrowed objects get their generated teardown, but the image owns their bytes
                    if (is_pointer_in_mash_image(element))
                        element->destruct_mashed_class();
                    else if (element) {
                        // a native virtual deleting destructor knows which heap its own object came from
                        if constexpr (std::has_virtual_destructor_v<T>)
                            delete element;
                        else {
                            element->~T();
                            memory::game_heap::free(element);
                        }
                    }

                    data[size] = nullptr;
                }
            }

            if (!is_pointer_in_mash_image(data))
                memory::game_heap::free(data);

            data     = nullptr;
            capacity = 0;

            container_base::clear();
        }
    };

    ASSERT_SIZEOF  (vector<void>,                    0x14);
    ASSERT_OFFSETOF(vector<void>, mash_image_offset, 0x00);
    ASSERT_OFFSETOF(vector<void>, size,              0x04);
    ASSERT_OFFSETOF(vector<void>, data,              0x08);
    ASSERT_OFFSETOF(vector<void>, capacity,          0x0C);
    ASSERT_OFFSETOF(vector<void>, destroy_elements,  0x10);
}} // treyarch::mash
