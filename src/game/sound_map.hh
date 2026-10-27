#pragma once

#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct sound_map_entry {
        mash::string       name;
        mash::vector
            <mash::string> banks; // .bnk names

        // inlined @ sub_9038E0
        void construct_mashed_class() {
            name .construct_mashed_class();
            banks.construct_mashed_class();
        }

        // inlined @ sub_904110
        void destruct_mashed_class() {
            name .destruct_mashed_class();
            banks.destruct_mashed_class();
        }

        // inlined @ sub_904A70
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            name .unmash(mash_info, this, buffer);
            banks.unmash(mash_info, this, buffer);
        }
    };

    class sound_map {

    public:
        void* self; // the resource handler points this at the map itself

        mash::vector<sound_map_entry> entries;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sound_map_entry,        0x20);
    ASSERT_OFFSETOF(sound_map_entry, banks, 0x0C);

    ASSERT_SIZEOF  (sound_map,          0x18);
    ASSERT_OFFSETOF(sound_map, entries, 0x04);
} // treyarch
