#pragma once

#include "treyarch/mash/types.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace mash {
        class mash_info_struct;
    } // mash

    // SM3 simple_timer
    struct simple_timer {
        f32 time;

        // sub_7E9730
        simple_timer() : time(0.0f) {}

        explicit simple_timer(mash::generating_vtable_version) {}

        simple_timer &operator=(f32 t) {
            time = t;

            return *this;
        }

        // sub_7E9740
        void construct_mashed_class() {
            time = 0.0f;
        }

        void destruct_mashed_class() {}
        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}
    };
} // treyarch
