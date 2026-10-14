#include "retail.hh"
#include "treyarch/game/vehicle_model.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_596D00
void vehicle_model_spec::construct_mashed_class() {
    unk_04.construct_mashed_class();
    unk_18.construct_mashed_class();
    unk_28.construct_mashed_class();
}

// sub_595650
void vehicle_model_spec::destruct_mashed_class() {
    unk_04.destruct_mashed_class();
    unk_18.destruct_mashed_class();
    unk_28.destruct_mashed_class();
}

// sub_598230
void vehicle_model_spec::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_04.unmash(mash_info, this, buffer);
    unk_18.unmash(mash_info, this, buffer);
    unk_28.unmash(mash_info, this, buffer);
}

// inlined @ sub_5A6350
void vehicle_model::construct_mashed_class() {
    extra_member_ids.construct_mashed_class();
    unk_28          .construct_mashed_class();

    retail::sub_59EE40((u32*)this); // initialize(mash::FROM_MASH)
}

// sub_5982A0
void vehicle_model::destruct_mashed_class() {
    retail::sub_596D50((u32*)this, mash::FROM_MASH); // finalize

    extra_member_ids.destruct_mashed_class();
    unk_28          .destruct_mashed_class();
}

// inlined @ sub_596030
void vehicle_model::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    extra_member_ids.unmash(mash_info, this, buffer);
    unk_28          .unmash(mash_info, this, buffer);
}

// inlined @ sub_597A50
void vehicle_model_group::construct_mashed_class() {
    unk_04.construct_mashed_class();
    unk_14.construct_mashed_class();
}

// sub_58EEE0
void vehicle_model_group::destruct_mashed_class() {
    unk_04.destruct_mashed_class();
    unk_14.destruct_mashed_class();
}

// inlined @ sub_594160
void vehicle_model_group::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_04.unmash(mash_info, this, buffer);
    unk_14.unmash(mash_info, this, buffer);
}

// sub_5A67E0
void vehicle_model_depot::construct_mashed_class() {
    self = nullptr;

    specs .construct_mashed_class();
    models.construct_mashed_class();
    groups.construct_mashed_class();
}

// sub_5A4CC0
void vehicle_model_depot::destruct_mashed_class() {
    specs .destruct_mashed_class();
    models.destruct_mashed_class();
    groups.destruct_mashed_class();
}

// sub_5A4D00
void vehicle_model_depot::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    specs .unmash(mash_info, this, buffer);
    models.unmash(mash_info, this, buffer);
    groups.unmash(mash_info, this, buffer);
}
