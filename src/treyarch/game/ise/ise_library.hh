#pragma once

#include "treyarch/game/ise/effect.hh"
#include "treyarch/shared/mash/avl_tree.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct ise_library_entry {
        u32          name_hash; // effects are spawned by a name, found by its hash
        ise::effect* effect;

        ~ise_library_entry() {
            if (effect)
                delete effect;

            effect = nullptr;
        }

        bool operator<(const ise_library_entry &other) const {
            return name_hash < other.name_hash;
        }

        // inlined @ sub_A6F4B0
        void construct_mashed_class() {
            if (effect)
                effect = (ise::effect*)mash::mash_virtual_base::construct_class_helper(effect);
        }

        // inlined @ sub_A6F250
        void destruct_mashed_class() {
            if (effect) {
                effect->destruct_mashed_class();
                effect = nullptr;
            }
        }

        // inlined @ sub_A6F3F0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            u8 class_mashed;
            mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

            if (class_mashed == mash::member_class_mashed)
                mash_info->unmash_class(effect, this, buffer);
            else
                effect = nullptr;
        }
    };

    // the ise manager keeps every loaded library and spawns effects from them
    class ise_library {

    public:
        void* self; // the resource handler points this at the library itself

        mash::avl_tree
            <ise_library_entry> effects;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);

    private:
        void initialize(mash::allocation_scope scope);
        void finalize(mash::allocation_scope scope);
    };

    ASSERT_SIZEOF  (ise_library_entry,         0x08);
    ASSERT_OFFSETOF(ise_library_entry, effect, 0x04);

    ASSERT_SIZEOF  (ise_library,          0x14);
    ASSERT_OFFSETOF(ise_library, effects, 0x04);
} // treyarch
