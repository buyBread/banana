#include "game/app.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/game.hh"
#include "game/localized_string_table.hh"

using namespace treyarch;

// sub_6B2F40
// inlined @ sub_6B9F30
// inlined @ sub_6BA110
void igo_3d_text::set_text(u32 text_id) {
    set_text_no_localize(references::game.read()->string_localizer->lookup_localized_string((i32)text_id));
}
