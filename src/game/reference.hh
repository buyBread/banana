#pragma once

#include "game/universal_resource.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace mash {
        class mash_info_struct;
    } // mash

    // a named resource, looked up when the owner is constructed from mash
    template<typename T>
    struct reference {
        string_hash name;
        T*          pointer;

        ~reference() {
            finalize(mash::ALLOCATED);
        }

        // sub_7A08A0
        // sub_7A1700
        // sub_7DA090
        // sub_7DA0F0
        // sub_7DA150
        // sub_7DA1B0
        // sub_7DA210
        // sub_7DA270
        // sub_7DA2D0
        // sub_7F9B30
        // sub_933270
        // sub_9332D0
        // sub_933330
        void construct_mashed_class() {
            initialize(mash::FROM_MASH);
        }

        void destruct_mashed_class() {
            finalize(mash::FROM_MASH);
        }

        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}

    private:
        void initialize(mash::allocation_scope) {
            string_hash empty;

            empty.initialize(mash::ALLOCATED);

            if (name == empty) {
                pointer = nullptr;

                return;
            }

            pointer = universal_resource::get<T>(name);
        }

        void finalize(mash::allocation_scope) {
            if (pointer)
                pointer = nullptr;
        }
    };

    // texture references also keep the texture's size class
    template<>
    struct reference<ngl::texture> {
        string_hash   name;
        ngl::texture* pointer;
        i32           size_class; // indexes the texture size table

        ~reference() {
            finalize(mash::ALLOCATED);
        }

        void construct_mashed_class() {
            initialize(mash::FROM_MASH);
        }

        // sub_7B7700
        void destruct_mashed_class() {
            finalize(mash::FROM_MASH);
        }

        // sub_7D3C40
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}

    private:
        void initialize(mash::allocation_scope scope);

        void finalize(mash::allocation_scope) {
            if (pointer)
                pointer = nullptr;
        }

        void calculate_size_class();

        static i8 dimension_class(u32 dimension);
    };

    ASSERT_SIZEOF  (reference<void>,          0x08);
    ASSERT_OFFSETOF(reference<void>, pointer, 0x04);

    ASSERT_SIZEOF  (reference<ngl::texture>,             0x0C);
    ASSERT_OFFSETOF(reference<ngl::texture>, size_class, 0x08);
} // treyarch
