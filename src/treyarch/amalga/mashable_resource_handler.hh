#pragma once

#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/amalga/resource_handler.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    // the resource is unmashed in place, inside the pack's own allocation
    template<typename T>
    struct mashable_resource_handler : resource_handler_base {
        // sub_759C40
        // sub_759D50
        // sub_759E00
        // sub_759EB0
        // sub_759F60
        // sub_75A010
        // sub_75A0C0
        // sub_75A170
        // sub_75A220
        // sub_75A2D0
        // sub_75A380
        // sub_75A430
        // sub_75A4E0
        // sub_764B60
        // sub_764C30
        // sub_769390
        // sub_76B750
        // sub_76D640
        static T* construct(resource_descriptor* descriptor) {
            mash::mash_info_struct mash_info(mash::UNMASH_MODE,
                                             descriptor->raw_payload,
                                             (i32)descriptor->payload_size,
                                             true);

            mash_info.get_header()->in_use = 1;

            T* resource = nullptr;

            mash_info.unmash_class(resource, nullptr, mash::NORMAL_BUFFER);
            mash::mash_info_struct::construct_class(resource);

            resource->self = descriptor->get_adjusted_payload();

            return resource;
        }

        // sub_75F780
        // sub_75F7F0
        // sub_75F860
        // sub_75F8D0
        // sub_75F940
        // sub_75F9B0
        // sub_75FA20
        // sub_75FA90
        // sub_75FB00
        // sub_75FB70
        // sub_75FBE0
        // sub_75FC50
        // sub_75FCC0
        // sub_766F80
        // sub_766FB0
        // sub_76B620
        // sub_76D4F0
        // sub_76E230
        i32 handle_resource(e_behavior           behavior,
                            resource_descriptor* descriptor,
                            limited_timer*) override {

            if (behavior == load)
                construct(descriptor);
            else
                destruct(descriptor);

            return 0;
        }

        // sub_749FF0
        // sub_759CF0
        // sub_762560
        // sub_7675E0
        static void destruct(resource_descriptor* descriptor) {
            ((mash::mash_info_struct::mash_header*)descriptor->raw_payload)->in_use = 0;

            ((T*)descriptor->calculate_adjusted_payload())->destruct_mashed_class();
        }
    };
}} // treyarch::amalga
