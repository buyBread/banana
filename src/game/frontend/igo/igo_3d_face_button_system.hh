#pragma once

#include <array>

#include "game/frontend/igo/igo_3d_widget.hh"
#include "treyarch/base/tl_fixed_string.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace references {
        // the anims of the 12 button kinds, 23 slots each; filled by a crt initializer (sub_B422C0)
        inline util::memory_reference<std::array<std::array<tl_fixed_string, 23>, 12>> button_anim_names { 0x0102D040 };
    } // references

    class igo_3d_face_button_system : public igo_3d_drawable {

    public:
        void draw_buttons() {
            vtable->method_06c(this);
        }

        mash::string* get_button_anim_name(mash::string* result, u32 kind, u32 index); // builds into result; "" past the table
    };
} // treyarch
