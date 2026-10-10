#pragma once

#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/shared/timing/limited_timer.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace amalga {
    struct resource_handler {
        virtual resource_handler* destroy(u8 mode) = 0;
        virtual i32 begin(i32 operation) = 0;
        virtual i32 progress(i32                  operation,
                             resource_descriptor* descriptor,
                             limited_timer*       time_limit) = 0;

        i32                 state;
        resource_pack_slot* pack_slot;
        e_resource_type     type;
        i32                 descriptor_cursor;

        bool advance(i32 operation, limited_timer* time_limit);
    };

    ASSERT_SIZEOF  (resource_handler,                    0x14);
    ASSERT_OFFSETOF(resource_handler, state,             0x04);
    ASSERT_OFFSETOF(resource_handler, pack_slot,         0x08);
    ASSERT_OFFSETOF(resource_handler, type,              0x0C);
    ASSERT_OFFSETOF(resource_handler, descriptor_cursor, 0x10);
}} // treyarch::amalga
