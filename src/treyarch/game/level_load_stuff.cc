#include <cstring>

#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_partition.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/level_descriptor.hh"
#include "treyarch/shared/filespec.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/resource_key.hh"

using namespace treyarch;

// sub_97AB70
level_load_stuff::level_load_stuff() : descriptor(nullptr),
                                       name("m0_arena"),
                                       hero_name("ch_spiderman"),
                                       level_clock() {

    reset_level_load_data();
}

// sub_72C9A0
void level_load_stuff::reset_level_load_data() {
    descriptor               = nullptr;
    loading_meter_val        = 0;
    load_complete_called     = 0;
    load_this_level_finished = 0;
    load_widgets_created     = 0;
}

// sub_736FF0
void level_load_stuff::construct_loading_widgets() {
    {
        mash::string pack_name("spidermanlogo");
        mash::string other_name("spidermanlogo");

        retail::sub_83F640((const char**)amalga::resource_manager::references::unk_010f7760.read(), (i32)&pack_name, 1);
        retail::sub_87E010(amalga::resource_manager::references::unk_010f7760.read(), (i32*)&pack_name, (i32)&other_name, 1);
    }

    unk_03c              = 0;
    load_widgets_created = 1;

    retail::sub_6988C0((i32)&references::frontend.get());
}

// sub_7370B0
void level_load_stuff::destroy_loading_widgets() {
    string_hash unpause_start;
    unpause_start.initialize(mash::ALLOCATED, "UNPAUSE_START");

    retail::sub_900D60(unpause_start.source_hash_code, 0x10000000, 0);

    load_widgets_created = 0;

    // retail follows with an empty call (nullsub_1)

    mash::string pack_name("spidermanlogo");
    mash::string other_name("spidermanlogo");

    retail::sub_87E040(amalga::resource_manager::references::unk_010f7760.read(), (i32*)&pack_name, (i32)&other_name, 1);
    retail::sub_824810((u32*)amalga::resource_manager::references::unk_010f7760.read(), (i32)&pack_name, 1);
}

// sub_770030
void level_load_stuff::look_up_level_descriptor() {
    descriptor = nullptr;

    amalga::resource_pack_slot* slot =
        (*amalga::resource_manager::references::partitions.read())[amalga::resource_partition_game]->pack_slots[0];

    string_hash level_hash;
    level_hash.initialize(mash::ALLOCATED, "level");

    resource_key key;
    key.hash = level_hash;
    key.type = 1;

    level_descriptor_set* levels;
    retail::sub_7628E0((u32**)slot, (u32*)&levels, (u32*)&key, nullptr, nullptr);

    filespec spec(name);
    mash::string level_name(spec.name.to_upper());

    if (!levels)
        return;

    u32 index = 0;

    for (; index < levels->descriptors.size; ++index) {
        if (!_stricmp(level_name.c_str(), levels->descriptors.data[index]->level_name.c_str()))
            break;
    }

    if (index == levels->descriptors.size)
        return;

    descriptor = levels->descriptors.data[index];

    retail::sub_76FB40(descriptor->memory_map_index); // configure_packs_by_memory_map
}
