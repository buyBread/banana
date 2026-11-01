#pragma once

#include "vc8/map.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;
    class game_meter_base;
    class meter_data_file;

    class game_meter_manager {

    public:
        entity*                         hero;
        vc8::map<i32, game_meter_base*> meter_dict;
        meter_data_file*                meter_data;

        // inlined @ sub_93D010
        void unregister_game_meters() {
            meter_dict.clear();

            meter_data = nullptr;
        }
    };

    ASSERT_OFFSETOF(game_meter_manager, hero,       0x00);
    ASSERT_OFFSETOF(game_meter_manager, meter_dict, 0x04);
    ASSERT_OFFSETOF(game_meter_manager, meter_data, 0x10);

    namespace references {
        inline util::memory_reference<game_meter_manager*> game_meter_manager { 0x010F9BD4 };

        // the meter behaviour files the hero loads for each suit
        inline util::memory_reference<string_hash> redsuit_behaviors   { 0x01101158 };
        inline util::memory_reference<string_hash> blacksuit_behaviors { 0x01110B5C };
    } // references
} // treyarch
