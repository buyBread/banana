#include "retail.hh"
#include "treyarch/game/cutscene/cut_scene.hh"
#include "treyarch/game/cutscene/cut_scene_player.hh"

using namespace treyarch;

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
void cut_scene_segment_unk_24::construct_mashed_class() {
    unk_00.construct_mashed_class();
    unk_0c.construct_mashed_class();
}

// inlined @ sub_806E50
void cut_scene_segment_unk_24::destruct_mashed_class() {
    unk_00.destruct_mashed_class();
    unk_0c.destruct_mashed_class();
}

// inlined @ sub_807430
void cut_scene_segment_unk_24::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    unk_00.unmash(mash_info, this, buffer);
    unk_0c.unmash(mash_info, this, buffer);
}

// inlined @ sub_8085F0
void cut_scene_segment::construct_mashed_class() {
    regions.construct_mashed_class();
    unk_24 .construct_mashed_class();

    i32 found;

    scene_anim = (void*)*retail::sub_75E130(&found, (i32)name.source_hash_code, 0x4D4E4153, nullptr); // 'SANM'
}

// sub_807340
void cut_scene_segment::destruct_mashed_class() {
    regions.destruct_mashed_class();
    unk_24 .destruct_mashed_class();

    if (unk_08)
        unk_08 = nullptr;
}

// sub_807940
void cut_scene_segment::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    regions.unmash(mash_info, this, buffer);
    unk_24 .unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        unk_08 = (u32*)mash_info->read_from_buffer(buffer, 4, 4);
    else
        unk_08 = nullptr;
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

    resource_context = (amalga::resource_pack_slot*)retail::sub_456240(); // get_resource_context
}

// sub_80A740
void cut_scene::destruct_mashed_class() {
    // cut_scene_player::stop; it stops only this one
    retail::sub_808FA0((i32*)references::cut_scene_player.read(), (i32)this);

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
