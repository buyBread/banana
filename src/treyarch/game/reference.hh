#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    namespace mash {
        class mash_info_struct;
    } // mash

    namespace ngl {
        struct texture;
    } // ngl

    class adjustment_layers_library;
    class environment;
    class light_behavior;
    class meshmap;
    class outdoors;

    // a named resource, looked up when the owner is constructed from mash
    template<typename T>
    struct reference {
        string_hash name;
        T*          pointer;

        ~reference() {
            finalize(mash::ALLOCATED);
        }

        // sub_7A08A0
        // sub_7DA1B0
        // sub_7DA210
        // sub_7DA270
        // sub_7DA2D0
        void construct_mashed_class() {
            initialize(mash::FROM_MASH);
        }

        void destruct_mashed_class() {
            finalize(mash::FROM_MASH);
        }

        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}

    private:
        void initialize(mash::allocation_scope scope);

        void finalize(mash::allocation_scope) {
            if (pointer)
                pointer = nullptr;
        }
    };

    // texture references also keep a value derived from the texture's size
    template<>
    struct reference<ngl::texture> {
        string_hash   name;
        ngl::texture* pointer;
        u32           unk_08;

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
    };

    template<> void reference<adjustment_layers_library>::initialize(mash::allocation_scope scope);
    template<> void reference<environment>::initialize(mash::allocation_scope scope);
    template<> void reference<light_behavior>::initialize(mash::allocation_scope scope);
    template<> void reference<meshmap>::initialize(mash::allocation_scope scope);
    template<> void reference<outdoors>::initialize(mash::allocation_scope scope);

    ASSERT_SIZEOF  (reference<void>,          0x08);
    ASSERT_OFFSETOF(reference<void>, pointer, 0x04);

    ASSERT_SIZEOF  (reference<ngl::texture>,         0x0C);
    ASSERT_OFFSETOF(reference<ngl::texture>, unk_08, 0x08);
} // treyarch
