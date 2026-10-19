#pragma once

#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // one entity's own mash image; an entity whose shared_index isn't its own takes its shared stream from that one
    class entity_mash {

    public:
        u8  unk_00[0x10];
        u16 shared_index;
        u16 unk_12;

        void construct_mashed_class() {}
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };

    // a slot's scene entities; each is unmashed from its own entry as the slot loads
    class alignas(64) scene_entity_container {

    public:
        mash::vector<entity_mash> entity_mashes;
        mash::vector_basic<u32>   unk_14;
        u8                        pad_024[0x1C];

        // sub_64B150
        void construct_mashed_class() {
            entity_mashes.construct_mashed_class();
            unk_14       .construct_mashed_class();
        }

        // sub_64D290
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            entity_mashes.unmash(mash_info, this, buffer);
            unk_14       .unmash(mash_info, this, buffer);
        }
    };

    ASSERT_SIZEOF  (entity_mash,               0x14);
    ASSERT_OFFSETOF(entity_mash, shared_index, 0x10);

    ASSERT_SIZEOF  (scene_entity_container,         0x40);
    ASSERT_OFFSETOF(scene_entity_container, unk_14, 0x14);
} // treyarch
