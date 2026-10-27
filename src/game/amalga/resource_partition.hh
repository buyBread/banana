#pragma once

#include "game/amalga/resource_memory_map.hh"
#include "game/amalga/resource_pack_slot.hh"
#include "game/amalga/resource_pack_streamer.hh"
#include "vc8/vector.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace amalga {
    struct resource_partition;

    // gets a copy of the slot list before every streamer frame advance
    using resource_partition_callback = void (__cdecl*)(resource_partition*                      partition,
                                                        vc8::vector<resource_pack_slot*>* pack_slots);

    struct resource_partition {
        e_partition_type                 type;
        e_resource_partition             partition_enum;
        vc8::vector<resource_pack_slot*> pack_slots;
        resource_pack_streamer           streamer;
        resource_partition_callback      callback;
        u8                               unk_060[buffer_location_count][0x1334]; // a slot allocator per buffer

        resource_pack_slot* get_pack_slot(string_hash pack_name);
    };

    ASSERT_SIZEOF  (resource_partition,                 0x26C8);
    ASSERT_OFFSETOF(resource_partition, partition_enum, 0x0004);
    ASSERT_OFFSETOF(resource_partition, pack_slots,     0x0008);
    ASSERT_OFFSETOF(resource_partition, streamer,       0x0018);
    ASSERT_OFFSETOF(resource_partition, callback,       0x005C);
    ASSERT_OFFSETOF(resource_partition, unk_060,        0x0060);
}} // treyarch::amalga
