#pragma once

#include "vc8/map.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class game_meter_base;
    class meter_data_file;

    class game_meter_manager {

    public:
        u32                             unk_00;
        vc8::map<i32, game_meter_base*> game_meters;
        meter_data_file*                meter_data;

        // inlined @ sub_93D010
        void unregister_game_meters() {
            game_meters.clear();

            meter_data = nullptr;
        }
    };

    ASSERT_OFFSETOF(game_meter_manager, game_meters, 0x04);
    ASSERT_OFFSETOF(game_meter_manager, meter_data,  0x10);

    namespace references {
        inline util::memory_reference<game_meter_manager*> game_meter_manager { 0x010F9BD4 };
    } // references
} // treyarch
