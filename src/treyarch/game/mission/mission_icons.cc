#include "retail.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_983F30
void mission_manager::update_eligible_missions() {
    if (state == mission_manager_state_initial_startup)
        return;

    flags &= ~mission_manager_flag_unk_00200000;

    if (!gen_global_exec || !references::g_world_ptr.read()->hero_ptr)
        return;

    i32 unk_value = -1;

    if (entity* hero = references::g_world_ptr.read()->hero_ptr) {
        player_interface* player = hero->player_ifc();

        unk_value = ((i32 (__thiscall*)(player_interface*))((void**)player->vtable)[0x130 / 4])(player); // slot 76
    }

    eligible_mission_instances->clear();

    eligible_mission_visitor visitor;
    visitor.vtable        = &references::eligible_mission_visitor_vtable.get();
    visitor.unk_004       = 0;
    visitor.hero_position = references::g_world_ptr.read()->hero_ptr->my_abs_po->get_position();
    visitor.unk_014       = unk_value;

    // fills eligible_mission_instances
    retail::sub_A1FCF0((u32*)gen_global_exec, (i32)&visitor);

    i32 count     = (i32)eligible_mission_instances->size();
    i32 processed = 0;

    for (i32 index = 0; index < count; ++index) {
        const chuck::vm::script_instance* instance = (*eligible_mission_instances)[index];

        // the rest waits for the next pass
        if (processed > 32) {
            flags |= mission_manager_flag_unk_00200000;

            return;
        }

        auto* header = (mission_header_instance_base*)instance->data.buffer;

        if (header->flags & mission_header_flag_unk_00020000)
            continue;

        ++processed;

        // raises a chuck_sle_MISSION_ELIGIBLE_t
        ((void (__thiscall*)(mission_manager*, string_hash, f32))retail::sub_97F710)(this, instance->name, header->unk_01c);

        if (header->flags & mission_header_flag_unk_00004000) {
            header->flags |= mission_header_flag_unk_00020000;

            continue;
        }

        if (header->flags & mission_header_flag_eligible_required)
            header->child_eligibility_id = unk_20c++;

        if (!(header->flags & mission_header_flag_uses_trigger)) {
            add_mission_icon(instance);

            continue;
        }

        header->flags |= mission_header_flag_unk_00020000;

        mission_icon_info trigger = { -1, instance };
        mission_triggers->push_back(trigger);
    }
}

// sub_983D30
void mission_manager::add_mission_icon(const chuck::vm::script_instance* instance) {
    auto* header = (mission_header_instance_base*)instance->data.buffer;

    if (header->flags & mission_header_flag_unk_00020000)
        return;

    header->flags |= mission_header_flag_unk_00020000;

    mission_icon_info icon = { -1, nullptr };
    mission_icons->push_back(icon);

    header->mission_icon_index     = (i32)mission_icons->size() - 1;
    mission_icons->back().instance = instance;
}

// sub_983E40
void mission_manager::update_mission_icons() {
    if (!(flags & mission_manager_flag_mission_ready) && state != mission_manager_state_running_mission &&
        selected_poi.instance && selected_poi_icon) {

        auto* header = (mission_header_instance_base*)selected_poi.instance->data.buffer;

        if (!(header->flags & mission_header_flag_uses_trigger)) {
            vector3 position;
            position.x = header->key_position.x;
            position.y = (f32)((f64)header->key_position.y + (f64)1.6f);
            position.z = header->key_position.z;

            ((void (__thiscall*)(mission_manager*, void*, const vector3*))retail::sub_981190)(this, selected_poi_icon, &position);
        }
    }

    // add_mission_icon can grow the vector, the walk keeps the old range
    mission_icon_info* end = mission_icons->end();

    for (mission_icon_info* icon = mission_icons->begin(); icon != end; ++icon) {
        u32 header_flags = ((mission_header_instance_base*)icon->instance->data.buffer)->flags;

        if (!(header_flags & mission_header_flag_unk_00800000) && !(header_flags & mission_header_flag_unk_00008000))
            continue;

        if (header_flags & mission_header_flag_unk_00010000 || header_flags & mission_header_flag_unk_02000000)
            continue;

        add_mission_icon(icon->instance);
    }
}

// sub_985D60
void mission_manager::check_mission_triggers() {
    if (flags & mission_manager_flag_mission_ready || state == mission_manager_state_running_mission ||
        flags & mission_manager_flag_need_to_malor) {

        return;
    }

    if (selected_poi.instance && selected_poi_icon) {
        auto* header = (mission_header_instance_base*)selected_poi.instance->data.buffer;

        if (header->flags & mission_header_flag_uses_trigger)
            ((void (__thiscall*)(mission_manager*, void*, const vector3*))retail::sub_981190)(this, selected_poi_icon, &header->key_position);
    }

    vector3 hero_position = references::g_world_ptr.read()->hero_ptr->my_abs_po->get_position();

    for (mission_icon_info* trigger = mission_triggers->begin(); trigger != mission_triggers->end(); ++trigger) {
        auto* header = (mission_header_instance_base*)trigger->instance->data.buffer;

        f32 dx = (f32)((f64)hero_position.x - (f64)header->key_position.x);
        f32 dy = (f32)((f64)hero_position.y - (f64)header->key_position.y);
        f32 dz = (f32)((f64)hero_position.z - (f64)header->key_position.z);

        f32 distance_squared = (f32)((f64)dx * (f64)dx + (f64)dy * (f64)dy + (f64)dz * (f64)dz);

        if ((f64)header->trigger_radius * (f64)header->trigger_radius > (f64)distance_squared) {
            retail::sub_9855F0((i32)this, 0, 1); // terminate_script

            // get_mission_info
            ((void (__thiscall*)(mission_manager*, const chuck::vm::script_instance*, mission_info*))
            retail::sub_981CD0)(this, trigger->instance, &next_mission);

            flags      |= mission_manager_flag_mission_ready | mission_manager_flag_using_trigger;
            checkpoint  = 0;

            return;
        }
    }
}

// sub_9821E0
void mission_manager::launch_auto_launch_instance(const chuck::vm::script_instance* instance) {
    flags |= mission_manager_flag_unk_00040000;

    // get_mission_info
    ((void (__thiscall*)(mission_manager*, const chuck::vm::script_instance*, mission_info*))
    retail::sub_981CD0)(this, instance, &next_mission);

    flags      |= mission_manager_flag_mission_ready;
    checkpoint  = 0;
}
