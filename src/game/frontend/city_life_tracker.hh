#pragma once

#include "vc8/vector.hh"
#include "game/frontend/igo/igo_map_poi.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a radar icon standing for a group of entities, tied to quest steps by name
    class city_life_tracker {

    public:
        void**                         vtable;
        vc8::vector<arch_base_vhandle> entities;
        vc8::vector<mash::string>      quest_events;          // upper-cased
        i32                            kind;
        igo_map_poi*                   mm_icon;
        bool                           in_active_quest;       // one of quest_events names a step of an active quest
        bool                           quest_events_changed;
        u8                             reserved_02e[0x02];

        vector3d     get_center_pos();
        void         add_quest_event(mash::string name);
        mash::string quest_event_at(i32 index);
    };

    ASSERT_SIZEOF  (city_life_tracker,                       0x30);
    ASSERT_OFFSETOF(city_life_tracker, entities,             0x04);
    ASSERT_OFFSETOF(city_life_tracker, quest_events,         0x14);
    ASSERT_OFFSETOF(city_life_tracker, kind,                 0x24);
    ASSERT_OFFSETOF(city_life_tracker, mm_icon,              0x28);
    ASSERT_OFFSETOF(city_life_tracker, in_active_quest,      0x2C);
    ASSERT_OFFSETOF(city_life_tracker, quest_events_changed, 0x2D);
} // treyarch
