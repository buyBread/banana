#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class polytube;

    // a conglomerate's tentacles, each drawn by a polytube; every list is indexed by the tentacle's number
    class tentacle_interface : public conglomerate_interface {

    public:
        class tentacle_info : public mash::mash_virtual_base {

        public:
            u8          reserved_004[0x14];
            f32         default_pct;
            f32         unk_01c;
            u8          reserved_020[0x04];
            f32         unk_024;
            f32         unk_028;
            string_hash effect;
            u8          reserved_030[0x44];
        };

        mash::vector<tentacle_info>           tentacle_infos;
        mash::vector_basic<polytube*>         polytubes;
        mash::vector_basic<arch_base_vhandle> end_targets;
        mash::vector_basic<u32>               looping_effects; // the polytube's looping effect ids, 0 for none
        u8                                    reserved_050[0x0C];

        void set_tentacle_end_target (u32 which, arch_base_vhandle target);
        void set_tentacle_default_pct(u32 which, f32 pct);
        void set_tentacle_effect     (u32 which, const string_hash &effect); // stops the running looping effect first
        u32  play_tentacle_effect    (u32 which, const string_hash &effect);
    };

    ASSERT_SIZEOF  (tentacle_interface::tentacle_info,              0x74);
    ASSERT_OFFSETOF(tentacle_interface::tentacle_info, default_pct, 0x18);
    ASSERT_OFFSETOF(tentacle_interface::tentacle_info, unk_024,     0x24);
    ASSERT_OFFSETOF(tentacle_interface::tentacle_info, effect,      0x2C);

    ASSERT_SIZEOF  (tentacle_interface,                  0x5C);
    ASSERT_OFFSETOF(tentacle_interface, tentacle_infos,  0x0C);
    ASSERT_OFFSETOF(tentacle_interface, polytubes,       0x20);
    ASSERT_OFFSETOF(tentacle_interface, end_targets,     0x30);
    ASSERT_OFFSETOF(tentacle_interface, looping_effects, 0x40);
} // treyarch
