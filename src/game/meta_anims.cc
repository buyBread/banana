#include "retail.hh"
#include "game/meta_anims.hh"

using namespace treyarch;

string_hash generic_anim_by_filename::my_type_key   { 0x7677521B }; // "generic_anim_by_filename"
string_hash ai_puppet_anim_by_filename::my_type_key { 0x1D9E74A5 }; // "ai_puppet_anim_by_filename"
string_hash combat_dodge_meta_anim::my_type_key     { 0x49367782 }; // "combat_dodge_meta_anim"
string_hash combat_block_meta_anim::my_type_key     { 0x165B21AA }; // "combat_block_meta_anim"

// sub_600EB0
void generic_anim_by_filename::construct_mashed_class() {
    als_meta_anim_by_filename::construct_mashed_class();
}

// sub_490CD0
void generic_anim_by_filename::destruct_mashed_class() {
    als_meta_anim_by_filename::destruct_mashed_class();
}

// sub_490CE0
void generic_anim_by_filename::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    als_meta_anim_by_filename::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71C6A0
mash::virtual_types_key generic_anim_by_filename::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_720510
bool generic_anim_by_filename::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_meta_anim_by_filename::get_virtual_type_key() == parent_class)
        return true;

    if (als_meta_anim_by_filename::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_71C6B0
i32 generic_anim_by_filename::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_6261D0
nal_anim_id generic_anim_by_filename::get_anim_id_to_play(actor* the_actor) {
    using get_anim_id_to_play_t = nal_anim_id* (__thiscall*)(generic_anim_by_filename*, nal_anim_id*, actor*);

    nal_anim_id result;
    ((get_anim_id_to_play_t)retail::sub_6261D0)(this, &result, the_actor);

    return result;
}

// sub_600EB0
void ai_puppet_anim_by_filename::construct_mashed_class() {
    als_meta_anim_by_filename::construct_mashed_class();
}

// sub_490CD0
void ai_puppet_anim_by_filename::destruct_mashed_class() {
    als_meta_anim_by_filename::destruct_mashed_class();
}

// sub_490CE0
void ai_puppet_anim_by_filename::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    als_meta_anim_by_filename::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B9F0
mash::virtual_types_key ai_puppet_anim_by_filename::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_720510
bool ai_puppet_anim_by_filename::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_meta_anim_by_filename::get_virtual_type_key() == parent_class)
        return true;

    if (als_meta_anim_by_filename::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_71C6B0
i32 ai_puppet_anim_by_filename::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_4B31B0
nal_anim_id ai_puppet_anim_by_filename::get_anim_id_to_play(actor* the_actor) {
    using get_anim_id_to_play_t = nal_anim_id* (__thiscall*)(ai_puppet_anim_by_filename*, nal_anim_id*, actor*);

    nal_anim_id result;
    ((get_anim_id_to_play_t)retail::sub_4B31B0)(this, &result, the_actor);

    return result;
}

// sub_418F50
void combat_dodge_meta_anim::construct_mashed_class() {
    als_meta_anim_by_filename::construct_mashed_class();

    key_anims.construct_mashed_class();
}

// sub_417500
void combat_dodge_meta_anim::destruct_mashed_class() {
    key_anims.destruct_mashed_class();

    als_meta_anim_by_filename::destruct_mashed_class();
}

// sub_417CA0
void combat_dodge_meta_anim::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    als_meta_anim_by_filename::unmash(mash_info, containing_class_ptr, buffer);

    key_anims.unmash(mash_info, this, buffer);
}

// sub_720500
mash::virtual_types_key combat_dodge_meta_anim::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_720510
bool combat_dodge_meta_anim::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_meta_anim_by_filename::get_virtual_type_key() == parent_class)
        return true;

    if (als_meta_anim_by_filename::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4E1E90
i32 combat_dodge_meta_anim::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_412370
nal_anim_id combat_dodge_meta_anim::get_anim_id_to_play(actor* the_actor) {
    using get_anim_id_to_play_t = nal_anim_id* (__thiscall*)(combat_dodge_meta_anim*, nal_anim_id*, actor*);

    nal_anim_id result;
    ((get_anim_id_to_play_t)retail::sub_412370)(this, &result, the_actor);

    return result;
}

// sub_418F50
void combat_block_meta_anim::construct_mashed_class() {
    als_meta_anim_by_filename::construct_mashed_class();

    key_anims.construct_mashed_class();
}

// sub_417500
void combat_block_meta_anim::destruct_mashed_class() {
    key_anims.destruct_mashed_class();

    als_meta_anim_by_filename::destruct_mashed_class();
}

// sub_417CA0
void combat_block_meta_anim::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    als_meta_anim_by_filename::unmash(mash_info, containing_class_ptr, buffer);

    key_anims.unmash(mash_info, this, buffer);
}

// sub_720600
mash::virtual_types_key combat_block_meta_anim::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_720510
bool combat_block_meta_anim::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (als_meta_anim_by_filename::get_virtual_type_key() == parent_class)
        return true;

    if (als_meta_anim_by_filename::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4E1E90
i32 combat_block_meta_anim::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_412650
nal_anim_id combat_block_meta_anim::get_anim_id_to_play(actor* the_actor) {
    using get_anim_id_to_play_t = nal_anim_id* (__thiscall*)(combat_block_meta_anim*, nal_anim_id*, actor*);

    nal_anim_id result;
    ((get_anim_id_to_play_t)retail::sub_412650)(this, &result, the_actor);

    return result;
}
