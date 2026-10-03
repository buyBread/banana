#pragma once

#include "treyarch/shared/mash/mash_info.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    // index order of the partition name table at 0x00E76EEC
    enum e_resource_partition : u32 {
        resource_partition_game,
        resource_partition_hero,
        resource_partition_language,
        resource_partition_mission,
        resource_partition_shenani,
        resource_partition_retrieval,
        resource_partition_common,
        resource_partition_district,
        resource_partition_voice,
        resource_partition_act,
        resource_partition_guest,
        resource_partition_sky,
        resource_partition_count
    };

    enum e_partition_type : u32 {
        partition_type_buffer,
        partition_type_preserved,
        partition_type_stack
    };

    // one per buffer location
    enum e_buffer_location : u32 {
        buffer_location_main,
        buffer_location_vram,
        buffer_location_count
    };

    struct resource_memory_map_partition {
        e_resource_partition partition_enum;
        e_partition_type     partition_type;
        u32                  slot_size[buffer_location_count];
        u32                  number_of_slots;
    };

    struct resource_memory_map {
        static constexpr u32 map_name_str_length = 16;

        char                           map_name_str[map_name_str_length];
        resource_memory_map_partition* memory_map_partitions; // the image keeps the partitions right after the map

        void construct_mashed_class() {}

        // inlined @ sub_7695D0
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            memory_map_partitions = (resource_memory_map_partition*)mash_info
                ->read_from_buffer(buffer,
                                   sizeof(resource_memory_map_partition) * resource_partition_count,
                                   4);
        }
    };

    ASSERT_SIZEOF  (resource_memory_map_partition,                  0x14);
    ASSERT_OFFSETOF(resource_memory_map_partition, partition_type,  0x04);
    ASSERT_OFFSETOF(resource_memory_map_partition, slot_size,       0x08);
    ASSERT_OFFSETOF(resource_memory_map_partition, number_of_slots, 0x10);

    ASSERT_SIZEOF  (resource_memory_map,                        0x14);
    ASSERT_OFFSETOF(resource_memory_map, memory_map_partitions, 0x10);
}} // treyarch::amalga
