#include "retail.hh"
#include "game/als/als.hh"
#include "game/cutscene/cut_scene_player.hh"
#include "game/slim/slim_controller.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_core_jq.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/wds/ai/core_ai_resource.hh"
#include "game/wds/ai/fight_director.hh"
#include "game/wds/camera/references.hh"
#include "game/math/references.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/references.hh"

using namespace treyarch;

// inlined @ sub_4E1780
void ai_core_list::remove(ai_core_list_node* node) {
    if (node->previous)
        node->previous->next = node->next;
    else
        node->list->head = node->next;

    if (node->next)
        node->next->previous = node->previous;
    else
        node->list->tail = node->previous;

    --node->list->count;

    node->next     = nullptr;
    node->previous = nullptr;
    node->list     = nullptr;
}

// inlined @ sub_4E1780
void ai_core_list::push_back(ai_core_list_node* node) {
    if (tail) {
        tail->next     = node;
        node->previous = tail;
        node->next     = nullptr;
        tail           = node;
    } else {
        node->next     = head;
        node->previous = nullptr;

        if (head)
            head->previous = node;

        head = node;

        if (!node->next)
            tail = node;
    }

    node->list = this;
    ++count;
}

// sub_4CE100
void ai_core::team_lists::clear() {
    for (vc8::vector<ai_core*> &team : teams)
        team.clear();
}

// sub_4DB9A0
void ai_core::team_lists::rebuild() {
    clear();

    for (ai_core_list_node* node = references::ai_core_list_high.get().head; node; node = node->next) {
        ai_core* core = node->core;

        if (core->my_team < ai_team::num_teams && !(core->flags & ai_core_flag_in_limbo))
            teams[core->my_team].push_back(core);
    }

    for (ai_core_list_node* node = references::ai_core_list_low.get().head; node; node = node->next) {
        ai_core* core = node->core;

        if (core->my_team < ai_team::num_teams && !(core->flags & ai_core_flag_in_limbo))
            teams[core->my_team].push_back(core);
    }

    valid = true;
}

// sub_402D50
// inlined @ sub_5C68C0
// inlined @ sub_5C78B0
// inlined @ sub_5C7B20
// inlined @ sub_5DB360
// inlined @ sub_5F3990
// inlined @ sub_8068A0
// inlined @ sub_981D50
// inlined @ sub_4C7A40
// inlined @ sub_86A1B0
// inlined @ sub_86A2A0
// inlined @ sub_86A3B0
// inlined @ sub_86A470
// inlined @ sub_870EF0
// inlined @ sub_4C77E0
// inlined @ sub_4CB950
// inlined @ sub_840570
// inlined @ sub_73F990
// inlined @ sub_758B90
// inlined @ sub_4D22E0
// inlined @ sub_4BEA10
// inlined @ sub_4BF1F0
// inlined @ sub_495E60
// inlined @ sub_496EF0
// inlined @ sub_49C200
// inlined @ sub_4A2420
// inlined @ sub_4BE3E0
// inlined @ sub_4BFE10
// inlined @ sub_4F9FD0
// inlined @ sub_9925A0
// inlined @ sub_4BF060
// inlined @ sub_4A1DA0
// inlined @ sub_41F3E0
// inlined @ sub_580EC0
// inlined @ sub_5745C0
// inlined @ sub_552EC0
info_node* ai_core::get_info_node(e_info_node_type type) const {
    u32 word = type >> 5;
    u32 bit  = 1u << (type & 0x1F);

    if (!my_info_node_list || !(info_node_mask[word] & bit))
        return nullptr;

    // nodes are packed in type order, so the high word's nodes sit after every low-word node
    u32 position = retail::sub_401F50(info_node_mask[word] & (bit - 1));

    if (word)
        position += retail::sub_401F50(info_node_mask[0]);

    return (*my_info_node_list)[position];
}

// sub_4C2EA0
// inlined @ sub_4C5840
// inlined @ sub_4C58D0
// inlined @ sub_4C5AC0
bool ai_core::is_tether_active() const {
    return my_param_block.get_optional_pb_int(tether_active_key, 0, nullptr) > 0;
}

