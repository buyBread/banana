#pragma once

#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace soap {
    class notification_manager;

    using notification_list             = dinkumware::vector<i32>;
    using notification_manager_get_list = notification_list* (__thiscall*)(notification_manager* self);

    struct notification_manager_vtable {
        void*                         reserved_000[2]; // initialize, frame_advance
        notification_manager_get_list notifications;
    };

    class notification_manager {

    public:
        notification_manager_vtable* vtable;

        notification_list* notifications() {
            return vtable->notifications(this);
        }
    };

    ASSERT_OFFSETOF(notification_manager_vtable, notifications, 0x08);
}} // treyarch::soap
