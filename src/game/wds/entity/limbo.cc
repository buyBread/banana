#include "retail.hh"
#include "game/amalga/worldly_pack_slot.hh"
#include "game/container/fixed_vector.hh"
#include "game/event/default_callbacks.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/limbo.hh"
#include "game/wds/references.hh"
#include "game/wds/region.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/jq/native_locks.hh"

using namespace treyarch;

// sub_7AA850
void limbo::add_limbo_entity(entity* ent) {
    engine_lock_scope lock(&jq::references::unk_01076aa8.get());

    references::g_limbo_entities.get().insert(ent->my_handle);

    ent->raise_event(treyarch::references::in_limbo.read());
}

// sub_7A4B30
void limbo::remove_limbo_entity(entity* ent) {
    engine_lock_scope lock(&jq::references::unk_01076aa8.get());

    auto &limbo_entities = references::g_limbo_entities.get();

    limbo_entities.erase(limbo_entities.find(ent->my_handle));

    ent->raise_event(treyarch::references::out_limbo.read());
}

// sub_7AA8F0
void limbo::update_limbo_entity(entity* ent) {
    if (ent && (ent->flags & entity_flag_member))
        ent = ent->get_conglom_owner();

    container::fixed_vector<region*, 15> found;

    terrain* the_terrain = treyarch::references::g_world_ptr.read()->the_terrain;

    if (ent->is_hero())
        retail::sub_789E20((i32*)the_terrain, (i32)&ent->my_abs_po->get_position(), (i32)&found); // terrain::find_regions
    else {
        vector3d center = ent->get_visual_center();

        retail::sub_789E20((i32*)the_terrain, (i32)&center, (i32)&found); // terrain::find_regions
    }

    const bool nowhere = found.empty();
    const bool vetoed  = ent->ext_flags & entity_ext_flag_unk_00001000;

    bool one_not_ready   = false;
    bool interior_loaded = false;

    for (region* r : found) {
        // the hero doesn't count a region that's locked and not forced
        if (ent->is_hero() && (r->flags & region_flag_locked) && !(r->flags & region_flag_forced))
            continue;

        if ((r->flags & region_flag_loaded) && r->slot && r->slot->slot_state == amalga::slot_state_ready) {
            if (r->flags & region_flag_interior)
                interior_loaded = true;
        } else
            one_not_ready = true;
    }

    if (nowhere || (one_not_ready && !interior_loaded) || vetoed) {
        if (!(ent->flags & entity_flag_in_limbo))
            add_limbo_entity(ent);
    } else if (ent->flags & entity_flag_in_limbo) {
        engine_lock_scope lock(&jq::references::unk_01076aa8.get());

        ent->clear_limbo();
    }
}
