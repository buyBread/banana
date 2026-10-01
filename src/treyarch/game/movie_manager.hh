#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    class movie_manager {

    public:
        void* vtable;
        i32   state;
        i32   unk_008;
        void* bink;

        bool is_playing() const {
            return state == 1;
        }

        void release_device_resources();
        void restore_device_resources();
    };

    ASSERT_OFFSETOF(movie_manager, state, 0x04);
    ASSERT_OFFSETOF(movie_manager, bink,  0x0C);

    namespace references {
        inline util::memory_reference<movie_manager*> movie_manager { 0x0102F2DC };
    } // references
} // treyarch
