#pragma once

#include "treyarch/ngl/texture/texture.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace movie {
    struct device_resource_state {
        ngl::texture* upload_targets[4];
        ngl::texture* decode_targets[8];
    };

    void release_device_resources();
    void restore_device_resources();

    namespace references {
        inline util::memory_reference<device_resource_state> device_resources { 0x0102CDDC };
    } // references

    ASSERT_SIZEOF  (device_resource_state,                 0x30);
    ASSERT_OFFSETOF(device_resource_state, upload_targets, 0x00);
    ASSERT_OFFSETOF(device_resource_state, decode_targets, 0x10);
}}} // treyarch::ngl::movie
