#pragma once

#include <array>

#include "treyarch/amalga/merged_apk/file.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/amalga/resource_amalgatoc.hh"
#include "treyarch/amalga/resource_memory_map.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace ngl {
        struct texture;
    } // ngl

    namespace amalga {
        struct resource_partition;

        // the slot, when there is one, is the current resource context for this object's lifetime
        class push_resource_context_stack_object {

            resource_pack_slot* m_context;
            bool                m_pushed;

        public:
            explicit push_resource_context_stack_object(resource_pack_slot* slot);
           ~push_resource_context_stack_object();

            push_resource_context_stack_object           (const push_resource_context_stack_object&) = delete;
            push_resource_context_stack_object &operator=(const push_resource_context_stack_object&) = delete;
        };

        // always pushes, always pops
        class resource_context_stack_object {

        public:
            explicit resource_context_stack_object(resource_pack_slot* context);
           ~resource_context_stack_object();

            resource_context_stack_object           (const resource_context_stack_object&) = delete;
            resource_context_stack_object &operator=(const resource_context_stack_object&) = delete;
        };

        namespace resource_manager {
            void create_inst();

            void frame_advance(f32 dt);
            void frame_advance(f32 dt, f32 max_ms);

            resource_amalgatoc* load_amalgapak();

            resource_partition* get_partition_pointer(resource_pack_slot* pack_slot);

            void* resolve_vrml_section(merged_apk::file*         owner,
                                       merged_apk::file_section* section,
                                       void*                     user_data);

            namespace references {
                inline util::memory_reference<bool> initialized { 0x0102FE4C };
                inline util::memory_reference<bool> advancing   { 0x0102FE4D };

                inline util::memory_reference<dinkumware::vector<resource_partition*>*> partitions                   { 0x0102FE50 };
                inline util::memory_reference<engine_recursive_lock*>                   resource_context_stack_mutex { 0x0102FE54 };

                // has no name in any build; held around every resource context push and pop
                inline util::memory_reference<engine_recursive_lock> unk_010300a8 { 0x010300A8 };

                inline util::memory_reference<std::array<u8*, buffer_location_count>> resource_buffer      { 0x0102FE58 };
                inline util::memory_reference<std::array<u32, buffer_location_count>> resource_buffer_used { 0x0102FE60 };
                inline util::memory_reference<std::array<u32, buffer_location_count>> resource_buffer_size { 0x0102FE68 };

                inline util::memory_reference<resource_amalgatoc*>                amalgatoc   { 0x0102FDA4 };
                inline util::memory_reference<mash::vector<resource_memory_map>*> memory_maps { 0x0102FDB0 };

                inline util::memory_reference<i32> in_use_memory_map     { 0x00E76EE4 };
                inline util::memory_reference<i32> amalgapak_id          { 0x00E76EE8 }; // retail pc never opens it
                inline util::memory_reference<u32> amalgapak_base_offset { 0x0102FE70 };

                // what VRML sections resolve to; the type-25 handler points it at the pack's vram buffer while relocating
                inline util::memory_reference<void*> resource_context { 0x0102FE74 };

                // frame_advance ticks it through sub_87EF10 before the partitions
                inline util::memory_reference<void*> unk_010f7760 { 0x010F7760 };

                // copies of ngl's textures, taken once at startup
                inline util::memory_reference<ngl::texture*> default_texture { 0x010300B8 };
                inline util::memory_reference<ngl::texture*> white_texture   { 0x01031D3C };
            } // references
        } // resource_manager

        ASSERT_SIZEOF(push_resource_context_stack_object, 0x08);
        ASSERT_SIZEOF(resource_context_stack_object,      0x01);
    } // amalga
} // treyarch
