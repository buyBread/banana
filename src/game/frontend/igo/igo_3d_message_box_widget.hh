#pragma once

#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/color.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class igo_3d_message_box_widget : public igo_3d_widget {

    public:
        // slot 55; the string is passed by value, so callers build it straight into the call
        using method_0dc_function = void (__thiscall*)(igo_3d_message_box_widget* self,
                                                       mash::string               text,
                                                       f32,
                                                       i32,
                                                       i32,
                                                       f32,
                                                       f32,
                                                       color32);

        method_0dc_function method_0dc() const {
            return (method_0dc_function)((void**)vtable)[0xDC / 4];
        }
    };
} // treyarch
