#include "retail.hh"
#include "game/frontend/igo/igo_3d_text_parameters.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/math/matrix_diag.hh"

using namespace treyarch;

// sub_6D0260
void igo_3d_text_parameters::parameter_set::construct_mashed_class() {
    unk_00.construct_mashed_class();
    unk_c4.construct_mashed_class();

    unk_d0 = math::identity_mat33();

    retail::sub_69F270((i32)this); // builds unk_d0 from the placement

    unk_100 = 0;
}

// sub_6F8FC0
void igo_3d_text_parameters::construct_mashed_class() {
    self = nullptr;

    parameters.construct_mashed_class();
}

// sub_6DE140
void igo_3d_text_parameters::destruct_mashed_class() {
    parameters.destruct_mashed_class();
}

// sub_6F6E00
void igo_3d_text_parameters::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    parameters.unmash(mash_info, this, buffer);
}
