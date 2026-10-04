#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace soap {
    class profile;

    using profile_initialize = void (__thiscall*)(profile* self, i32 required_users, bool flag);
    using profile_method     = void (__thiscall*)(profile* self);
    using profile_query      = i32  (__thiscall*)(profile* self);

    struct profile_vtable {
        void*              reserved_000[4]; // subject's attach, detach, notify, the deleting destructor
        profile_initialize initialize;
        profile_method     frame_advance;
        profile_query      busy;
    };

    class profile {

    public:
        profile_vtable* vtable;

        void initialize(i32 required_users, bool flag) {
            vtable->initialize(this, required_users, flag);
        }

        void frame_advance() {
            vtable->frame_advance(this);
        }

        i32 busy() {
            return vtable->busy(this);
        }
    };

    ASSERT_OFFSETOF(profile_vtable, initialize,    0x10);
    ASSERT_OFFSETOF(profile_vtable, frame_advance, 0x14);
    ASSERT_OFFSETOF(profile_vtable, busy,          0x18);
}} // treyarch::soap
