#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class igo_3d_text;

    struct igo_3d_text_vtable {
        void* reserved_000[78];
        void  (__thiscall* render)(igo_3d_text* self); // sub_69D520
    };

    // only the slot the HUD text draws call is typed
    class igo_3d_text {
    public:
        igo_3d_text_vtable* vtable;

        void render() {
            vtable->render(this);
        }
    };

    ASSERT_OFFSETOF(igo_3d_text_vtable, render, 0x138);
} // treyarch
