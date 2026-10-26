#pragma once

#include "treyarch/game/wds/entity/actor.hh"
#include "treyarch/shared/math/po.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class conglomerate : public actor {

    public:
        u8          reserved_0d8[0x50];
        u32         unk_128;    // its members' count
        po*         unk_12c;    // a po per member; als::biped_physics_mocomp saves and restores them
        u8          reserved_130[0x44];
        string_hash unk_174;    // the als reports it as a car's skeleton name
        u8          reserved_178[0x20];
    };

    ASSERT_SIZEOF  (conglomerate,          0x198);
    ASSERT_OFFSETOF(conglomerate, unk_128, 0x128);
    ASSERT_OFFSETOF(conglomerate, unk_12c, 0x12C);
    ASSERT_OFFSETOF(conglomerate, unk_174, 0x174);
} // treyarch
