#pragma once

#include "game/amalga/resource_handler.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class anchor_collection;
    struct region;
} // treyarch

namespace treyarch { namespace amalga {
    // creates or destroys a slot's anchors a few at a time, across as many calls as the time limit needs
    struct anchor_collection_resource_handler : resource_handler_base {
        i32                current_anchor_idx; // -1 between collections
        region*            our_region;
        anchor_collection* all_anchors;

        // sub_72E340
        void pre_handle_resources(e_behavior) override {
            current_anchor_idx = -1;
            our_region         = nullptr;
        }

        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*       time_limit) override;
    };

    ASSERT_SIZEOF  (anchor_collection_resource_handler,                     0x20);
    ASSERT_OFFSETOF(anchor_collection_resource_handler, current_anchor_idx, 0x14);
    ASSERT_OFFSETOF(anchor_collection_resource_handler, our_region,         0x18);
    ASSERT_OFFSETOF(anchor_collection_resource_handler, all_anchors,        0x1C);
}} // treyarch::amalga
