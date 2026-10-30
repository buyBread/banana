#include "retail.hh"
#include "game/als/meta_linear_blend.hh"
#include "game/als/meta_anim_instance.hh"

using namespace treyarch;

string_hash als::als_meta_linear_blend::my_type_key { 0x5B6C5498 }; // "als::als_meta_linear_blend"

// sub_5F64D0
void als::als_meta_linear_blend::construct_mashed_class() {
    als_meta_anim_base::construct_mashed_class();

    key_anims.construct_mashed_class();

    load_key_anims();

    current_blend = 0.5f;
}

// sub_5EAE60
void als::als_meta_linear_blend::destruct_mashed_class() {
    key_anims.destruct_mashed_class();

    als_meta_anim_base::destruct_mashed_class();
}

// sub_5F5760
void als::als_meta_linear_blend::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    als_meta_anim_base::unmash(mash_info, containing_class_ptr, buffer);

    key_anims.unmash(mash_info, this, buffer);
}

// sub_5F6460
mash::virtual_types_key als::als_meta_linear_blend::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5F4EF0
bool als::als_meta_linear_blend::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_meta_anim_base::get_virtual_type_key() == parent_class)
        return true;

    if (als_meta_anim_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_658B50
i32 als::als_meta_linear_blend::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BEEE0
nal::anim* als::als_meta_linear_blend::get_anim_proxy() const {
    return key_anims.array[0]->the_anim_ptr;
}

// sub_5D87B0
als::nal_als_instance_bridge als::als_meta_linear_blend::create_anim_inst(nal::transport_list*    the_skel,
                                                                          u32                     flags,
                                                                          nal::skeleton_map*      skel_map,
                                                                          u32,
                                                                          void*,
                                                                          animation_logic_system* the_als,
                                                                          state_machine*          the_layer) {

    nal_als_instance_bridge result;
    result.als_inst = new als_meta_linear_blend_instance(this, the_skel, flags, skel_map, the_als, the_layer);
    result.nal_inst = nullptr;

    return result;
}

// sub_5BEDD0
void als::als_meta_linear_blend::load_key_anims() {
    for (u32 index = 0; index < key_anims.size; ++index)
        key_anims[index]->lookup_nal_animation();

    f32 anim_duration = key_anims.size ? key_anims[0]->the_anim_ptr->duration : 0.0f;

    // what's left of a stripped assert that every key anim is anim_duration long
    for (u32 index = 0; index < key_anims.size; ++index) {
        if (key_anims[index]->the_anim_ptr->duration != anim_duration)
            break;
    }
}
