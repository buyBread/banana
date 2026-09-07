#pragma once

#include "treyarch/ngl/texture/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace ngl { namespace shadow {
    struct device_resource_state {
        ngl::texture* depth_targets[2];
        ngl::texture* color_targets[2];
    };

    struct target_dimensions {
        u32 widths[2];
        u32 heights[2];
    };

    void release_device_resources();
    void restore_device_resources();

    namespace references {
        inline util::memory_reference<u8>                    initialized      { 0x01073DFB };
        inline util::memory_reference<device_resource_state> device_resources { 0x01075FC0 };
        inline util::memory_reference<target_dimensions>     dimensions       { 0x00E7AFC4 };
    } // references

    ASSERT_SIZEOF  (device_resource_state,                0x10);
    ASSERT_OFFSETOF(device_resource_state, depth_targets, 0x00);
    ASSERT_OFFSETOF(device_resource_state, color_targets, 0x08);

    ASSERT_SIZEOF  (target_dimensions,          0x10);
    ASSERT_OFFSETOF(target_dimensions, widths,  0x00);
    ASSERT_OFFSETOF(target_dimensions, heights, 0x08);
}}} // treyarch::ngl::shadow
