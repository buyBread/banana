#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class movie_manager {

    public:
        void* vtable;
        i32   state;

        bool is_playing() const {
            return state == 1;
        }
    };

    ASSERT_OFFSETOF(movie_manager, state, 0x04);
} // treyarch
