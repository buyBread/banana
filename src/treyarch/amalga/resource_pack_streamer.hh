#pragma once

#include "treyarch/amalga/resource_memory_map.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/shared/dinkumware/list.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/resource_key.hh"
#include "treyarch/shared/timing/limited_timer.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct resource_amalgatoc_pack_entry;
    struct resource_partition;

    struct resource_pack_queue_entry {
        char                        name[32]; // a 32-character name leaves it unterminated
        i32                         slot_idx;
        resource_pack_slot_callback callback;
    };

    using resource_pack_queue = dinkumware::list<resource_pack_queue_entry>;

    struct resource_pack_streamer {
        using flush_callback = void (__cdecl*)();

        bool                            active;
        u8                              pad_01[3];
        i32                             currently_streaming;
        dinkumware::vector
            <resource_pack_slot*>*      pack_slots;
        string_hash                     curr_pack_name;
        resource_pack_slot*             curr_slot;
        i32                             curr_slot_idx;
        resource_amalgatoc_pack_entry*  curr_pack_entry;
        resource_partition*             my_partition;
        dinkumware::list
            <resource_pack_queue_entry> load_queue;
        u8*                             aram_buffer;   // only ever cleared
        resource_pack_slot_callback     load_callback; // handed to every queued load
        f32                             load_timer;
        i32                             curr_stream_request_id[buffer_location_count];
        i32                             curr_file_id;

        static void stream_request_callback(i32 state, i32 request, void* user);

        void frame_advance(f32 dt, limited_timer* time_limit);
        void frame_advance_idle(f32 dt);
        void frame_advance_streaming(f32 dt);
        void finish_streaming();

        void flush(flush_callback callback, f32 callback_interval);

        void load(const char* pack_name, i32 slot_idx);
        void load_internal(const char* pack_name, i32 slot_idx, resource_pack_slot_callback callback);
        void finish_data_read();

        resource_pack_slot* find_loaded_pack(const resource_key &pack_name, resource_partition* partition_to_search);

        bool all_slots_idle() const;
    };

    ASSERT_SIZEOF  (resource_pack_queue_entry,           0x28);
    ASSERT_OFFSETOF(resource_pack_queue_entry, slot_idx, 0x20);
    ASSERT_OFFSETOF(resource_pack_queue_entry, callback, 0x24);

    ASSERT_SIZEOF  (resource_pack_streamer,                         0x44);
    ASSERT_OFFSETOF(resource_pack_streamer, currently_streaming,    0x04);
    ASSERT_OFFSETOF(resource_pack_streamer, pack_slots,             0x08);
    ASSERT_OFFSETOF(resource_pack_streamer, curr_pack_name,         0x0C);
    ASSERT_OFFSETOF(resource_pack_streamer, curr_slot,              0x10);
    ASSERT_OFFSETOF(resource_pack_streamer, curr_slot_idx,          0x14);
    ASSERT_OFFSETOF(resource_pack_streamer, curr_pack_entry,        0x18);
    ASSERT_OFFSETOF(resource_pack_streamer, my_partition,           0x1C);
    ASSERT_OFFSETOF(resource_pack_streamer, load_queue,             0x20);
    ASSERT_OFFSETOF(resource_pack_streamer, aram_buffer,            0x2C);
    ASSERT_OFFSETOF(resource_pack_streamer, load_callback,          0x30);
    ASSERT_OFFSETOF(resource_pack_streamer, load_timer,             0x34);
    ASSERT_OFFSETOF(resource_pack_streamer, curr_stream_request_id, 0x38);
    ASSERT_OFFSETOF(resource_pack_streamer, curr_file_id,           0x40);
}} // treyarch::amalga
