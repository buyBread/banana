#pragma once

#include <array>

#include "treyarch/amalga/apkf/file.hh"
#include "treyarch/amalga/resource_amalgatoc.hh"
#include "treyarch/amalga/resource_memory_map.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mutex.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace ngl {
        struct texture;
    } // ngl

    namespace amalga {
        struct resource_partition;

        namespace resource_manager {
            void create_inst();
            
            resource_amalgatoc* load_amalgapak();

            void* resolve_vrml_section(apkf::file*         owner,
                                       apkf::file_section* section,
                                       void*               user_data);

            namespace references {
                inline util::memory_reference<bool> initialized { 0x0102FE4C };

                inline util::memory_reference<dinkumware::vector<resource_partition*>*> partitions                   { 0x0102FE50 };
                inline util::memory_reference<engine_recursive_lock*>                   resource_context_stack_mutex { 0x0102FE54 };

                inline util::memory_reference<std::array<u8*, buffer_location_count>> resource_buffer      { 0x0102FE58 };
                inline util::memory_reference<std::array<u32, buffer_location_count>> resource_buffer_used { 0x0102FE60 };
                inline util::memory_reference<std::array<u32, buffer_location_count>> resource_buffer_size { 0x0102FE68 };

                inline util::memory_reference<resource_amalgatoc*>                amalgatoc   { 0x0102FDA4 };
                inline util::memory_reference<mash::vector<resource_memory_map>*> memory_maps { 0x0102FDB0 };

                inline util::memory_reference<i32> in_use_memory_map     { 0x00E76EE4 };
                inline util::memory_reference<i32> amalgapak_id          { 0x00E76EE8 }; // retail pc never opens it
                inline util::memory_reference<u32> amalgapak_base_offset { 0x0102FE70 };

                // what VRML sections resolve to while a pack's apkf is relocated
                inline util::memory_reference<void*> resource_context { 0x0102FE74 };

                // copies of ngl's textures, taken once at startup
                inline util::memory_reference<ngl::texture*> default_texture { 0x010300B8 };
                inline util::memory_reference<ngl::texture*> white_texture   { 0x01031D3C };
            } // references
        } // resource_manager
    } // amalga
} // treyarch
