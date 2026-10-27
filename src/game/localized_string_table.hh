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
    namespace references {
        // one entry per global text id; the order table starts at -1 and gets sorted by hash on first use
        inline util::memory_reference<u32> global_text_hashes { 0x00E6C688 };
        inline util::memory_reference<i32> global_text_order  { 0x00E6FDA0 };
    } // references

    // the GLOBALTEXT pack's type-5 "<pack>_WIN" resource; script text follows the global text
    class localized_string_table {

    public:
        void*              self; // the resource handler points this at the table itself
        mash::vector
            <mash::string> strings;
        u32                unk_18; // the total string count in every shipped table
        i32                scripttext_number;

        // sub_75F020
        void construct_mashed_class() {
            self = nullptr;

            strings.construct_mashed_class();
        }

        // sub_6E06F0
        void destruct_mashed_class() {
            strings.destruct_mashed_class();
        }

        // sub_6F7600
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            strings.unmash(mash_info, this, buffer);
        }

        // sub_6AB9D0
        const char* lookup_localized_string(i32 num) const {
            return strings[num]->c_str();
        }
    };

    i32 find_global_text(string_hash id);

    ASSERT_SIZEOF  (localized_string_table,                    0x20);
    ASSERT_OFFSETOF(localized_string_table, strings,           0x04);
    ASSERT_OFFSETOF(localized_string_table, unk_18,            0x18);
    ASSERT_OFFSETOF(localized_string_table, scripttext_number, 0x1C);
} // treyarch
