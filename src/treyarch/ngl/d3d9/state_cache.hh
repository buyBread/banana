#pragma once

#include <d3d9.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace d3d9 {
    struct sampler_state_cache {
        DWORD address_u;
        DWORD address_v;
        DWORD address_w;
        DWORD border_color;
        DWORD mag_filter;
        DWORD min_filter;
        DWORD mip_filter;
        DWORD mip_lod_bias;
        DWORD maximum_mip_level;
        DWORD maximum_anisotropy;
        DWORD srgb_texture;
        DWORD element_index;
        DWORD displacement_map_offset;
        DWORD pad_034[3];
    };

    void initialize_sampler_filters();

    namespace references {
        inline util::memory_reference<sampler_state_cache> sampler_states { 0x011168BC };
    } // references

    ASSERT_SIZEOF  (sampler_state_cache,                          0x40);
    ASSERT_OFFSETOF(sampler_state_cache, address_u,               0x00);
    ASSERT_OFFSETOF(sampler_state_cache, mag_filter,              0x10);
    ASSERT_OFFSETOF(sampler_state_cache, min_filter,              0x14);
    ASSERT_OFFSETOF(sampler_state_cache, mip_filter,              0x18);
    ASSERT_OFFSETOF(sampler_state_cache, displacement_map_offset, 0x30);
}}} // treyarch::ngl::d3d9
