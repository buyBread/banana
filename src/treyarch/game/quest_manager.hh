#pragma once

#include "treyarch/shared/singleton.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    class igo_3d_text;

    class quest_manager : public singleton_instance<quest_manager, 0x01087FD4> {

    public:
        u8 unk_000;

        static void draw_text();
    };

    namespace references {
        // created by sub_7F7760 during game::load_this_level
        inline util::memory_reference<igo_3d_text*> quest_text { 0x01087FCC };
    } // references
} // treyarch
