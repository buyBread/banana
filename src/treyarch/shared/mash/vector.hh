#pragma once

#include <cassert>
#include <type_traits>

#include "treyarch/shared/mash/container_base.hh"
#include "treyarch/shared/memory/heap.hh"
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

        // sub_A1BAE0
        // sub_A1BAF0
        // sub_A1C120
        // sub_A1C240
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

        // sub_43B260
        // sub_43E330
        // sub_4422C0
        // sub_445FC0
        // sub_4CDCF0
        // sub_594090
        // sub_596030
        // sub_596160
        // sub_59C210
        // sub_5E79A0
        // sub_5E7C70
        // sub_5E7D60
        // sub_63DF50
        // sub_6E6AE0
        // sub_6E6BC0
        // sub_6E6CA0
        // sub_75CD60
        // sub_7627E0
        // sub_7A5B70
        // sub_7DA390
        // sub_7DA4C0
        // sub_7DA590
        // sub_7DA670
        // sub_7DA7C0
        // sub_7DAA20
        // sub_7DC3F0
        // sub_7DC490
        // sub_7DCB50
        // sub_7DCBF0
        // sub_7DDFE0
        // sub_7DE0B0
        // sub_7FB980
        // sub_7FBA70
        // sub_7FBB30
        // sub_7FE860
        // sub_8013F0
        // sub_801ED0
        // sub_932D60
        // sub_932E50
        // sub_976230
        // sub_978890
        // sub_A1D7F0
        // sub_A1FFF0
        // sub_A200C0
        // sub_A20880
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

        // sub_4421F0
        // sub_4CF0B0
        // sub_597A50
        // sub_6F1D60
        // sub_764A50
        // sub_7DCA80
        // sub_7DE950
        // sub_8073B0
        // sub_937950
        void construct_mashed_class() {
            container_base::construct_mashed_class();

            initialize(FROM_MASH);
        }

        // sub_5A6350
        // sub_7DDD40
        // sub_7FFC70
        // sub_979970
        void initialize(allocation_scope scope) {
            if (scope == ALLOCATED) {
                data             = nullptr;
                capacity         = 0;
                destroy_elements = true;

                return;
            }

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

        // sub_4398F0
        // sub_4450D0
        // sub_4BA940
        // sub_4DC7D0
        // sub_595FA0
        // sub_59C180
        // sub_5A0A40
        // sub_6CC410
        // sub_6CC4A0
        // sub_6D7330
        // sub_6D73C0
        // sub_762770
        // sub_7D35E0
        // sub_7D8DC0
        // sub_7D8F00
        // sub_7DC180
        // sub_7DC210
        // sub_7DC280
        // sub_7DC310
        // sub_7DC380
        // sub_7DD520
        // sub_7DDDC0
        // sub_7F8980
        // sub_7FB8F0
        // sub_801110
        // sub_8011A0
        // sub_804EF0
        // sub_928F90
        // sub_974250
        // sub_976150
        // sub_A1ABC0
        // sub_A1AC90
        // sub_A1BEA0
        // sub_A1C1B0
        void clear() {
            if (destroy_elements) {
                while (size) {
                    destroy_element(data[size - 1]);

                    --size;
                }
            }

            if (!is_pointer_in_mash_image(data))
                memory::heap::free(data);

            data     = nullptr;
            capacity = 0;

            container_base::clear();
        }

        // sub_4DB230
        // sub_75CCD0
        // sub_7DA450
        // sub_7DA730
        // sub_7DAAE0
        // sub_9741C0
        void destroy_element(T* &element) {
            // borrowed objects get their generated teardown, but the image owns their bytes
            if (is_pointer_in_mash_image(element))
                element->destruct_mashed_class();
            else if (element) {
                // a native virtual deleting destructor knows which heap its own object came from
                if constexpr (std::has_virtual_destructor_v<T>)
                    delete element;
                else {
                    element->~T();
                    memory::heap::free(element);
                }
            }

            element = nullptr;
        }
    };

    ASSERT_SIZEOF  (vector<void>,                    0x14);
    ASSERT_OFFSETOF(vector<void>, mash_image_offset, 0x00);
    ASSERT_OFFSETOF(vector<void>, size,              0x04);
    ASSERT_OFFSETOF(vector<void>, data,              0x08);
    ASSERT_OFFSETOF(vector<void>, capacity,          0x0C);
    ASSERT_OFFSETOF(vector<void>, destroy_elements,  0x10);
}} // treyarch::mash
