#pragma once

#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    namespace chuck { namespace vm {
        class script_instance;
    }} // chuck::vm

    class ai_core;
    struct resource_key;

    struct base_ai_data {
        u8       reserved_000[0x28];
        ai_core* core;
    };

    struct actor_unk_0b0 {
        u8                          reserved_000[0x0C];
        chuck::vm::script_instance* instance; // the actor's "__enx" instance
        u8*                         unk_010;  // its script object's name hash sits at +4
        resource_key*               unk_014;  // the script's resource; its hash is the filename
        string_hash                 unk_018;  // key prefix
    };

    class actor : public entity {

    public:
        u8             reserved_074[0x3C];
        actor_unk_0b0* unk_0b0;
        u8             reserved_0b4[0x0C];
        u32            unk_0c0; // handed to script threads as unk_60
        u8             reserved_0c4[0x0C];
        base_ai_data*  my_base_ai_data;

        ai_core* get_ai_core() const;
    };

    ASSERT_OFFSETOF(base_ai_data, core, 0x28);

    ASSERT_OFFSETOF(actor_unk_0b0, instance, 0x0C);
    ASSERT_OFFSETOF(actor_unk_0b0, unk_018,  0x18);

    ASSERT_OFFSETOF(actor, unk_0b0,         0xB0);
    ASSERT_OFFSETOF(actor, unk_0c0,         0xC0);
    ASSERT_OFFSETOF(actor, my_base_ai_data, 0xD0);
} // treyarch
