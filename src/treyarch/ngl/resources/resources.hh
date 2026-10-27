#pragma once

#include "treyarch/ngl/fx/effect.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "game/fixed_string.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace resources {
    void* get_resource(fixed_string* name, u32 type);

    void resolve_texture(texture* &cache, const char* name);
    void resolve_effect(fx::effect* &cache, const char* name);

    void init();

    namespace references {
        inline util::memory_reference<u32>   default_package_size { 0x00F55004 };
        inline util::memory_reference<u8>    default_package_data { 0x00F56000 };
        inline util::memory_reference<void*> default_package_copy { 0x01118514 };
    } // references
}}} // treyarch::ngl::resources
