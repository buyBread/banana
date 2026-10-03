#pragma once

#include "treyarch/amalga/resource_memory_map.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/resource_key.hh"
#include "treyarch/shared/timing/limited_timer.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct resource_amalgatoc_pack_entry;
    struct resource_directory;
    struct resource_pack_slot;
    struct resource_pack_streamer;
    struct resource_partition;

    enum e_slot_state : i32 {
        slot_state_empty,
        slot_state_streaming,
        slot_state_constructing,
        slot_state_destructing,
        slot_state_ready
    };

    enum e_slot_callback : i32 {
        callback_load_started,
        callback_load_finished,
        callback_load_cancelled,
        callback_construct,
        callback_pre_destruct,
        callback_destruct,
        callback_post_destruct
    };

    using resource_pack_slot_callback = bool (__cdecl*)(e_slot_callback         reason,
                                                        resource_pack_streamer* streamer,
                                                        resource_pack_slot*     slot,
                                                        limited_timer*          time_limit);

    // resource_pack_slot's own vtable is 0x00BCA360; the handler-owning slots are worldly_pack_slot (0x00BCA37C)
    struct resource_pack_slot_vtable {
        bool                (__thiscall* on_load)(resource_pack_slot* self, limited_timer* time_limit);   // true while there's more to do
        bool                (__thiscall* on_unload)(resource_pack_slot* self, limited_timer* time_limit); // same
        resource_pack_slot* (__thiscall* destroy)(resource_pack_slot* self, u32 flags);
        void                (__thiscall* clear_slot)(resource_pack_slot* self);
        void                (__thiscall* clear_pack)(resource_pack_slot* self);
    };

    struct resource_pack_slot {
        resource_pack_slot_vtable*  vtable;
        resource_key                pack_name;
        mash::string                pack_name_str;
        e_slot_state                slot_state;
        u32                         slot_size[buffer_location_count];
        u8*                         header_mem_addr[buffer_location_count];
        resource_directory*         pack_directory;
        u32                         unk_30; // the toc entry's +0x40
        timed_progress              unload_progress;
        resource_partition*         my_partition;
        resource_pack_slot_callback my_callback;

        void frame_advance(f32 dt, limited_timer* time_limit);
        void notify_load_started(resource_amalgatoc_pack_entry* entry, resource_pack_slot_callback callback);
    };

    ASSERT_SIZEOF  (resource_pack_slot_vtable,             0x14);
    ASSERT_OFFSETOF(resource_pack_slot_vtable, clear_pack, 0x10);

    ASSERT_SIZEOF  (resource_pack_slot,                  0x44);
    ASSERT_OFFSETOF(resource_pack_slot, pack_name,       0x04);
    ASSERT_OFFSETOF(resource_pack_slot, pack_name_str,   0x0C);
    ASSERT_OFFSETOF(resource_pack_slot, slot_state,      0x18);
    ASSERT_OFFSETOF(resource_pack_slot, slot_size,       0x1C);
    ASSERT_OFFSETOF(resource_pack_slot, header_mem_addr, 0x24);
    ASSERT_OFFSETOF(resource_pack_slot, pack_directory,  0x2C);
    ASSERT_OFFSETOF(resource_pack_slot, unk_30,          0x30);
    ASSERT_OFFSETOF(resource_pack_slot, unload_progress, 0x34);
    ASSERT_OFFSETOF(resource_pack_slot, my_partition,    0x3C);
    ASSERT_OFFSETOF(resource_pack_slot, my_callback,     0x40);
}} // treyarch::amalga