// sub_4C2E60
// inlined @ sub_4C58D0
// inlined @ sub_4C5AC0
f32 ai_core::get_tether_radius() const {
    return my_param_block.get_optional_pb_float(tether_radius_key, 1000.0f, nullptr);
}

// sub_4C5800
vector3d ai_core::get_tether_center() const {
    return my_param_block.get_optional_pb_vector3d(tether_center_key, my_actor->my_abs_po->get_position(), nullptr);
}

// sub_4C2ED0
void ai_core::enable_tether(const vector3d &center, f32 radius) {
    my_param_block.set_pb_vector3d(tether_center_key, center);
    my_param_block.set_pb_float(tether_radius_key, radius);
    my_param_block.set_pb_int(tether_active_key, 1);
}

// sub_4C5840
void ai_core::disable_tether() {
    if (is_tether_active()) {
        my_param_block.set_pb_vector3d(tether_center_key, my_actor->my_abs_po->get_position());
        my_param_block.set_pb_float(tether_radius_key, 1000.0f);
    }

    my_param_block.set_pb_int(tether_active_key, 0);
}

// sub_4C58D0
vector3d ai_core::clip_to_tether(const vector3d* pos, bool* clipped) const {
    if (is_tether_active()) {
        f32 radius = get_tether_radius();

        if (radius >= 0.0f) {
            vector3d center   = get_tether_center();
            vector3d offset   = *pos - center;
            f32      distance = offset.length();

            if (distance > radius) {
                // lands just inside the sphere
                if (radius > 0.01f)
                    radius -= 0.01f;

                f32 scale = radius / distance;

                if (clipped)
                    *clipped = true;

                return vector3d(center.x + offset.x * scale,
                                center.y + offset.y * scale,
                                center.z + offset.z * scale);
            }
        }
    }

    if (clipped)
        *clipped = false;

    return *pos;
}

// sub_4C5AC0
bool ai_core::is_within_tether(const vector3d* pos, f32 margin) const {
    if (!is_tether_active())
        return true;

    f32 radius = get_tether_radius();

    if (radius < 0.0f)
        return true;

    vector3d center = get_tether_center();

    return radius >= (f64)(*pos - center).length() + margin;
}

// sub_4C63E0
vector3d ai_core::get_movement_dest() const {
    if (my_loco_inode && is_moving())
        return my_loco_inode->get_goto_dest_pos();

    return my_actor->my_abs_po->get_position();
}

// sub_4C2F60
bool ai_core::is_strafing() const {
    return is_moving() && my_loco_inode && my_loco_inode->allow_facing;
}

// sub_4C2F30
// inlined @ sub_4C62A0
bool ai_core::set_facing_dir(const vector3d &dir, f32 rot_speed_deg) {
    if (!my_loco_inode)
        return false;

    return ((bool (__thiscall*)(loco_inode*, const vector3d*, f32))retail::sub_458480)(my_loco_inode, &dir, rot_speed_deg); // loco_inode::set_facing_dir
}

// sub_4C62A0
bool ai_core::set_facing_point(const vector3d &point, f32 rot_speed_deg) {
    return set_facing_dir(point - my_actor->my_abs_po->get_position(), rot_speed_deg);
}

// sub_4C7A40
bool ai_core::stop_movement() {
    if (!my_loco_inode)
        return true;

    // a swift layer that's in use stops itself
    for (e_info_node_type type : { info_node_type_swift_surface_layer, info_node_type_swift_flight_layer }) {
        auto swift_layer = (loco_inode*)get_info_node(type);

        if (swift_layer && swift_layer->is_using_swift()) {
            swift_layer->set_flag_stop_movement(true);

            return true;
        }
    }

    if (!lock->try_acquire_recursive())
        return false;

    bool stopped = false;

    if (my_locomotion_machine) {
        if (!my_loco_inode->is_player_locomotion())
            my_locomotion_machine->external_request_exit();

        stopped = true;
    } else if (my_locomotion_mode == ai_core_mode_killing_machines) {
        my_locomotion_mode     = ai_core_mode_normal;
        locomotion_exit_status = state_exit_code_failure;

        stopped = true;
    }

    lock->release();

    return stopped;
}

