#include "retail.hh"
#include "game/frontend/igo/igo_3d_script_widget.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_70B290
igo_3d_text* igo_3d_script_widget::get_text(i32 id) {
    if (id == -2)
        return nullptr;

    if (member_texts.size()) {
        auto* found = member_texts.find(id);

        if (found != member_texts.end())
            return found->value.second;
    }

    igo_3d_text* text = nullptr;

    void* memory = mm::allocate(sizeof(igo_3d_text));

    if (memory)
        text = (igo_3d_text*)retail::sub_701610(memory, text_parameters_key.source_hash_code); // igo_3d_text::igo_3d_text

    texts.push_back(text);

    if (text)
        member_texts[id] = text;

    return text;
}
