#include "retail.hh"
#include "treyarch/game/frontend/igo/igo_3d_widget_parameters.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/math/references.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_6F25A0
void igo_3d_widget_parameters::construct_mashed_class() {
    self = nullptr;

    unk_04.construct_mashed_class();
    unk_10.construct_mashed_class();
    unk_44.construct_mashed_class();
    unk_50.construct_mashed_class();
    unk_5c.construct_mashed_class();
    unk_98.construct_mashed_class();
    unk_ac.construct_mashed_class();

    matrix  = (matrix4x4*)memory::heap::allocate(sizeof(matrix4x4));
    *matrix = treyarch::references::identity_matrix.get();

    retail::sub_69B8F0((i32)this); // builds matrix from the placement

    unk_94 = 0;
}

// sub_6E7D30
void igo_3d_widget_parameters::destruct_mashed_class() {
    if (matrix) {
        memory::heap::free(matrix);
        matrix = nullptr;
    }

    unk_04.destruct_mashed_class();
    unk_10.destruct_mashed_class();
    unk_44.destruct_mashed_class();
    unk_50.destruct_mashed_class();
    unk_5c.destruct_mashed_class();
    unk_98.destruct_mashed_class();
    unk_ac.destruct_mashed_class();
}

// sub_6F65A0
void igo_3d_widget_parameters::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_04.unmash(mash_info, this, buffer);
    unk_10.unmash(mash_info, this, buffer);
    unk_44.unmash(mash_info, this, buffer);
    unk_50.unmash(mash_info, this, buffer);
    unk_5c.unmash(mash_info, this, buffer);
    unk_98.unmash(mash_info, this, buffer);
    unk_ac.unmash(mash_info, this, buffer);
}
