#pragma once

#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class conversation_menu_system {
        
    public:
        void* vtable;
        u8    reserved_004[0x170];
        bool  active;

        bool is_conversation_active();
    };

    ASSERT_OFFSETOF(conversation_menu_system, active, 0x174);
} // treyarch
