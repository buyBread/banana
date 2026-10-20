#pragma once

#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    struct ui_character_info {
        mash::string name;
        mash::string character_base;    // (?) like s1_symbiotelight
        f32          unk_18;            // 0 or 0.5
        f32          unk_1c;            // 0, 0.25, 0.5 or 0.75
        i32          unk_20;
        mash::string name_text;         // localized when the map is built
        mash::string character_variant; // (?) like s1_symbiotelight_slasher or s1_symbiotelight_snatcher

        // inlined @ sub_6D7450
        void construct_mashed_class() {
            name             .construct_mashed_class();
            character_base   .construct_mashed_class();
            name_text        .construct_mashed_class();
            character_variant.construct_mashed_class();
        }

        // inlined @ sub_6CC530
        void destruct_mashed_class() {
            name             .destruct_mashed_class();
            character_base   .destruct_mashed_class();
            name_text        .destruct_mashed_class();
            character_variant.destruct_mashed_class();
        }

        // inlined @ sub_6D7850
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            name             .unmash(mash_info, this, buffer);
            character_base   .unmash(mash_info, this, buffer);
            name_text        .unmash(mash_info, this, buffer);
            character_variant.unmash(mash_info, this, buffer);
        }
    };

    class ui_character_info_list {

    public:
        void* self; // the resource handler points this at the list itself

        mash::vector<ui_character_info> characters; // conversation speaker ids index this

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (ui_character_info,                    0x3C);
    ASSERT_OFFSETOF(ui_character_info, character_base,    0x0C);
    ASSERT_OFFSETOF(ui_character_info, unk_18,            0x18);
    ASSERT_OFFSETOF(ui_character_info, name_text,         0x24);
    ASSERT_OFFSETOF(ui_character_info, character_variant, 0x30);

    ASSERT_SIZEOF  (ui_character_info_list,             0x18);
    ASSERT_OFFSETOF(ui_character_info_list, characters, 0x04);

    namespace references {
        // the level start fills it from global_ui_char_info, keyed by character and variant
        inline util::memory_reference<void*> ui_character_info_map { 0x0102CE40 };
    } // references
} // treyarch
