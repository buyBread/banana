#include "retail.hh"
#include "game/als/internal.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/entity/actor.hh"
#include "game/hash/algo.hh"

using namespace treyarch;

// sub_5BB4F0
void als::request_data::correct_category_name(actor* the_actor) {
    static const string_hash default_ai_loco_category = string_hash(hash::djb2("default_ai_loco_category"));

    // the shared default stands in for whatever locomotion category the actor's ai uses
    if (!is_trans_to_category || state_name != default_ai_loco_category)
        return;

    ai_core* core = the_actor->get_ai_core();

    if (core && core->my_loco_inode) {
        string_hash category;
        state_name = *((string_hash* (__thiscall*)(loco_inode*, string_hash*))retail::sub_44EDB0)(core->my_loco_inode, &category);
    }
}

string_hash als::state::my_type_key                      { 0xCE728D15 }; // "als::state"
string_hash als::category::my_type_key                   { 0x8F24B512 }; // "als::category"
string_hash als::transition_group_base::my_type_key      { 0x1264BD65 }; // "als::transition_group_base"
string_hash als::state_machine_shared::my_type_key       { 0xC3849A9F }; // "als::state_machine_shared"
string_hash als::layer_state_machine_shared::my_type_key { 0x943C5AFB }; // "als::layer_state_machine_shared"

// inlined @ sub_5ED320
void als::state_unk_44::construct_mashed_class() {
    unk_00.construct_mashed_class();
}

// inlined @ sub_7D8DC0
void als::state_unk_44::destruct_mashed_class() {
    unk_00.destruct_mashed_class();
}

// inlined @ sub_5EC280
void als::state_unk_44::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_00.unmash(mash_info, this, buffer);
}

// inlined @ sub_5ED320
void als::state_unk_58::construct_mashed_class() {
    unk_00.construct_mashed_class();
}

// inlined @ sub_7D8DC0
void als::state_unk_58::destruct_mashed_class() {
    unk_00.destruct_mashed_class();
}

// inlined @ sub_5EC350
void als::state_unk_58::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_00.unmash(mash_info, this, buffer);
}

// sub_5ED320
void als::state::construct_mashed_class() {
    name            .construct_mashed_class();
    cat_name        .construct_mashed_class();
    events          .construct_mashed_class();
    hit_pause_events.construct_mashed_class();
    unk_44          .construct_mashed_class();
    unk_58          .construct_mashed_class();
    unk_6c          .construct_mashed_class();

    if (my_param_block)
        my_param_block->construct_mashed_class();

    if (blends)
        mash::mash_info_struct::construct_class(blends);
}

// sub_5E89C0
void als::state::destruct_mashed_class() {
    name            .destruct_mashed_class();
    cat_name        .destruct_mashed_class();
    events          .destruct_mashed_class();
    hit_pause_events.destruct_mashed_class();
    unk_44          .destruct_mashed_class();
    unk_58          .destruct_mashed_class();
    unk_6c          .destruct_mashed_class();

    if (my_param_block) {
        my_param_block->destruct_mashed_class();
        my_param_block = nullptr;
    }

    if (blends) {
        blends->destruct_mashed_class();
        blends = nullptr;
    }
}

// sub_5F55B0
void als::state::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name            .unmash(mash_info, this, buffer);
    cat_name        .unmash(mash_info, this, buffer);
    events          .unmash(mash_info, this, buffer);
    hit_pause_events.unmash(mash_info, this, buffer);
    unk_44          .unmash(mash_info, this, buffer);
    unk_58          .unmash(mash_info, this, buffer);
    unk_6c          .unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(my_param_block, this, buffer);
    else
        my_param_block = nullptr;

    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(blends, this, buffer);
    else
        blends = nullptr;
}

