#pragma once

#include "treyarch/amalga/resource_memory_map.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/amalga/resource_pack_streamer.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct resource_partition;

    // gets a copy of the slot list before every streamer frame advance
    using resource_partition_callback = void (__cdecl*)(resource_partition*                      partition,
                                                        dinkumware::vector<resource_pack_slot*>* pack_slots);

    struct resource_partition {
        e_partition_type                        type;
        e_resource_partition                    partition_enum;
        dinkumware::vector<resource_pack_slot*> pack_slots;
        resource_pack_streamer                  streamer;
        resource_partition_callback             callback;
        u8                                      unk_060[buffer_location_count][0x1334]; // a slot allocator per buffer

        resource_pack_slot* get_pack_slot(string_hash pack_name);
    };

    ASSERT_SIZEOF  (resource_partition,                 0x26C8);
    ASSERT_OFFSETOF(resource_partition, partition_enum, 0x0004);
    ASSERT_OFFSETOF(resource_partition, pack_slots,     0x0008);
    ASSERT_OFFSETOF(resource_partition, streamer,       0x0018);
    ASSERT_OFFSETOF(resource_partition, callback,       0x005C);
    ASSERT_OFFSETOF(resource_partition, unk_060,        0x0060);
}} // treyarch::amalga
