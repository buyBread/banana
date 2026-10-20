#include "treyarch/game/ise/ise_library.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_A6F5C0
void ise_library::construct_mashed_class() {
    self = nullptr;

    effects.construct_mashed_class();

    initialize(mash::FROM_MASH);
}

// sub_A6F460
void ise_library::destruct_mashed_class() {
    finalize(mash::FROM_MASH);

    effects.destruct_mashed_class();
}

// sub_A6F5F0
void ise_library::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    effects.unmash(mash_info, this, buffer);
}

// sub_A6EF00
void ise_library::initialize(mash::allocation_scope scope) {
    if (scope == mash::ALLOCATED) {
        effects.destruct_contents = true;

        return;
    }

    for (auto it = effects.begin(); it.node; it.iterate())
        it.node->element->effect->library_loaded();
}

// sub_A6EF80
void ise_library::finalize(mash::allocation_scope) {
    for (auto it = effects.begin(); it.node; it.iterate())
        it.node->element->effect->library_unloaded();
}
