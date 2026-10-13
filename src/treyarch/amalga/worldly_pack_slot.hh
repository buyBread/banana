#pragma once

#include <cstddef>

#include "treyarch/amalga/resource_handler.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/amalga/resource_types.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/shared/timing/limited_timer.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    struct worldly_entity_instances {
        progress unload_progress;
        i32      unload_index;
        void*    instances;

        worldly_entity_instances() : instances(nullptr) {}

        void* operator new(std::size_t size) noexcept {
            return memory::heap::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            memory::heap::free(allocation);
        }
    };

    struct worldly_pack_slot : resource_pack_slot {
        progress                  preload_progress;
        progress                  unload_progress; // hides resource_pack_slot::unload_progress
        resource_handler_base*    resource_handlers[(u32)e_resource_type::count];
        worldly_entity_instances* entity_instances;

        worldly_pack_slot();

        void clear_slot();

        template<typename T>
        void create_resource_handler(e_resource_type type);
    };

    namespace references {
        inline util::memory_reference<amalga::resource_pack_slot_vtable> worldly_pack_slot_vtable { 0x00BCA37C };
    } // references

    ASSERT_SIZEOF  (worldly_entity_instances,               0x0C);
    ASSERT_OFFSETOF(worldly_entity_instances, unload_index, 0x04);
    ASSERT_OFFSETOF(worldly_entity_instances, instances,    0x08);

    ASSERT_SIZEOF  (worldly_pack_slot,                    0x1F0);
    ASSERT_OFFSETOF(worldly_pack_slot, preload_progress,  0x044);
    ASSERT_OFFSETOF(worldly_pack_slot, unload_progress,   0x048);
    ASSERT_OFFSETOF(worldly_pack_slot, resource_handlers, 0x04C);
    ASSERT_OFFSETOF(worldly_pack_slot, entity_instances,  0x1EC);
}} // treyarch::amalga
