#include "retail.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/core_ai_resource.hh"
#include "game/wds/ai/fight_director.hh"
#include "game/math/references.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
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

// sub_4C3A70
actor* info_node::get_actor() const {
    return my_ai_core->my_actor;
}

bool std_carry_inode::add_slave(std_carry_inode* slave, bool engage_slave_mode) {
    return retail::sub_4BEA10((u32*)this, (i32*)slave, engage_slave_mode);
}

void std_carry_inode::remove_slave(std_carry_inode* slave, bool kill_slave_mode) {
    retail::sub_4BA230((u32*)this, (i32)slave, kill_slave_mode);
}

void std_fear_inode::reset_morale() {
    retail::sub_44EFF0((i32)this);
}

void std_fear_inode::break_morale_force_cower(f32 unk) {
    retail::sub_457530((i32)this, unk);
}

void std_fear_inode::break_morale_force_flee(vector3d position, f32 unk) {
    ((void (__thiscall*)(std_fear_inode*, vector3d, f32))retail::sub_457780)(this, position, unk);
}

void std_fear_inode::break_morale_force_flee(arch_base_vhandle target, f32 unk) {
    ((void (__thiscall*)(std_fear_inode*, arch_base_vhandle, f32))retail::sub_457650)(this, target, unk);
}

bool ai_car_inode::force_enter_car(arch_base_vhandle car, i32 seat, bool animate) {
    return ((bool (__thiscall*)(ai_car_inode*, arch_base_vhandle, i32, bool))retail::sub_4C15F0)(this, car, seat, animate);
}

bool ai_car_inode::force_exit_car(bool animate) {
    return retail::sub_4BFA10((i32)this, animate);
}

void crawl_layer_inode::attach_to_surface(const line_info &lif, const vector3d &desired_forward) {
    retail::sub_45AC10((u8*)this, (i32)&lif, (f32*)&desired_forward);
}

void crawl_layer_inode::release_from_surface(const vector3d &desired_forward) {
    retail::sub_45B250(this, (i32)&desired_forward);
}

// sub_452D60
vector3d teleporting_layer_inode::get_teleport_target_pos() const {
    return my_param_block.get_optional_pb_vector3d(teleport_target_pos_key, references::unk_00fc3d70.get(), nullptr);
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
        return *((const vector3d* (__thiscall*)(loco_inode*))retail::sub_458440)(my_loco_inode); // loco_inode: last goto point

    return my_actor->my_abs_po->get_position();
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
        info_node* swift_layer = get_info_node(type);

        if (swift_layer && ((bool (__thiscall*)(info_node*))retail::sub_44FAD0)(swift_layer)) { // swift layer: in use
            ((void (__thiscall*)(info_node*, bool))retail::sub_44FAB0)(swift_layer, true); // swift layer: stop movement

            return true;
        }
    }

    if (!lock->try_acquire_recursive())
        return false;

    bool stopped = false;

    if (my_locomotion_machine) {
        if (!((bool (__thiscall*)(loco_inode*))(*(void***)my_loco_inode)[23])(my_loco_inode)) // slot 23
            ((void (__thiscall*)(ai_state_machine*))retail::sub_4C6990)(my_locomotion_machine); // ai_state_machine: request exit

        stopped = true;
    } else if (my_locomotion_mode == ai_core_mode_killing_machines) {
        my_locomotion_mode     = ai_core_mode_normal;
        locomotion_exit_status = state_exit_code_failure;

        stopped = true;
    }

    lock->release();

    return stopped;
}

bool ai_core::push_base_machine(ai_core* other_core, const resource_key &the_state_graph, bool do_it_now) {
    return retail::sub_4DE730((i32*)this, (i32*)other_core, (u32*)&the_state_graph, do_it_now);
}

bool ai_core::pop_base_machine(const resource_key &the_state_graph, bool do_it_now) {
    return retail::sub_4DE570((i32)this, (u32*)&the_state_graph, do_it_now);
}

bool ai_core::change_locomotion_machine(string_hash the_loco_inode) {
    return retail::sub_4CC6E0((i32)this, (i32)the_loco_inode.source_hash_code);
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
bool ai_core::should_be_in_limbo() const {
    entity* owner = (entity*)my_actor;

    if (!(flags & ai_core_flag_ignore_limbo)) {
        if (owner->flags & 1)
            return true;

        if (!owner->get_primary_region())
            return true;
    }

    return ((bool (__thiscall*)(entity*))owner->vtable[0x198 / 4])(owner);
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

    string_hash team;

    retail::sub_435400((u32*)&my_param_block, // param_block::get_optional_pb_hash
                       &team.source_hash_code,
                       references::team_hash.read().source_hash_code,
                       (i32)no_team.source_hash_code,
                       nullptr);

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