// sub_5EC8D0
mash::virtual_types_key als::state::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_718FB0
bool als::state::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_580E30
i32 als::state::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_5D4760
void* als::state::unk_048(bool unk, entity* the_entity) const {
    string_hash combat_id;
    string_hash group;

    // the entity's script data names them; a shielded actor's group is the mechs'
    if (the_entity && the_entity->has_ifc(entity_ifc_script_data)) {
        string_hash combat_id_name;
        combat_id_name.initialize(mash::ALLOCATED, "Combat_ID");

        string_hash value;
        value.initialize(mash::ALLOCATED, ((param_block*)((u8*)the_entity->my_ifc_storage->get_ifc(entity_ifc_script_data) + 0x10))->get_optional_pb_fixedstring(combat_id_name, "", nullptr));

        combat_id = value;

        string_hash group_name;
        group_name.initialize(mash::ALLOCATED, "hit_react_group");

        value.initialize(mash::ALLOCATED, ((param_block*)((u8*)the_entity->my_ifc_storage->get_ifc(entity_ifc_script_data) + 0x10))->get_optional_pb_fixedstring(group_name, "", nullptr));

        group = value;

        if (the_entity->is_an_actor()) {
            if (ai_core* core = ((actor*)the_entity)->get_ai_core()) {
                string_hash shield;
                shield.initialize(mash::ALLOCATED, "SYM_SHIELD_UP");

                if (core->my_param_block.get_optional_pb_int(shield, 0, nullptr)) {
                    string_hash mechs;
                    mechs.initialize(mash::ALLOCATED, "mechs");

                    group = mechs;
                }
            }
        }
    }

    void* by_group = nullptr;
    void* unnamed  = nullptr;

    for (state_unk_58* record : unk ? unk_6c : unk_58) {
        if (combat_id != string_hash() && combat_id == record->unk_00)
            return &record->unk_04;

        if (group != string_hash() && !by_group && group == record->unk_00)
            by_group = &record->unk_04;
        else if (!unnamed && record->unk_00 == string_hash())
            unnamed = &record->unk_04;
    }

    return by_group ? by_group : unnamed;
}

// sub_5EC8C0
f32 als::state::get_secondary_playback_speed(f32, f32) const {
    return 1.0f;
}

// sub_720040
f32 als::state::get_playback_speed() const {
    return 1.0f;
}

// sub_720050
f32 als::state::get_start_time() const {
    return 0.0f;
}

// inlined @ sub_5F8120
void als::category::construct_mashed_class() {
    name.construct_mashed_class();

    if (my_param_block)
        my_param_block->construct_mashed_class();
}

// inlined @ sub_5F70C0
void als::category::destruct_mashed_class() {
    name.destruct_mashed_class();

    if (my_param_block) {
        my_param_block->destruct_mashed_class();
        my_param_block = nullptr;
    }
}

// sub_5E8A50
void als::category::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(my_param_block, this, buffer);
    else
        my_param_block = nullptr;
}

// inlined @ sub_723C50
mash::virtual_types_key als::category::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// inlined @ sub_723C50
bool als::category::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// inlined @ sub_723A80
mash::virtual_types_key als::transition_group_base::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// inlined @ sub_723A80
bool als::transition_group_base::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_5ED400
void als::state_machine_shared::construct_mashed_class() {
    state_list      .construct_mashed_class();
    category_list   .construct_mashed_class();
    trans_group_list.construct_mashed_class();
}

// sub_5E8AC0
void als::state_machine_shared::destruct_mashed_class() {
    state_list      .destruct_mashed_class();
    category_list   .destruct_mashed_class();
    trans_group_list.destruct_mashed_class();
}

// sub_5F4B40
void als::state_machine_shared::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    state_list      .unmash(mash_info, this, buffer);
    category_list   .unmash(mash_info, this, buffer);
    trans_group_list.unmash(mash_info, this, buffer);
}

// sub_7203C0
mash::virtual_types_key als::state_machine_shared::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_718FB0
bool als::state_machine_shared::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_658B50
i32 als::state_machine_shared::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_A944D0
als::layer_id als::state_machine_shared::get_layer_id() const {
    return base_layer;
}

// sub_5F4BA0
void als::layer_state_machine_shared::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    state_machine_shared::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_720460
mash::virtual_types_key als::layer_state_machine_shared::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_720470
bool als::layer_state_machine_shared::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (state_machine_shared::get_virtual_type_key() == parent_class)
        return true;

    if (state_machine_shared::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4BF700
i32 als::layer_state_machine_shared::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_71E700
als::layer_id als::layer_state_machine_shared::get_layer_id() const {
    return the_layer_id;
}

// sub_5B00D0
void als::layer_state_machine_shared::set_layer_id(layer_id new_id) {
    the_layer_id = new_id;
}

// sub_5F6220
void als::animation_logic_system_shared::construct_mashed_class() {
    self = nullptr;

    the_layers_shared.construct_mashed_class();

    if (the_base_sm_shared)
        mash::mash_info_struct::construct_class(the_base_sm_shared);

    if (meta_anim_table)
        meta_anim_table->construct_mashed_class();
}

// sub_5E8B00
void als::animation_logic_system_shared::destruct_mashed_class() {
    the_layers_shared.destruct_mashed_class();

    if (the_base_sm_shared) {
        the_base_sm_shared->destruct_mashed_class();
        the_base_sm_shared = nullptr;
    }

    if (meta_anim_table) {
        meta_anim_table->destruct_mashed_class();
        meta_anim_table = nullptr;
    }
}

// sub_5F56D0
void als::animation_logic_system_shared::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    the_layers_shared.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(the_base_sm_shared, this, buffer);
    else
        the_base_sm_shared = nullptr;

    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(meta_anim_table, this, buffer);
    else
        meta_anim_table = nullptr;
}