// sub_4DF470
// inlined @ sub_4DF500
void ai_core::push_disable(i32 unk) {
    if (disable_count <= 0) {
        push_base_machine(references::disabled_asg.get(), true);
        stop_movement();
        my_actor->raise_event(references::ai_disabled.read());
    }

    ++disable_count;
}

// sub_4DF4B0
// inlined @ sub_4DF600
void ai_core::pop_disable(i32 unk) {
    bool was_disabled = disable_count > 0;

    if (--disable_count < 0)
        disable_count = 0;

    if (was_disabled && disable_count <= 0) {
        pop_base_machine(references::disabled_asg.get(), true);
        my_actor->raise_event(references::ai_enabled.read());
    }
}

// sub_4DF500
void ai_core::push_disable_all(bool ignore_low_priority) {
    engine_lock_scope list_lock(&references::ai_core_list_lock.get());

    for (ai_core_list_node* node = references::ai_core_list_high.get().head; node; node = node->next)
        node->core->push_disable(0);

    for (ai_core_list_node* node = references::ai_core_list_low.get().head; node; node = node->next)
        node->core->push_disable(0);
}

// sub_4DF600
void ai_core::pop_disable_all(bool ignore_low_priority) {
    engine_lock_scope list_lock(&references::ai_core_list_lock.get());

    for (ai_core_list_node* node = references::ai_core_list_high.get().head; node; node = node->next)
        node->core->pop_disable(0);

    for (ai_core_list_node* node = references::ai_core_list_low.get().head; node; node = node->next)
        node->core->pop_disable(0);
}

// sub_4DE2E0
bool ai_core::change_base_machine(const resource_key &the_state_graph, bool do_it_now) {
    engine_lock_scope core_lock(lock);

    if (!retail::sub_4C7290((u32*)my_resource, the_state_graph.hash.source_hash_code, the_state_graph.type)) // core_ai_resource: lists the base graph
        return false;

    if (!retail::sub_4C7400(the_state_graph.hash.source_hash_code, the_state_graph.type, my_resource->resource_context)) // state_graph_manager::find_state_graph_from_resource
        return false;

    if (my_mode != ai_core_mode_killing_machines)
        my_mode = ai_core_mode_change_machines;

    if (&the_state_graph != &new_base_machine)
        new_base_machine.set(the_state_graph);

    if (do_it_now) {
        if (my_base_machine) {
            retail::sub_4CD3B0((u32*)this, (i32)my_base_machine); // ai_core: unregister machine

            if (ai_state_machine* old_machine = my_base_machine) {
                retail::sub_4CE450((u32*)old_machine); // ai_state_machine::~ai_state_machine
                mm::free(old_machine);
            }

            my_base_machine = nullptr;
        }

        stop_movement();

        retail::sub_4DD830((u32*)this, nullptr, (u32*)&new_base_machine, (char**)&my_base_machine); // ai_core::spawn_state_machine_internal

        my_mode = ai_core_mode_normal;

        frame_advance(0.0001f);
    }

    return true;
}

// sub_4DE460
bool ai_core::push_base_machine(const resource_key &the_state_graph, bool do_it_now) {
    engine_lock_scope core_lock(lock);

    if (!my_base_machine)
        return false;

    // a pending change counts as the running machine
    resource_key        running;
    const resource_key* current = &new_base_machine;

    if (my_mode != ai_core_mode_change_machines && my_mode != ai_core_mode_killing_machines) {
        running = my_base_machine->get_name();
        current = &running;
    }

    resource_key saved;
    saved.set(*current);

    if (!change_base_machine(the_state_graph, do_it_now))
        return false;

    base_machine_stack.push_front(saved);

    return true;
}

