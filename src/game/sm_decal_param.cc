#include "game/sm_decal_param.hh"
#include "treyarch/mash/mash_info.hh"

using namespace treyarch;

// sub_7BC4D0
void sm_decal_param_layer::destruct_mashed_class() {
    diffuse_map.destruct_mashed_class();
    normal_map .destruct_mashed_class();

    if (uv_animation) {
        uv_animation->destruct_mashed_class();
        uv_animation = nullptr;
    }
}

// sub_7D99A0
void sm_decal_param_layer::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    diffuse_map.unmash(mash_info, this, buffer);
    normal_map .unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(uv_animation, this, buffer);
    else
        uv_animation = nullptr;
}

// sub_7D9A10
void sm_decal_param::construct_mashed_class() {
    self = nullptr;

    diffuse        .construct_mashed_class();
    decal          .construct_mashed_class();
    environment_map.construct_mashed_class();
}

// sub_7CB1C0
void sm_decal_param::destruct_mashed_class() {
    diffuse        .destruct_mashed_class();
    decal          .destruct_mashed_class();
    environment_map.destruct_mashed_class();
}

// sub_7DB430
void sm_decal_param::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    diffuse        .unmash(mash_info, this, buffer);
    decal          .unmash(mash_info, this, buffer);
    environment_map.unmash(mash_info, this, buffer);
}
