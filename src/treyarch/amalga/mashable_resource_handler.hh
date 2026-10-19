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
        // sub_759AB0
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
        // sub_75A590
        // sub_75A640
        // sub_75A6F0
        // sub_75A7A0
        // sub_75A850
        // sub_75A900
        // sub_75A9B0
        // sub_75AA60
        // sub_75AB10
        // sub_75ABC0
        // sub_75AC70
        // sub_75AD20
        // sub_75ADD0
        // sub_75AE80
        // sub_75AF30
        // sub_75AFE0
        // sub_75B090
        // sub_75B140
        // sub_75B1F0
        // sub_75B2A0
        // sub_75B350
        // sub_75B400
        // sub_75B4B0
        // sub_75B560
        // sub_75B610
        // sub_75B6C0
        // sub_75B770
        // sub_75B820
        // sub_75B8D0
        // sub_75B980
        // sub_75BA30
        // sub_75BAE0
        // sub_75BB90
        // sub_75BC40
        // sub_75BCF0
        // sub_75BDA0
        // sub_75BE50
        // sub_75BF00
        // sub_75BFB0
        // sub_75C060
        // sub_75C110
        // sub_75C1C0
        // sub_75C270
        // sub_75C320
        // sub_75C3D0
        // sub_75C480
        // sub_75C530
        // sub_75C690
        // sub_75C740
        // sub_75C7F0
        // sub_764A90
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
        // sub_75FD30
        // sub_75FDA0
        // sub_75FE10
        // sub_75FE80
        // sub_75FEF0
        // sub_75FF60
        // sub_75FFD0
        // sub_760040
        // sub_7600B0
        // sub_760120
        // sub_760190
        // sub_760200
        // sub_760270
        // sub_7602E0
        // sub_760350
        // sub_7603C0
        // sub_760430
        // sub_7604A0
        // sub_760510
        // sub_760580
        // sub_7605F0
        // sub_760660
        // sub_7606D0
        // sub_760740
        // sub_7607B0
        // sub_760820
        // sub_760890
        // sub_760900
        // sub_760970
        // sub_7609E0
        // sub_760A50
        // sub_760AC0
        // sub_760B30
        // sub_760BA0
        // sub_760C10
        // sub_760C80
        // sub_760CF0
        // sub_760D60
        // sub_760DD0
        // sub_760E40
        // sub_760EB0
        // sub_760F20
        // sub_760F90
        // sub_761000
        // sub_761070
        // sub_7610E0
        // sub_761150
        // sub_761230
        // sub_7612A0
        // sub_761310
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

        // sub_735D10
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
