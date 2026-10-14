#include "treyarch/game/tilemap.hh"
#include "treyarch/shared/mash/mash_info.hh"

using namespace treyarch;

// sub_7DAC80
void tilemap::construct_mashed_class() {
    self = nullptr;

    for (reference<ngl::texture> &texture : unk_04)
        texture.construct_mashed_class();

    unk_34.construct_mashed_class();

    for (reference<ngl::texture> &texture : unk_4c)
        texture.construct_mashed_class();

    unk_7c.construct_mashed_class();

    for (reference<ngl::texture> &texture : unk_94)
        texture.construct_mashed_class();
}

// sub_7C2F20
void tilemap::destruct_mashed_class() {
    for (reference<ngl::texture> &texture : unk_04)
        texture.destruct_mashed_class();

    unk_34.destruct_mashed_class();

    for (reference<ngl::texture> &texture : unk_4c)
        texture.destruct_mashed_class();

    unk_7c.destruct_mashed_class();

    for (reference<ngl::texture> &texture : unk_94)
        texture.destruct_mashed_class();
}

// sub_7D92A0
void tilemap::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    for (reference<ngl::texture> &texture : unk_04)
        texture.unmash(mash_info, this, buffer);

    unk_34.unmash(mash_info, this, buffer);

    for (reference<ngl::texture> &texture : unk_4c)
        texture.unmash(mash_info, this, buffer);

    unk_7c.unmash(mash_info, this, buffer);

    for (reference<ngl::texture> &texture : unk_94)
        texture.unmash(mash_info, this, buffer);
}
