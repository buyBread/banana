#pragma once

#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
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
        u8                           reserved_004[0x04];

        static notification_manager* inst();

        notification_list* notifications() {
            return vtable->notifications(this);
        }
    };

    namespace references {
        inline util::memory_reference<notification_manager*> notification_manager { 0x01123D14 };
    } // references

    ASSERT_OFFSETOF(notification_manager_vtable, notifications, 0x08);

    ASSERT_SIZEOF(notification_manager, 0x08);
}} // treyarch::soap
