#pragma once

#include "treyarch/game/event/event_callback.hh"
#include "treyarch/shared/arch_base_vhandle.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class event;

    // a code callback on one entity's event; by default it counts the raises
    class generic_event_callback {

    public:
        u32 callback_id;
        u32 times_raised;

        generic_event_callback();

        // inlined @ sub_723DA0
        explicit generic_event_callback(mash::generating_vtable_version) {}

        void construct_mashed_class();
        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}

        void add   (arch_base_vhandle owner, string_hash event_type_id, code_event_callback_function function, void* parameters);
        void remove(arch_base_vhandle owner, string_hash event_type_id);

        static void count_raise(event* raised_event, arch_base_vhandle recipient, void* parameters);
    };

    ASSERT_SIZEOF  (generic_event_callback,               0x08);
    ASSERT_OFFSETOF(generic_event_callback, times_raised, 0x04);
} // treyarch