// sub_4DE570
bool ai_core::pop_base_machine(const resource_key &the_state_graph, bool do_it_now) {
    engine_lock_scope core_lock(lock);

    if (base_machine_stack.empty())
        return false;

    if (my_mode == ai_core_mode_change_machines) {
        // popping the pending machine cancels the change
        if (the_state_graph == new_base_machine) {
            resource_key &below = base_machine_stack.front();

            if (&below != &new_base_machine)
                new_base_machine.set(below);

            base_machine_stack.pop_front();

            my_mode = ai_core_mode_normal;

            return true;
        }
    } else if (my_base_machine->get_name() == the_state_graph) {
        resource_key below;
        below.set(base_machine_stack.front());

        base_machine_stack.pop_front();

        return change_base_machine(below, do_it_now);
    }

    for (auto* position = base_machine_stack.begin(); position != base_machine_stack.end(); position = position->next) {
        if (position->value == the_state_graph) {
            base_machine_stack.erase(position);

            return true;
        }
    }

    return false;
}

// sub_4DE730
bool ai_core::attempt_push_base_machine(ai_core* asker, const resource_key &the_state_graph, bool do_it_now) {
    if (!asker)
        asker = this;

    if (!retail::sub_4CDEC0((i32*)asker, (i32*)this, nullptr)) // wait_for_other_ai
        return false;

    push_base_machine(the_state_graph, do_it_now);

    retail::sub_4CC650((i32)asker, (u32*)this); // release_wait_for_other_ai

    return true;
}

// sub_4D22E0
bool ai_core::attempt_to_engage_slave_mode(arch_base_vhandle new_master, als::layer_id desired_als_layer) {
    auto* master = (actor*)new_master.resolve();

    if (!master)
        return false;

    als::animation_logic_system_interface* als = my_actor->get_als();

    if (!als)
        return false;

    // a layer that already has a master is only taken over while it can be interrupted
    if (retail::sub_4824F0((u32*)get_info_node(info_node_type_slave), desired_als_layer) &&
        !als->get_als_layer(desired_als_layer)->is_interruptable())

        return false;

    ai_core* other = master->get_ai_core();

    if (!other)
        other = this;

    if (!retail::sub_4CDEC0((i32*)other, (i32*)this, nullptr)) // wait_for_other_ai
        return false;

    ((slave_inode*)get_info_node(info_node_type_slave))->internal_engage_slave_mode(new_master, desired_als_layer);

    retail::sub_4CC650((i32)other, (u32*)this); // release_wait_for_other_ai

    return true;
}

// sub_4DF720
// inlined @ sub_4E0E00
bool ai_core::restart_base_machine() {
    if (!my_base_machine)
        return false;

    change_base_machine(my_base_machine->get_name(), true);

    return true;
}

const ai_state_machine* ai_core::find_machine(const resource_key &graph_name) const {
    return ((const ai_state_machine* (__thiscall*)(const ai_core*, const resource_key*))retail::sub_4CCD60)(this, &graph_name);
}

bool ai_core::change_locomotion_machine(string_hash the_loco_inode) {
    return retail::sub_4CC6E0((i32)this, (i32)the_loco_inode.source_hash_code);
}

// sub_4E03E0
void ai_core::advance_machine_recursive(ai_state_machine* the_machine, f32 delta_t) {
    for (i32 index = 0; index < the_machine->get_num_children(); ++index)
        advance_machine_recursive(the_machine->my_children[index], delta_t);

    the_machine->process_mode(delta_t);
}

void ai_core::do_machine_exit(ai_state_machine* the_machine) {
    retail::sub_4CCDC0((u32*)this, (char**)the_machine);
}

