#pragma once

#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/amalga/resource_handler.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/types.hh"
#include "util/types.hh"

namespace treyarch { namespace amalga {
    // the resource is unmashed in place, inside the pack's own allocation.
    // there's no begin step.
    template<typename T>
    struct mashable_resource_handler : resource_handler {
        // sub_759EB0
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

        // sub_75F8D0
        i32 progress_mashable(i32                  operation,
                              resource_descriptor* descriptor) {

            if (!operation) {
                construct(descriptor);

                return 0;
            }

            ((mash::mash_info_struct::mash_header*)descriptor->raw_payload)->in_use = 0;

            ((T*)descriptor->calculate_adjusted_payload())->destruct_mashed_class();

            return 0;
        }
    };
}} // treyarch::amalga
