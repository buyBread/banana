#include "game/sound_toc.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_903A60
void sound_toc::construct_mashed_class() {
    self = nullptr;

    entries.construct_mashed_class();

    next = references::sound_tocs.read();

    references::sound_tocs.write(this);
}

// sub_903690
void sound_toc::destruct_mashed_class() {
    sound_toc* first = references::sound_tocs.read();

    if (this == first)
        references::sound_tocs.write(next);
    else if (first) {
        // the search never moves past the second toc, so a toc any further down spins here forever
        sound_toc* toc = first->next;

        while (toc) {
            if (toc == this) {
                first->next = next;

                break;
            }
        }
    }

    entries.destruct_mashed_class();
}

// sub_903E60
void sound_toc::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    entries.unmash(mash_info, this, buffer);
}
