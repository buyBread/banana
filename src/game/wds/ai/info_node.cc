#include "retail.hh"
#include "game/als/als.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/colgeom.hh"
#include "game/math/references.hh"

using namespace treyarch;

// sub_4C3A70
actor* info_node::get_actor() const {
    return my_ai_core->my_actor;
}

als::category_id als_inode::get_category_id(als::layer_id the_layer) {
    als::category_id cat;

    ((als::category_id* (__thiscall*)(als_inode*, als::category_id*, als::layer_id))retail::sub_409930)(this, &cat, the_layer);

    return cat;
}

const combo_move_data* als_inode::get_combo_move(als::layer_id the_layer) {
    return ((const combo_move_data* (__thiscall*)(als_inode*, als::layer_id))retail::sub_44F330)(this, the_layer);
}

bool als_inode::anim_finished(als::category_id cat, als::layer_id the_layer, bool allow_looping, bool ignore_time) {
    return ((bool (__thiscall*)(als_inode*, als::category_id, als::layer_id, bool, bool))retail::sub_44F1F0)(this,
                                                                                                           cat,
                                                                                                           the_layer,
                                                                                                           allow_looping,
                                                                                                           ignore_time);
}

void als_inode::request_category_transition(als::category_id cat,
                                            als::layer_id    layer,
                                            bool             should_succeed,
                                            bool             is_master_request,
                                            bool             ingore_same_cat_request_protection,
                                            bool             unk) {

    ((void (__thiscall*)(als_inode*,
                         als::category_id,
                         als::layer_id,
                         bool,
                         bool,
                         bool,
                         bool))retail::sub_457B20)(this,
                                                   cat,
                                                   layer,
                                                   should_succeed,
                                                   is_master_request,
                                                   ingore_same_cat_request_protection,
                                                   unk);
}

f32 als_inode::get_time_to_signal(string_hash       evt,
                                  als::layer_id     the_layer,
                                  bool*             signal_exists,
                                  bool              scale_by_speed,
                                  anim_event_param* param) {

    return ((f32 (__thiscall*)(als_inode*,
                               string_hash,
                               als::layer_id,
                               bool*,
                               bool,
                               anim_event_param*))retail::sub_44F390)(this,
                                                                      evt,
                                                                      the_layer,
                                                                      signal_exists,
                                                                      scale_by_speed,
                                                                      param);
}

i32 als_inode::get_optional_als_pb_int(const string_hash   &parameter,
                                       const i32           &default_value,
                                             als::layer_id  the_layer,
                                             bool*          exists) const {

    return ((i32 (__thiscall*)(const als_inode*,
                               const string_hash*,
                               const i32*,
                               als::layer_id,
                               bool*))retail::sub_44F510)(this,
                                                          &parameter,
                                                          &default_value,
                                                          the_layer,
                                                          exists);
}

// sub_450480
void loco_inode::set_needs_repathfind(bool needs_it, bool latch) {
    needs_repathfind = needs_it;

    if (!repathfind_latch)
        repathfind_latch = latch;
}

// sub_458440
const vector3d &loco_inode::get_goto_dest_pos() const {
    if (unk_064.size)
        return unk_064.array[unk_064.size - 1];

    if (goto_points.size)
        return goto_points.array[goto_points.size - 1];

    return references::unk_00fc3d70.get();
}

// sub_44FAD0
bool loco_inode::is_using_swift() const {
    return my_param_block.get_optional_pb_int(currently_using_swift_key, 0, nullptr) != 0;
}

// sub_44FAB0
void loco_inode::set_flag_stop_movement(bool stop) {
    my_param_block.set_pb_int(flag_stop_movement_key, stop);
}

// sub_44FA70
void loco_inode::set_allow_als_category_request(bool allow) {
    my_param_block.set_pb_int(allow_als_category_request_key, allow);
}

