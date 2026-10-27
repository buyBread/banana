#include "retail.hh"
#include "game/als/meta_anim_swing.hh"
#include "game/als/meta_anim_instance.hh"

using namespace treyarch;

string_hash als::als_meta_anim_swing::my_type_key { 0x016ED125 }; // "als::als_meta_anim_swing"

// sub_5F4FB0
void als::als_meta_anim_swing::construct_mashed_class() {
    als_meta_anim_base::construct_mashed_class();

    keys.construct_mashed_class();

    load_key_anims();
}

// sub_5E3170
void als::als_meta_anim_swing::destruct_mashed_class() {
    keys.destruct_mashed_class();

    als_meta_anim_base::destruct_mashed_class();
}

// sub_5F6530
void als::als_meta_anim_swing::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    als_meta_anim_base::unmash(mash_info, containing_class_ptr, buffer);

    keys.unmash(mash_info, this, buffer);
}

// sub_5F4EE0
mash::virtual_types_key als::als_meta_anim_swing::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_5F4EF0
bool als::als_meta_anim_swing::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_meta_anim_base::get_virtual_type_key() == parent_class)
        return true;

    if (als_meta_anim_base::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5BAE20
i32 als::als_meta_anim_swing::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5BF040
nal::anim* als::als_meta_anim_swing::get_anim_proxy() const {
    return keys.array[0].the_anim_ptr;
}

// sub_5D8830
als::nal_als_instance_bridge als::als_meta_anim_swing::create_anim_inst(nal::transport_list*    the_skel,
                                                                        u32                     flags,
                                                                        nal::skeleton_map*      skel_map,
                                                                        u32,
                                                                        void*,
                                                                        animation_logic_system* the_als,
                                                                        state_machine*          the_layer) {

    nal_als_instance_bridge result;
    result.als_inst = new als_meta_anim_swing_instance(this, the_skel, flags, skel_map, the_als, the_layer);
    result.nal_inst = nullptr;

    return result;
}

// sub_5BF020
void als::als_meta_anim_swing::load_key_anims() {
    for (u32 index = 0; index < 8; ++index)
        keys[index].lookup_nal_animation();
}
