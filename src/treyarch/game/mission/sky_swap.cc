#include <cstdio>
#include <cstring>

#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_memory_map.hh"
#include "treyarch/amalga/resource_partition.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/ngl/fx/references.hh"
#include "treyarch/ngl/texture/texture.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_97E820
mash::string mission_manager::get_sky_name() const {
    i32 hour = (i32)(game_time / 60) / 60 % 24;

    if (current_act == 2) {
        if (hour >= 6 && hour <= 11)
            return mash::string("Act3_dawn");

        if (hour >= 12 && hour <= 17)
            return mash::string("Act3_noon");

        if (hour >= 18 && hour <= 23)
            return mash::string("Act3_sunset");

        return mash::string("Act3_midnight");
    }

    if (hour >= 6 && hour <= 11)
        return mash::string("Act1_dawn");

    if (hour >= 12 && hour <= 17)
        return mash::string("Act1_noon");

    if (hour >= 18 && hour <= 23)
        return mash::string("Act1_sunset");

    return mash::string("Act1_midnight");
}

// sub_980880
void mission_manager::check_sky_name() {
    // an empty sky name turns the swap off
    if (!std::strncmp(sky_name.c_str(), "", mash::string::max_length))
        return;

    if (!std::strncmp(get_sky_name().c_str(), sky_name.c_str(), mash::string::max_length))
        return;

    wanted_sky_name = get_sky_name();
    sky_swap_state  = 1;
}

// sub_97E610
void mission_manager::load_sky_pack() {
    sky_name = wanted_sky_name;

    const char* level     = &references::level_name_buffer.get();
    const char* separator = "_";

    if (!_stricmp(level, "megacity")) {
        level     = "";
        separator = "";
    }

    char pack_name[256];
    std::sprintf(pack_name, "%s%s%s", level, separator, sky_name.c_str());

    string_hash pack_hash;
    pack_hash.initialize(mash::ALLOCATED, pack_name);

    if (!retail::sub_73A200((u32*)amalga::resource_manager::references::amalgatoc.read(), pack_hash.source_hash_code)) {
        sky_swap_state = 0;

        return;
    }

    const char* loaded = (const char*)retail::sub_744E30();

    if (sky_force_reload || !loaded || _stricmp(loaded, pack_name)) {
        ngl::fx::references::environment_texture.write(ngl::references::black_texture.read());

        retail::sub_76FAA0((u32*)references::game.read());
        retail::sub_76F660(pack_name);
    }

    sky_swap_state = 3;
}

// sub_980950
void mission_manager::apply_sky_pack() {
    auto* partition = (amalga::resource_partition*)retail::sub_739C40(amalga::resource_partition_sky);

    if (partition->pack_slots[0]->slot_state != amalga::slot_state_ready)
        return;

    amalga::push_resource_context_stack_object context(
        (amalga::resource_pack_slot*)retail::sub_74D900(amalga::resource_partition_sky));

    fixed_string texture_name;

    texture_name.hash = string_hash(hash::djb2(sky_name.c_str()));
    texture_name.set_text(sky_name.c_str());

    ngl::fx::references::environment_texture.write((ngl::texture*)retail::sub_9E45A0(&texture_name.text));

    if (texture_name.text)
        memory::heap::free(texture_name.text);

    sky_swap_state = 0;
}