// sub_4BEA10
bool std_carry_inode::add_slave(std_carry_inode* slave, bool engage_slave_mode) {
    engine_lock_scope scope(&lock);

    if (!slave->get_core()->is_in_slave_mode() &&
        (!engage_slave_mode || !slave->get_core()->attempt_to_engage_slave_mode(get_actor()->my_handle, (als::layer_id)9)))

        return false;

    if (slave == desired_slave)
        desired_slave = nullptr;

    // the carried actor stops blocking the camera
    collision_geometry* colgeom = slave->get_actor()->get_colgeom();

    if (slave->get_actor()->flags & entity_flag_member)
        colgeom = ((entity*)retail::sub_601910((u32*)slave->get_actor()))->get_colgeom(); // entity_base::get_attach_root

    if (colgeom)
        colgeom->flags = (colgeom->flags & ~fi_collide_camera) | fi_collide_camera_see_thru;
    else {
        slave->get_actor()->set_colgeom_flag_recursive(fi_collide_camera, false);
        slave->get_actor()->set_colgeom_flag_recursive(fi_collide_camera_see_thru, true);
    }

    carry_slaves->push_back(slave);
    slave->carry_master = this;

    if (animation_controller* slave_ctrl = slave->get_actor()->anim_ctrl)
        retail::sub_5F5E90(slave_ctrl, nullptr, nullptr); // animation_controller::set_master

    auto* master_als = (als_inode*)get_core()->get_info_node(info_node_type_als);
    auto* slave_als  = (als_inode*)slave->get_core()->get_info_node(info_node_type_als);

    if (master_als) {
        string_hash carry;
        carry.initialize(mash::ALLOCATED, "Carry");

        master_als->request_category_transition(carry, (als::layer_id)9, true, false, false, false);

        if (slave_als) {
            auto* master_logic = master_als->my_als;
            auto* slave_logic  = slave_als->my_als;

            slave_logic ->set_coordinator(master_logic, 0);
            master_logic->set_coordinator(master_logic, 0);
        }
    }

    string_hash bone;
    bone = references::bip01_r_clavicle.read();

    if (slave_als) {
        auto* slave_logic = slave_als->my_als;

        engine_lock_scope als_scope(slave_logic->get_access_lock());

        string_hash carried_sync;
        carried_sync.initialize(mash::ALLOCATED, "Carried_Sync");

        slave_als->request_category_transition(carried_sync, (als::layer_id)9, true, true, false, false);
        slave_logic->force_update(0.0001f, true);

        string_hash carry_bone;

        bone = *(string_hash*)retail::sub_44F550((u32**)slave_als, // als_inode::get_optional_pb_hash
                                                 (i32)&carry_bone,
                                                 (u32*)&carry_bone_key,
                                                 (u32*)&references::bip01_r_clavicle.get(),
                                                 9,
                                                 0);

        attach_slave(slave, bone);

        string_hash carried;
        carried.initialize(mash::ALLOCATED, "Carried");

        slave_als->request_category_transition(carried, (als::layer_id)9, true, true, false, false);
        slave_logic->force_update(0.0001f, true);
    } else
        attach_slave(slave, bone);

    if (slave->get_actor()->has_ifc(entity_ifc_physical)) {
        slave->get_actor()->physical_ifc()->cancel_all_velocity(references::unk_00fc4a10.get(), true);
        slave->get_actor()->physical_ifc()->enable(false);
    }

    if (slave->get_actor()->has_ifc(entity_ifc_standing))
        slave->get_actor()->standing_ifc()->enable(false);

    slave->get_actor()->set_collisions_active(false, true);

    retail::sub_65B290((u32**)slave->get_actor(), (i32)get_actor()->my_handle.value); // actor::add_collision_ignorance
    retail::sub_65B290((u32**)get_actor(), (i32)slave->get_actor()->my_handle.value); // actor::add_collision_ignorance

    if (get_actor()->has_ifc(entity_ifc_player))
        get_actor()->player_ifc()->unk_135(1);

    slave->get_actor()->raise_event(references::picked_up.read());

    my_param_block.set_pb_hash(carry_type_key, slave->get_carry_type());

    return true;
}

// inlined @ sub_4BEA10
void std_carry_inode::attach_slave(std_carry_inode* slave, const string_hash &bone) { // onto the bone of a conglomerate that has it, else onto the carrier
    if (get_actor()->is_a_conglomerate() && ((conglomerate*)get_actor())->has_member_for_linking(bone)) {
        auto* body   = (conglomerate*)get_actor();
        auto  member = retail::sub_655D80((i32)body, body->get_member_index(bone)); // conglomerate::get_member_for_linking

        retail::sub_653EB0((u32*)slave->get_actor(), (i32)member); // attaches it keeping its world po
        retail::sub_626BE0((i32)slave->get_actor(), (u64*)&references::unk_00fc4f60.get()); // entity_base::set_rel_po
    } else
        retail::sub_653EB0((u32*)slave->get_actor(), (i32)get_actor()); // attaches it keeping its world po
}

void std_carry_inode::remove_slave(std_carry_inode* slave, bool kill_slave_mode) {
    retail::sub_4BA230((u32*)this, (i32)slave, kill_slave_mode);
}

// sub_492510
bool std_carry_inode::get_scaring_informant() const {
    return my_param_block.get_optional_pb_int(scaring_informant_key, 0, nullptr) != 0;
}

// sub_492540
bool std_carry_inode::get_dropping_informant() const {
    return my_param_block.get_optional_pb_int(dropping_informant_key, 0, nullptr) != 0;
}

// sub_4923F0
bool std_carry_inode::get_allow_web_pickup() const {
    return my_param_block.get_optional_pb_int(allow_web_pickup_key, 1, nullptr) != 0;
}

// sub_492370
bool std_carry_inode::get_allow_pickup() const {
    return my_param_block.get_optional_pb_int(allow_pickup_key, 0, nullptr) != 0;
}

// sub_4924A0
string_hash std_carry_inode::get_carry_type() const {
    string_hash none;
    none.initialize(mash::ALLOCATED);

    return my_param_block.get_optional_pb_hash(carry_type_key, none, nullptr);
}

// sub_482510
void slave_inode::internal_engage_slave_mode(arch_base_vhandle new_master, als::layer_id desired_als_layer) {
    engine_lock_scope scope(&lock);

    if (!masters.size) {
        get_core()->push_base_machine(references::slave_asg.get(), true);

        get_core()->flags |= ai_core_flag_slave_mode;
        get_core()->flags &= ~ai_core_flag_unk_00000100;
    }

    masters.push_back({ new_master, desired_als_layer });
}

// sub_490FF0
bool std_puppet_inode::get_start_rhino_ride() const {
    return my_param_block.get_optional_pb_int(start_rhino_ride_key, 0, nullptr) != 0;
}

// sub_490DD0
i32 std_puppet_inode::get_puppet_generic_in_service() const {
    return my_param_block.get_optional_pb_int(puppet_generic_in_service_key, 0, nullptr);
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
