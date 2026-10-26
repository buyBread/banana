#pragma once

#include "vc8/vector.hh"
#include "treyarch/game/wds/entity/interface/generic_interface.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class morph_interface : public entity_interface {

    public:
        // switches to the morph unless it's already the one; a null key clears it
        bool set_morph(resource_key morph);

        static void frame_advance_all_morph_ifcs(f32 t);
    };

    namespace references {
        // VC8 vector object (first at 0x010034B0);
        // registration and removal (sub_641D30);
        // lock 0x01003630
        inline util::memory_reference<vc8::vector<morph_interface*>> all_morph_ifcs { 0x010034AC };
    } // references
} // treyarch
