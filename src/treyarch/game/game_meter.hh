#pragma once

#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // the base of every meter; mashed ones get their retail vtable from the type registry
    class game_meter_base : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x38];
    };

    class meter_data_file {

    public:
        void* self; // the resource handler points this at the file itself

        mash::vector
            <game_meter_base> game_meters;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (game_meter_base, 0x3C);

    ASSERT_SIZEOF  (meter_data_file,              0x18);
    ASSERT_OFFSETOF(meter_data_file, game_meters, 0x04);
} // treyarch
