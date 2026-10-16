#include "retail.hh"
#include "treyarch/game/frontend/igo/igo_3d_text_parameters.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/math/references.hh"

using namespace treyarch;

// sub_6D0260
void igo_3d_text_parameters::parameter_set::construct_mashed_class() {
    unk_00.construct_mashed_class();
    unk_c4.construct_mashed_class();

    const vector4 &origin = math::references::unk_00e735d0.get();

    unk_d0[0]  = 1.0f;
    unk_d0[1]  = 0.0f;
    unk_d0[2]  = 0.0f;
    unk_d0[3]  = 0.0f;
    unk_d0[4]  = 1.0f;
    unk_d0[5]  = 0.0f;
    unk_d0[6]  = 0.0f;
    unk_d0[7]  = 0.0f;
    unk_d0[8]  = 1.0f;
    unk_d0[9]  = origin.x;
    unk_d0[10] = origin.y;
    unk_d0[11] = origin.z;

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