// sub_4E0550
void ai_core::frame_advance(f32 delta_t) {
    engine_lock_scope core_lock(lock);

    if (!my_actor->is_alive())
        stop_movement();

    if (!my_param_block.get_optional_pb_int(references::disable_path_location_inode_hash.read(), 0, nullptr) && path_location)
        retail::sub_447E50((i32)path_location, delta_t);

    if (flags & ai_core_flag_unk_00000100) {
        pop_base_machine(references::slave_asg.get(), false);

        flags &= ~ai_core_flag_unk_00000100;
    }

    if (my_mode == ai_core_mode_change_machines) {
        my_base_machine->request_exit();

        my_mode = ai_core_mode_killing_machines;
    }

    if (my_locomotion_mode == ai_core_mode_change_machines) {
        if (my_locomotion_machine)
            my_locomotion_machine->request_exit();

        my_locomotion_mode = ai_core_mode_killing_machines;
    }

    if (my_locomotion_machine)
        advance_machine_recursive(my_locomotion_machine, delta_t);

    if (my_base_machine) {
        if (slim::slim_actor_controller* tactics = tactics_controller) {
            if (base_controller) {
                if (base_controller->unk_04c->flags & 0x80)
                    tactics->flags |= 0x80;
                else
                    tactics->flags &= ~0x80;
            }

            auto basic = (ai_basic_inode*)get_info_node(info_node_type_ai_basic);

            if (!basic || basic->get_flag(ai_flag_allowed_to_tactics)) {
                retail::sub_518380((u32*)tactics, delta_t, 0);
                tactics->unk_002(delta_t);
            }
        }

        advance_machine_recursive(my_base_machine, delta_t);
    }

    retail::sub_4DB360((i32)this, delta_t); // ai_core::advance_info_nodes
    retail::sub_4D2150((u32*)this);         // ai_core::exit_pending_machines

    if (my_mode == ai_core_mode_killing_machines && !my_base_machine) {
        retail::sub_4DD830((u32*)this, nullptr, (u32*)&new_base_machine, (char**)&my_base_machine); // ai_core::spawn_state_machine_internal

        my_mode = ai_core_mode_normal;
    }

    // a new locomotion machine waits until the als's base layer can be interrupted
    if (my_locomotion_mode == ai_core_mode_killing_machines && !my_locomotion_machine) {
        auto als_node = (als_inode*)get_info_node(info_node_type_als);

        if (!als_node || ((als::animation_logic_system_interface*)als_node->my_als)->get_als_layer(als::base_layer)->is_interruptable()) {
            retail::sub_4DD830((u32*)this, // ai_core::spawn_state_machine_internal
                               nullptr,
                               (u32*)&my_loco_inode->get_loco_graph(),
                               (char**)&my_locomotion_machine);

            if (!my_locomotion_machine)
                locomotion_exit_status = state_exit_code_failure;

            my_locomotion_mode = ai_core_mode_normal;
        }
    }

    if (auto als_node = (als_inode*)get_info_node(info_node_type_als)) {
        auto the_als = (als::animation_logic_system_interface*)als_node->my_als;

        if ((the_als->get_als_layer(als::base_layer)->did_transition_succeed() || the_als->force_updated) &&
            !references::cut_scene_player.read()->is_playing() &&
            pending_sound_event.valid() &&
            !unk_628 &&
            (u32)retail::sub_4C5640((u32*)this) != unk_62c) {

            if (my_actor->has_sound_ifc()) {
                string_hash event(pending_sound_event.source_hash_code);

                retail::sub_65FA00((i32)my_actor->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)event.source_hash_code, false); // sound_interface: post event
            }

            pending_sound_event = string_hash();
            unk_62c             = 0;
        }
    }

    if (my_actor->is_alive())
        unk_630 = false;

    // despawn_effect_distance is compared with the squared distance to the camera
    bool far_away = false;

    if (my_param_block.does_parameter_exist(references::despawn_effect_distance_hash.read())) {
        f32 distance2 = (my_actor->my_abs_po->get_position() - references::camera_position.get().get_xyz()).length2();
        f32 limit     = my_param_block.get_pb_float(references::despawn_effect_distance_hash.read());

        if (distance2 > limit)
            far_away = true;
    }

    if (lifetime_effect) {
        if (unk_630 || !my_actor->is_visible() || far_away) {
            my_actor->stop_looping_effect(lifetime_effect);
            lifetime_effect = 0;
        }
    } else if (!far_away && !unk_630 && my_param_block.does_parameter_exist(references::lifetime_effect_hash.read())) {
        string_hash none;
        none.initialize(mash::ALLOCATED);

        if (my_param_block.get_pb_hash(references::lifetime_effect_hash.read()) != none && retail::sub_4E1FC0((u32*)my_actor)) // entity::is_visible
            lifetime_effect = my_actor->spawn_looping_effect(my_param_block.get_pb_hash(references::lifetime_effect_hash.read()), 0);
    }

    if (infected_effect) {
        if (unk_630 || !my_actor->is_visible() || far_away) {
            my_actor->stop_looping_effect(infected_effect);
            infected_effect = 0;
        }
    } else if (!far_away && !unk_630 && my_param_block.does_parameter_exist(references::infected_effect_hash.read())) {
        string_hash none;
        none.initialize(mash::ALLOCATED);

        if (my_param_block.get_pb_hash(references::infected_effect_hash.read()) != none && retail::sub_4E1FC0((u32*)my_actor)) // entity::is_visible
            infected_effect = my_actor->spawn_looping_effect(my_param_block.get_pb_hash(references::infected_effect_hash.read()), 0);
    }

    unk_628 = false;

    retail::sub_4CE090((u32*)this, delta_t); // ai_core::post_frame_advance
}

