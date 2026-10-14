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
    class meshmap;

    // a named resource, looked up when the owner is constructed from mash
    template<typename T>
    struct reference {
        string_hash name;
        T*          pointer;
        u32         unk_08; // only textures set it

        // sub_7DA210
        // sub_7DA270
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

    template<> void reference<ngl::texture>::initialize(mash::allocation_scope scope);
    template<> void reference<meshmap>::initialize(mash::allocation_scope scope);
    template<> void reference<adjustment_layers_library>::initialize(mash::allocation_scope scope);

    ASSERT_SIZEOF  (reference<void>,          0x0C);
    ASSERT_OFFSETOF(reference<void>, pointer, 0x04);
    ASSERT_OFFSETOF(reference<void>, unk_08,  0x08);
} // treyarch
