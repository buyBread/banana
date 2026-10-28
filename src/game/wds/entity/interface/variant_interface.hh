#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "treyarch/mash/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct variant_record;

    // a conglomerate's swappable looks: named variants and their texture sets
    class variant_interface : public entity_interface {

    public:
        mash::vector<variant_record> records;
        u8                           reserved_020[0x14];
        variant_record*              applied_record;
        u8                           reserved_038[0x08];
        u32                          variant_textures;
        u32                          ifl_frames;
        u32                          min_variant_textures;
        u32                          max_variant_textures;
        u8                           reserved_050[0x02];
        bool                         randomized;
    };

    // an actor's numbered variant and the mode it's picked by
    class simple_variant_interface : public actor_interface {

    public:
        i32 simple_variant_mode;
        i32 simple_variant;
    };

    ASSERT_OFFSETOF(variant_interface, records,          0x0C);
    ASSERT_OFFSETOF(variant_interface, applied_record,   0x34);
    ASSERT_OFFSETOF(variant_interface, variant_textures, 0x40);
    ASSERT_OFFSETOF(variant_interface, randomized,       0x52);

    ASSERT_OFFSETOF(simple_variant_interface, simple_variant_mode, 0x0C);
    ASSERT_OFFSETOF(simple_variant_interface, simple_variant,      0x10);
} // treyarch
