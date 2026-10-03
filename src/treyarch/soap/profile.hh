#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace soap {
    class profile;

    using profile_method = void (__thiscall*)(profile* self);
    using profile_query  = i32  (__thiscall*)(profile* self);

    struct profile_vtable {
        void*          reserved_000[5]; // subject's attach, detach, notify, the deleting destructor, initialize
        profile_method frame_advance;
        profile_query  busy;
    };

    class profile {

    public:
        profile_vtable* vtable;

        void frame_advance() {
            vtable->frame_advance(this);
        }

        i32 busy() {
            return vtable->busy(this);
        }
    };

    ASSERT_OFFSETOF(profile_vtable, frame_advance, 0x14);
    ASSERT_OFFSETOF(profile_vtable, busy,          0x18);
}} // treyarch::soap
