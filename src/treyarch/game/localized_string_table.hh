#pragma once

#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the GLOBALTEXT pack's type-5 "<pack>_WIN" resource; script text follows the global text
    class localized_string_table {

    public:
        void*                  self; // the resource handler points this at the table itself
        mash::vector
            <mash::string>     strings;
        u32                    unk_18; // the total string count in every shipped table
        i32                    scripttext_number;

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
    };

    ASSERT_SIZEOF  (localized_string_table,                    0x20);
    ASSERT_OFFSETOF(localized_string_table, strings,           0x04);
    ASSERT_OFFSETOF(localized_string_table, unk_18,            0x18);
    ASSERT_OFFSETOF(localized_string_table, scripttext_number, 0x1C);
} // treyarch
