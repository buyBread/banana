#pragma once

#include "util/memory_reference.hh"

namespace treyarch {
    class igo_3d_text;

    class quest_manager {

    public:
        u8 unk_000;

        static void draw_text();
    };

    namespace references {
        inline util::memory_reference<quest_manager*> quest_manager { 0x01087FD4 };

        // created by sub_7F7760 during game::load_this_level
        inline util::memory_reference<igo_3d_text*> quest_text { 0x01087FCC };
    } // references
} // treyarch
