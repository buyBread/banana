#include <bit>

#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/game/cutscene/cut_scene.hh"
#include "treyarch/game/cutscene/cut_scene_player.hh"

using namespace treyarch;

// sub_5AF170
u32 entity_class_entry::cutscene_animation_event::get_param_type() const {
    return param_type;
}

// sub_A6FDB0
f32 entity_class_entry::cutscene_animation_event::get_param_float() const {
    return std::bit_cast<f32>(param);
}

// sub_AD5300
i32 entity_class_entry::cutscene_animation_event::get_param_int() const {
    return (i32)param;
}

// sub_5AF180
string_hash entity_class_entry::cutscene_animation_event::get_param_hash() const {
    string_hash hash;

    hash.initialize(mash::ALLOCATED, nullptr, param);

    return hash;
}

// inlined @ sub_808510
void entity_class_entry::construct_mashed_class() {
    morph_name.construct_mashed_class();
    events    .construct_mashed_class();

    entity = arch_base_vhandle();
}

// sub_8072F0
void entity_class_entry::destruct_mashed_class() {
    morph_name.destruct_mashed_class();
    events    .destruct_mashed_class();
}

// sub_8075E0
void entity_class_entry::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    morph_name.unmash(mash_info, this, buffer);
    events    .unmash(mash_info, this, buffer);
}

// inlined @ sub_8073F0
void cut_scene_segment_attachment::construct_mashed_class() {
    name  .construct_mashed_class();
    unk_0c.construct_mashed_class();
}

// inlined @ sub_806E50
void cut_scene_segment_attachment::destruct_mashed_class() {
    name  .destruct_mashed_class();
    unk_0c.destruct_mashed_class();
}

// inlined @ sub_807430
void cut_scene_segment_attachment::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name  .unmash(mash_info, this, buffer);
    unk_0c.unmash(mash_info, this, buffer);
}

// inlined @ sub_8085F0
void cut_scene_segment::construct_mashed_class() {
    regions    .construct_mashed_class();
    attachments.construct_mashed_class();

    scene_anim = (nal::scene_anim*)amalga::resource_manager::get_merged_resource_or_default(name, nal::scene_anim_type, nullptr);
}

// sub_807340
void cut_scene_segment::destruct_mashed_class() {
    regions    .destruct_mashed_class();
    attachments.destruct_mashed_class();

    if (unk_08)
        unk_08 = nullptr;
}

// sub_807940
void cut_scene_segment::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    regions    .unmash(mash_info, this, buffer);
    attachments.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        unk_08 = (u32*)mash_info->read_from_buffer(buffer, 4, 4);
    else
        unk_08 = nullptr;
}

// sub_8054A0
bool cut_scene::has_sound() const {
    for (u32 i = 0; i < segments.size; i++) {
        string_hash no_sound;

        no_sound.initialize(mash::ALLOCATED);

        if (segments[i]->sound != no_sound)
            return true;
    }

    return false;
}

// sub_808EF0
void cut_scene::construct_mashed_class() {
    self = nullptr;

    entity_class_entries.construct_mashed_class();
    segments            .construct_mashed_class();
    unk_30              .construct_mashed_class();
    name                .construct_mashed_class();
    unk_48              .construct_mashed_class();
    hero_entry          .construct_mashed_class();

    resource_context = amalga::resource_manager::get_resource_context();
}

// sub_80A740
void cut_scene::destruct_mashed_class() {
    references::cut_scene_player.read()->stop(this);

    entity_class_entries.destruct_mashed_class();
    segments            .destruct_mashed_class();
    unk_30              .destruct_mashed_class();
    name                .destruct_mashed_class();
    unk_48              .destruct_mashed_class();
    hero_entry          .destruct_mashed_class();
}

// sub_80A810
void cut_scene::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    entity_class_entries.unmash(mash_info, this, buffer);
    segments            .unmash(mash_info, this, buffer);
    unk_30              .unmash(mash_info, this, buffer);
    name                .unmash(mash_info, this, buffer);
    unk_48              .unmash(mash_info, this, buffer);
    hero_entry          .unmash(mash_info, this, buffer);
}
