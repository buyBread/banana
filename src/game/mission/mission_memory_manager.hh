#pragma once

#include "treyarch/mash/string.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // created by sub_428F00, constructor sub_8818A0
    class mission_memory_manager : public singleton<mission_memory_manager, 0x010F7760> {

    public:
        mash::string unk_004;
        i32          packs_unloading;
        u32          unk_014;
        u8           reserved_018[0x54];
    };

    ASSERT_SIZEOF  (mission_memory_manager,                  0x6C);
    ASSERT_OFFSETOF(mission_memory_manager, packs_unloading, 0x10);
} // treyarch
