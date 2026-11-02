#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/moved_entities.hh"
#include "treyarch/jq/native_locks.hh"

using namespace treyarch;

// sub_796C70
void moved_entities::add_moved(arch_base_vhandle e) { // past the list's end the flags stay set and the handle is dropped
    auto* ent = (entity*)e.resolve();

    if (!ent || ent->unk_102())
        return;

    if (ent->flags & entity_flag_member) {
        ent = ent->get_conglom_owner();

        if (!ent)
            return;
    }

    if (!ent->set_flag_in_the_moved_list())
        return;

    ent->ext_flags |= entity_ext_flag_bounding_sphere_dirty;

    const i32 index = _InterlockedIncrement((volatile long*)&references::moved_count.get()) - 1;

    if (index < max_moved) {
        references::moved_list.get()[index] = ent->my_handle;

        // the answer is dropped
        ent->is_a_conglomerate();
    }

    if (ent->has_adopted_children())
        ent->dirty_adopted_children();
}

// sub_773A50
u32 moved_entities::get_moved_count() { // never past the list's last slot
    const u32 count = (u32)references::moved_count.read();

    return count < (u32)max_moved ? count : (u32)max_moved - 1;
}

// sub_7850F0
void moved_entities::remove_dead_registered_entities() { // each removal starts the scan over
    auto &registered = references::registered_entities.get();

    u32 i = 0;

    while (i < registered.size()) {
        if (registered[i].resolve())
            ++i;
        else {
            registered.erase(i);

            i = 0;
        }
    }
}

// sub_785150
void moved_entities::unregister_entity_to_update_every_frame(entity* ent) {
    engine_lock_scope lock(&jq::references::unk_01075fd0.get());

    auto &registered = references::registered_entities.get();

    u32 i = 0;

    while (i < registered.size() && !(registered[i] == ent->my_handle))
        ++i;

    if (i != registered.size())
        registered.erase(i);

    ent->flags &= ~entity_flag_update_regions_every_frame;
}

// sub_79C470
void moved_entities::add_registered_entities() {
    remove_dead_registered_entities();

    auto &registered = references::registered_entities.get();

    for (u32 i = 0; i < registered.size(); ++i)
        add_moved(registered[i]);
}
