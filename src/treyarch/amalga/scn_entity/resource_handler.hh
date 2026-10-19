#pragma once

#include "treyarch/amalga/resource_handler.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    // unmashes a slot's scene entities a few at a time, across as many calls as the time limit needs
    struct scene_entity_container_resource_handler : resource_handler_base {
        i32                     entity_index; // the next one to unmash
        mash::mash_info_struct* mash_info;    // kept on the heap until the last one

        // sub_72E330
        void pre_handle_resources(e_behavior) override {
            entity_index = 0;
            mash_info    = nullptr;
        }

        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*       time_limit) override;
    };

    ASSERT_SIZEOF  (scene_entity_container_resource_handler,               0x1C);
    ASSERT_OFFSETOF(scene_entity_container_resource_handler, entity_index, 0x14);
    ASSERT_OFFSETOF(scene_entity_container_resource_handler, mash_info,    0x18);
}} // treyarch::amalga
