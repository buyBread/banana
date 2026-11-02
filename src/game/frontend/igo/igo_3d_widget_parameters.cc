#include "retail.hh"
#include "game/frontend/igo/igo_3d_widget_parameters.hh"
#include "treyarch/mash/mash_info.hh"
#include "game/math/references.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_6F25A0
void igo_3d_widget_parameters::construct_mashed_class() {
    self = nullptr;

    name           .construct_mashed_class();
    entity_name    .construct_mashed_class();
    spawn_anim     .construct_mashed_class();
    anim_50        .construct_mashed_class();
    anim_5c        .construct_mashed_class();
    text_children  .construct_mashed_class();
    widget_children.construct_mashed_class();

    matrix  = (matrix4x4*)mm::allocate(sizeof(matrix4x4));
    *matrix = treyarch::references::identity_matrix.get();

    update_transform_matrix();

    unk_94 = 0;
}

// sub_6E7D30
void igo_3d_widget_parameters::destruct_mashed_class() {
    if (matrix) {
        mm::free(matrix);
        matrix = nullptr;
    }

    name           .destruct_mashed_class();
    entity_name    .destruct_mashed_class();
    spawn_anim     .destruct_mashed_class();
    anim_50        .destruct_mashed_class();
    anim_5c        .destruct_mashed_class();
    text_children  .destruct_mashed_class();
    widget_children.destruct_mashed_class();
}

// sub_6F65A0
void igo_3d_widget_parameters::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name           .unmash(mash_info, this, buffer);
    entity_name    .unmash(mash_info, this, buffer);
    spawn_anim     .unmash(mash_info, this, buffer);
    anim_50        .unmash(mash_info, this, buffer);
    anim_5c        .unmash(mash_info, this, buffer);
    text_children  .unmash(mash_info, this, buffer);
    widget_children.unmash(mash_info, this, buffer);
}

void igo_3d_widget_parameters::update_transform_matrix() {
    retail::sub_69B8F0((i32)this); // builds matrix from the placement
}