// sub_4E0E00
void ai_core::reset() {
    engine_lock_scope core_lock(lock);

    // without a base machine, up to ten tiny frames to spawn one
    if (!restart_base_machine()) {
        for (i32 attempt = 0; attempt < 10; ++attempt) {
            frame_advance(0.0001f);

            if (my_base_machine) {
                change_base_machine(my_base_machine->get_name(), true);
                break;
            }
        }
    }

    for (u32 index = 0; index < my_info_node_list->size; ++index)
        (*my_info_node_list)[index]->reset();
}

// sub_4E0C40
void treyarch::ai_core_jq_callback(jq::batch* batch) {
    auto    core  = (ai_core*)batch->payload[0];
    entity* owner = core->my_actor;

    f32 dilation = owner->has_ifc(entity_ifc_time) ?
        owner->time_ifc()->calc_time_dilation() :
        references::g_world_ptr.read()->time_mgr.get_time_dilation_factor();

    core->frame_advance((f32)((f64)dilation * (f64)ai_core_jq::delta_t.read()));
}

// sub_4C57B0
void ai_core::stop_looping_effects() {
    if (lifetime_effect) {
        my_actor->stop_looping_effect(lifetime_effect);
        lifetime_effect = 0;
    }

    if (infected_effect) {
        my_actor->stop_looping_effect(infected_effect);
        infected_effect = 0;
    }
}

// sub_4C6340
void ai_core::post_pending_sound_event() {
    if (pending_sound_event.valid()) {
        if (my_actor->has_ifc(entity_ifc_sound)) {
            string_hash event;
            event.initialize(mash::ALLOCATED, nullptr, pending_sound_event.source_hash_code);

            retail::sub_65FA00((i32)my_actor->my_ifc_storage->get_ifc(entity_ifc_sound), (i32)event.source_hash_code, false); // sound_interface: post event
        }

        pending_sound_event = string_hash();
        unk_62c             = 0;
    }

    unk_628 = false;
}

// sub_4C53F0
// inlined @ sub_4E1780
bool ai_core::should_be_in_limbo() const {
    entity* owner = (entity*)my_actor;

    if (!(flags & ai_core_flag_ignore_limbo)) {
        if (owner->flags & entity_flag_in_limbo)
            return true;

        if (!owner->get_primary_region())
            return true;
    }

    return owner->unk_102();
}

// sub_4CE030
void ai_core::enter_limbo() {
    if (my_info_node_list) {
        for (u32 index = 0; index < my_info_node_list->size; ++index)
            (*my_info_node_list)[index]->enter_limbo();
    }

    flags |= ai_core_flag_in_limbo;

    retail::sub_4CD5B0((u32*)this);
}

