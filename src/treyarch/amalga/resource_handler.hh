#pragma once

#include <cstddef>

#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/amalga/resource_types.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/timing/limited_timer.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct resource_pack_slot;

    // one per resource type in every worldly_pack_slot.
    // ...right, okay, we're probably looking at something else here.
    // the voodoo magic w/ `__single_inheritance` is needed to keep the pointer to `handle` 4 bytes for the redirect.
    // it's not actually doing anything important beyond that.
    struct __single_inheritance resource_handler_base : progress {
        enum e_behavior : i32 {
            load,
            unload
        };

        using handle_resource_t = i32 (__thiscall*)(resource_handler_base* self,
                                                    e_behavior             behavior,
                                                    resource_descriptor*   descriptor,
                                                    limited_timer*         time_limit);

        resource_pack_slot* pack_slot;
        e_resource_type     type;
        i32                 descriptor_cursor;

        resource_handler_base() : pack_slot(nullptr), type(e_resource_type::error) {}

        // sub_73BB60
        virtual ~resource_handler_base() {}

        // sub_689610
        virtual void pre_handle_resources(e_behavior) {}

        // 0 moves to the next descriptor, 1 yields until the next handle
        virtual i32 handle_resource(e_behavior           behavior,
                                    resource_descriptor* descriptor,
                                    limited_timer*       time_limit) = 0;

        bool handle(e_behavior behavior, limited_timer* time_limit);

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    ASSERT_SIZEOF  (resource_handler_base,                    0x14);
    ASSERT_OFFSETOF(resource_handler_base, state,             0x04);
    ASSERT_OFFSETOF(resource_handler_base, pack_slot,         0x08);
    ASSERT_OFFSETOF(resource_handler_base, type,              0x0C);
    ASSERT_OFFSETOF(resource_handler_base, descriptor_cursor, 0x10);
}} // treyarch::amalga
