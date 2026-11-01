#pragma once

#include "game/frontend/igo/igo_3d_text.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // IGO3DListBox; the conversation menu's replies
    class igo_3d_list_box : public igo_3d_text {

    public:
        // slot 100
        i32 get_visible_item_count() {
            return ((i32 (__thiscall*)(igo_3d_list_box*))(*(void***)this)[100])(this);
        }

        // slot 103
        i32 get_selected_item() {
            return ((i32 (__thiscall*)(igo_3d_list_box*))(*(void***)this)[103])(this);
        }

        // slot 106
        void add_item(i32 text_id) { // a localized global text
            ((void (__thiscall*)(igo_3d_list_box*, i32))(*(void***)this)[106])(this, text_id);
        }

        // slot 108
        void add_item(const mash::string &text) { // the slot owns the copy
            ((void (__thiscall*)(igo_3d_list_box*, mash::string))(*(void***)this)[108])(this, text);
        }

        // slot 111
        void clear_items() {
            ((void (__thiscall*)(igo_3d_list_box*))(*(void***)this)[111])(this);
        }

        // slot 112
        i32 get_item_count() {
            return ((i32 (__thiscall*)(igo_3d_list_box*))(*(void***)this)[112])(this);
        }
    };
} // treyarch
