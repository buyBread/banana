#include "treyarch/game/als/internal.hh"

using namespace treyarch;

string_hash als::state::my_type_key { 0xCE728D15 }; // "als::state"

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
