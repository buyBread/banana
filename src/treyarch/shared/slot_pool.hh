#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    template <class guts_t, class handle_t>
    class slot_t {

        handle_t id;
        guts_t   guts;
    };

    template <class guts_t, class handle_t>
    class slot_pool {

        handle_t slot_mask;
        handle_t ref_count_inc;
        handle_t used_mask;

        enum { unused_slots_cache_max = 8 };

        slot_t<guts_t, handle_t>* slots;
        i32                       used_slots;
        i32                       max_slots;
        handle_t                  unused_slots[unused_slots_cache_max];
        i32                       unused_slots_count;
        u8                        reserved_03c[0x04];
        u32                       synchronization[3];
        u8                        reserved_04c[0x04];
    };

    using pointer_slot_pool = slot_pool<void*, u32>;
    ASSERT_SIZEOF(pointer_slot_pool, 0x50);
}
