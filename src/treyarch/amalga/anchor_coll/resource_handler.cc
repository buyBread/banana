#include "retail.hh"
#include "treyarch/amalga/anchor_coll/resource_handler.hh"
#include "treyarch/amalga/mashable_resource_handler.hh"
#include "treyarch/amalga/resource_directory.hh"
#include "treyarch/amalga/resource_pack_slot.hh"
#include "treyarch/game/wds/entity/anchor_collection.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/region.hh"

using namespace treyarch;

// sub_75E4F0
i32 amalga::anchor_collection_resource_handler::handle_resource(e_behavior           behavior,
                                                                resource_descriptor* descriptor,
                                                                limited_timer*       time_limit) {

    world_dynamics_system* world = treyarch::references::g_world_ptr.read();

    engine_lock_scope lock(&world->ent_mgr.lock);

    i32 more_to_do = 1;

    if (behavior == unload) {
        anchor_collection* anchors = (anchor_collection*)descriptor->calculate_adjusted_payload();

        if (current_anchor_idx == -1)
            current_anchor_idx = 0;

        while (!(time_limit && time_limit->out_of_time()) && current_anchor_idx < (i32)anchors->anchor_positions.size)
            retail::sub_7BCD90((u32*)anchors, current_anchor_idx++); // anchor_collection::destroy_anchor

        if (current_anchor_idx == (i32)anchors->anchor_positions.size) {
            retail::sub_7DEE10((i32**)anchors); // anchor_collection::destroy_anchor_pool

            mashable_resource_handler<anchor_collection>::destruct(descriptor);

            current_anchor_idx = -1;
            more_to_do         = 0;
        }

        return more_to_do;
    }

    anchor_collection* anchors;

    if (current_anchor_idx == -1) {
        anchors = mashable_resource_handler<anchor_collection>::construct(descriptor);

        retail::sub_7DF420((u32*)anchors); // anchor_collection::create_anchor_pool

        if (world->the_terrain) {
            string_hash name = pack_slot->pack_name.hash;

            our_region = (region*)retail::sub_7A1990((u32*)world->the_terrain, (i32)name.source_hash_code);
        }

        current_anchor_idx = 0;
        all_anchors        = anchors;
    } else
        anchors = all_anchors;

    while (!(time_limit && time_limit->out_of_time()) && current_anchor_idx < (i32)anchors->anchor_positions.size) {
        retail::sub_7BCC70((u32*)anchors, current_anchor_idx, (u32*)our_region); // anchor_collection::create_anchor

        ++current_anchor_idx;
    }

    if (our_region && current_anchor_idx == (i32)anchors->anchor_positions.size)
        our_region->anchors = anchors;

    if (current_anchor_idx == (i32)anchors->anchor_positions.size) {
        current_anchor_idx = -1;
        more_to_do         = 0;
    }

    return more_to_do;
}
