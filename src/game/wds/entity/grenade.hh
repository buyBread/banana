#pragma once

#include "game/wds/entity/actor.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class grenade : public actor {

    public:
        u8 reserved_0d8[0x10];

        // slot 166
        void redirect(arch_base_vhandle target, entity* redirector) {
            ((void (__thiscall*)(grenade*, arch_base_vhandle, entity*))vtable[166])(this, target, redirector);
        }

        // slot 167
        void redirect(const vector3d &dir, entity* redirector) {
            ((void (__thiscall*)(grenade*, const vector3d*, entity*))vtable[167])(this, &dir, redirector);
        }

        // slot 168
        void disable_guidance() {
            ((void (__thiscall*)(grenade*))vtable[168])(this);
        }
    };

    ASSERT_SIZEOF(grenade, 0xE8);
} // treyarch
