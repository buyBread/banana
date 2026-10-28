#pragma once

#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    template <class guts_t, class handle_t>
    class slot_pool;

    template <class guts_t, class handle_t>
    class slot_t {

        handle_t id;
        guts_t   guts;

        handle_t occupy(const guts_t &the_guts, handle_t used_mask, handle_t ref_count_inc) {
            id   |= used_mask;
            id   += ref_count_inc;
            guts  = the_guts;

            return id;
        }

        void vacate(handle_t used_mask) {
            id &= ~used_mask;
        }

        bool get_guts(const handle_t &check_handle, guts_t* returned_guts) {
            if (check_handle != id)
                return false;

            *returned_guts = guts;

            return true;
        }

        bool in_use(handle_t used_mask) const {
            return (id & used_mask) != 0;
        }

        friend class slot_pool<guts_t, handle_t>;
    };

    // every access but refill_unused_cache holds the pool's lock
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
        engine_recursive_lock     lock;

        // sub_697B80
        void refill_unused_cache() {
            for (i32 i = 0; i < max_slots; ++i) {
                if (unused_slots_count >= unused_slots_cache_max)
                    break;

                if (!slots[i].in_use(used_mask)) {
                    unused_slots[unused_slots_count] = i;
                    unused_slots_count++;
                }
            }
        }

        // inlined @ sub_70EDB0
        handle_t get_free_slot() {
            if (!unused_slots_count)
                refill_unused_cache();

            unused_slots_count--;

            return unused_slots[unused_slots_count];
        }

    public:
        i32 remaining() const {
            return max_slots - used_slots;
        }

        // inlined @ sub_70EDB0
        handle_t add_to_pool(const guts_t &getting_into_the_pool) {
            engine_lock_scope scope(&lock);

            handle_t slot_to_use = get_free_slot();
            handle_t handle      = slots[slot_to_use].occupy(getting_into_the_pool, used_mask, ref_count_inc);

            used_slots++;

            return handle;
        }

        // inlined @ sub_6DFDA0
        void remove_from_pool(const handle_t &getting_out_of_the_pool) {
            engine_lock_scope scope(&lock);

            handle_t which_slot = getting_out_of_the_pool & slot_mask;

            slots[which_slot].vacate(used_mask);

            if (unused_slots_count < unused_slots_cache_max) {
                unused_slots[unused_slots_count] = which_slot;
                unused_slots_count++;
            }

            used_slots--;
        }

        // inlined @ sub_6C7440
        // inlined @ sub_6D2710
        // inlined @ sub_6DFDA0
        bool get_slot_contents(const handle_t &check_handle, guts_t* returned_guts) {
            engine_lock_scope scope(&lock);

            return slots[check_handle & slot_mask].get_guts(check_handle, returned_guts);
        }
    };

    using pointer_slot_pool = slot_pool<void*, u32>;
    ASSERT_SIZEOF(pointer_slot_pool, 0x50);
}
