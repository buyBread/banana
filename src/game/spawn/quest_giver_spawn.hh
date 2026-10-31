#pragma once

#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class entity;
    class quest_giver_location_item;
    struct quest_giver;

    // where one quest giver stands; built by retail, hung on the giver and on the spawn tables
    struct quest_giver_spawn {
        quest_giver_location_item* location; // the last of the giver's locations whose condition holds
        bool                       enabled;
        u8                         pad_05[0x03];
        entity*                    spawned_entity;
        quest_giver*               giver;
        vector3d                   position;

        vector3d get_position();
        vector3d get_facing();
        vector3d get_marker_offset();

        void reselect_location();
    };

    ASSERT_SIZEOF  (quest_giver_spawn,                0x1C);
    ASSERT_OFFSETOF(quest_giver_spawn, enabled,        0x04);
    ASSERT_OFFSETOF(quest_giver_spawn, spawned_entity, 0x08);
    ASSERT_OFFSETOF(quest_giver_spawn, giver,          0x0C);
    ASSERT_OFFSETOF(quest_giver_spawn, position,       0x10);
} // treyarch
