#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct igo_generic_input_listener_vtable {
        void* reserved_000[24];
    };

    class igo_generic_input_listener {
    public:
        igo_generic_input_listener_vtable* vtable;
        u32                                unk_04;
        u32                                unk_08;
    };

    ASSERT_SIZEOF(igo_generic_input_listener_vtable, 24 * 4);
    ASSERT_SIZEOF(igo_generic_input_listener,        0x0C);
} // treyarch
