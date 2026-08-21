#pragma once

#include "treyarch/shared/container/skip_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace resources {
    struct effect_resource {
        u32          flags;
        fixed_string name;
        i32          technique_count;
        void*        techniques;
        i32          parameter_count;
        void*        parameters;
        u32          post_load_state[10];
        void*        runtime_effect;
    };

    struct effect_resource_name {
        static string_hash get(const effect_resource* value) {
            return value->name.hash;
        }
    };

    struct effect_resource_directory :
        container::skip_list<effect_resource, effect_resource_name> {};

    void* __cdecl resolve(fixed_string* name, u32 type);

    namespace references {
        inline ::util::memory_reference<effect_resource_directory> effects { 0x011171E4 };
    } // references

    ASSERT_SIZEOF  (effect_resource,                 0x48);
    ASSERT_OFFSETOF(effect_resource, flags,          0x00);
    ASSERT_OFFSETOF(effect_resource, name,           0x04);
    ASSERT_OFFSETOF(effect_resource, technique_count, 0x0C);
    ASSERT_OFFSETOF(effect_resource, techniques,      0x10);
    ASSERT_OFFSETOF(effect_resource, parameter_count, 0x14);
    ASSERT_OFFSETOF(effect_resource, parameters,      0x18);
    ASSERT_OFFSETOF(effect_resource, runtime_effect,  0x44);

    ASSERT_SIZEOF(effect_resource_directory, 0x10);
}}} // treyarch::ngl::resources
