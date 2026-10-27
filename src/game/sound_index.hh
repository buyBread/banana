#pragma once

#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // turns a game-side name into the wwise id that events, switches, states and rtpcs get posted with
    struct sound_index_entry {
        string_hash category; // "event", "switch", "state" or "rpcp"
        string_hash name;
        u32         id;
    };

    class sound_index {

    public:
        void* self; // the resource handler points this at the index itself

        mash::vector_basic
            <sound_index_entry> entries; // sorted by name

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sound_index_entry,     0x0C);
    ASSERT_OFFSETOF(sound_index_entry, id, 0x08);

    ASSERT_SIZEOF  (sound_index,          0x14);
    ASSERT_OFFSETOF(sound_index, entries, 0x04);

    namespace references {
        inline util::memory_reference<sound_index*> sound_index { 0x010F8AE4 };
    } // references
} // treyarch
