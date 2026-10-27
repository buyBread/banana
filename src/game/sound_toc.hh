#pragma once

#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the wwise file open looks its file id up by name and opens file_name from the audio folder
    struct sound_toc_entry {
        string_hash  name;      // the file name without its extension
        mash::string file_name; // .bnk or .wav
        mash::string bank_name;
        string_hash  language;  // "none" outside the language packs
        u32          offset;    // into bank_name
        u32          size;

        // inlined @ sub_902E10
        void construct_mashed_class() {
            file_name.construct_mashed_class();
            bank_name.construct_mashed_class();
        }

        // inlined @ sub_902A80
        void destruct_mashed_class() {
            file_name.destruct_mashed_class();
            bank_name.destruct_mashed_class();
        }

        // inlined @ sub_902B20
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            file_name.unmash(mash_info, this, buffer);
            bank_name.unmash(mash_info, this, buffer);
        }
    };

    class sound_toc {

    public:
        void* self; // the resource handler points this at the toc itself

        sound_toc*            next;
        mash::vector
            <sound_toc_entry> entries;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (sound_toc_entry,           0x28);
    ASSERT_OFFSETOF(sound_toc_entry, bank_name, 0x10);
    ASSERT_OFFSETOF(sound_toc_entry, language,  0x1C);
    ASSERT_OFFSETOF(sound_toc_entry, size,      0x24);

    ASSERT_SIZEOF  (sound_toc,          0x1C);
    ASSERT_OFFSETOF(sound_toc, next,    0x04);
    ASSERT_OFFSETOF(sound_toc, entries, 0x08);

    namespace references {
        inline util::memory_reference<sound_toc*> sound_tocs { 0x010F8AE8 }; // the newest loaded toc
    } // references
} // treyarch
