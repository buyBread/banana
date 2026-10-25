#include "treyarch/game/als/animation_blend.hh"

using namespace treyarch;

string_hash als::animation_blends_list::my_type_key { 0x9371CB86 }; // "als::animation_blends_list"

// sub_5ED310
void als::animation_blends_list::construct_mashed_class() {
    blends.construct_mashed_class();
}

// sub_5E89A0
void als::animation_blends_list::destruct_mashed_class() {
    blends.destruct_mashed_class();
}

// sub_5F4B10
void als::animation_blends_list::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    blends.unmash(mash_info, this, buffer);
}

// sub_5ED2A0
mash::virtual_types_key als::animation_blends_list::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_718FB0
bool als::animation_blends_list::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_61B4B0
i32 als::animation_blends_list::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BA820
als::animation_blend* als::animation_blends_list::find_blend(const nal_anim_id &anim_name) const { // the blend for coming from that animation
    for (u32 index = 0; index < blends.size; ++index) {
        if (anim_name == blends.data[index]->anim)
            return blends.data[index];
    }

    return nullptr;
}