// sub_4C6500
void ai_core::exit_limbo() {
    if (my_info_node_list) {
        for (u32 index = 0; index < my_info_node_list->size; ++index)
            (*my_info_node_list)[index]->exit_limbo();
    }

    flags &= ~ai_core_flag_in_limbo;
}

// inlined @ sub_4E1780
bool ai_core::wants_low_priority() const {
    if (flags & ai_core_flag_forcing_high_priority)
        return false;

    if (retail::sub_601C50((u32*)my_actor))
        return false;

    return my_resource->low_priority_advance ||
           my_team == ai_team::team_true_neutral ||
           ((flags & ai_core_flag_unk_00000008) && my_team == ai_team::team_civilian);
}

// inlined @ sub_4E1780
void ai_core::refresh_team() {
    string_hash no_team;
    no_team.initialize(mash::ALLOCATED);

    string_hash team = my_param_block.get_optional_pb_hash(references::team_hash.read(), no_team, nullptr);

    my_team = ai_team::get_enum_by_hash(team);
}

// inlined @ sub_4E1780
void ai_core::refresh_alive() {
    entity* owner    = (entity*)my_actor;
    bool    is_alive = true;

    if (owner->has_ifc(entity_ifc_damage)) {
        damage_interface* damage = owner->damage_ifc();

        is_alive = damage->hit_points.val > 0;
    }

    alive = is_alive;
}

// sub_4E1780
void ai_core::frame_advance_all_core_ais(f32 delta_t) {
    engine_lock_scope list_lock(&references::ai_core_list_lock.get());

    ai_core_list &high_list = references::ai_core_list_high.get();
    ai_core_list &low_list  = references::ai_core_list_low.get();
    team_lists   &all_teams = references::team_lists.get();

    for (ai_core_list_node* node = high_list.head; node;) {
        ai_core* core = node->core;

        if (core->should_be_in_limbo()) {
            if (!(core->flags & ai_core_flag_in_limbo))
                core->enter_limbo();
        } else if (core->flags & ai_core_flag_in_limbo) {
            core->exit_limbo();
        }

        core->refresh_team();
        core->refresh_alive();

        ai_core_list_node* next = node->next;

        if (core->wants_low_priority()) {
            ai_core_list::remove(node);
            low_list.push_back(core->list_node);

            all_teams.valid = false;
        }

        node = next;
    }

    for (ai_core_list_node* node = low_list.head; node;) {
        ai_core* core = node->core;

        if (core->should_be_in_limbo()) {
            if (!(core->flags & ai_core_flag_in_limbo))
                core->enter_limbo();
        } else if (core->flags & ai_core_flag_in_limbo) {
            core->exit_limbo();
        }

        core->refresh_team();
        core->refresh_alive();

        ai_core_list_node* next = node->next;

        if (!core->wants_low_priority()) {
            ai_core_list::remove(node);
            high_list.push_back(core->list_node);

            all_teams.valid = false;
        }

        node = next;
    }

    all_teams.rebuild();

    retail::sub_4F5C80(delta_t);
    retail::sub_436250(delta_t);

    ai_core* hero_core = ((actor*)references::g_world_ptr.read()->hero_ptr)->get_ai_core();

    if (hero_core) {
        if (info_node* hero_targets = hero_core->get_info_node(info_node_type_combat_target))
            retail::sub_472FF0((u32*)hero_targets);
    }

    retail::sub_4CBD30();
    retail::sub_4CE750((u32*)fight_director::inst(), delta_t); // fight director frame
    retail::sub_99CB30(delta_t);
    retail::sub_4E0F90(delta_t, false); // info node pass
    retail::sub_4DCA30();
    retail::sub_4CA920(delta_t, false); // pre_ai_core_jq

    if (hero_core) {
        if (info_node* hero_targets = hero_core->get_info_node(info_node_type_combat_target))
            retail::sub_48BA50((u32**)hero_targets);
    }

    retail::sub_4DD0E0();
    retail::sub_4CA790(delta_t, false); // ai_core::advance_all_cores
    retail::sub_4E0F90(delta_t, true);  // info node post pass
}
