#pragma once

#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class static_spawn_item;
    struct persistence_data;
    struct spawn_point;

    // an active despawned with its script instance, kept for a few seconds
    struct recent_despawn {
        spawn_point*       record;
        static_spawn_item* item;
        f32                age; // seconds since the despawn
        bool               unk_0c;
        bool               unk_0d;
        u8                 pad_0e[0x02];
        string_hash        instance_name;
        persistence_data*  persistence;

        recent_despawn();
    };

    ASSERT_SIZEOF  (recent_despawn,                0x18);
    ASSERT_OFFSETOF(recent_despawn, age,           0x08);
    ASSERT_OFFSETOF(recent_despawn, unk_0c,        0x0C);
    ASSERT_OFFSETOF(recent_despawn, unk_0d,        0x0D);
    ASSERT_OFFSETOF(recent_despawn, instance_name, 0x10);
    ASSERT_OFFSETOF(recent_despawn, persistence,   0x14);
} // treyarch
