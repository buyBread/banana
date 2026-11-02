#include <cmath>
#include <cstdlib>

#include "retail.hh"
#include "game/wds/entity/interface/spiderman_rvb_attacks.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/manip_obj_interface.hh"
#include "game/wds/entity/interface/occupant_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity_base.hh"
#include "game/wds/entity/vehicle.hh"
#include "game/als/als.hh"
#include "game/als/param.hh"
#include "game/input/game_button.hh"
#include "game/game.hh"
#include "game/game_meter_manager.hh"
#include "game/math/po.hh"
#include "game/math/references.hh"
#include "game/mutex.hh"
#include "treyarch/math/trig.hh"

using namespace treyarch;

// ==== spiderman_rvb_grapple_attack ==== //

string_hash spiderman_rvb_grapple_attack::my_type_key { 0x15AF1FAD }; // "spiderman_rvb_grapple_attack"

// sub_71AE40
spiderman_rvb_grapple_attack::~spiderman_rvb_grapple_attack() {}

// sub_99CD60
void spiderman_rvb_grapple_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_grapple_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_grapple_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71AE30
mash::virtual_types_key spiderman_rvb_grapple_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_grapple_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_grapple_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9B6B90
bool spiderman_rvb_grapple_attack::update(e_combat_move            move,
                                          const player_interface*  plr_ifc,
                                          als_inode*               my_als,
                                          f32                      delta_t,
                                          f32*                     best_time,
                                          e_combat_move            current_chosen_move,
                                          const plr_combat_attack* current_chosen_attack,
                                          const void*              unk) {

    // against the light ranged attack only a press of its own, with targets, wins
    if (current_chosen_move == combat_move_light_ranged)
        return update_against_own_best(move,
                                       plr_ifc,
                                       my_als,
                                       delta_t,
                                       best_time,
                                       current_chosen_move,
                                       current_chosen_attack,
                                       unk,
                                       true);

    return plr_combat_attack::update(move,
                                     plr_ifc,
                                     my_als,
                                     delta_t,
                                     best_time,
                                     current_chosen_move,
                                     current_chosen_attack,
                                     unk);
}

// sub_9A6CC0
bool spiderman_rvb_grapple_attack::attack_possible(const player_interface* plr_ifc) {
    return plr_combat_attack::attack_possible(plr_ifc);
}

// sub_9B6AD0
f32 spiderman_rvb_grapple_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    if (!retail::sub_40F580((i32)tmp_target->target.resolve())) // the target's als has the Combat_SM_Grapple_Grab category
        return -1.0f;

    i32 mode = plr_ifc->get_mode();

    if (mode < 3 || mode > 7)
        return plr_combat_attack::compute_target_score(tmp_target, plr_ifc, flag_override);

    u32    flags = flag_override ? flag_override : target_flags;
    actor* owner = plr_ifc->my_actor;

    return tmp_target->compute_score(owner,
                                     owner->my_abs_po->get_position(),
                                     attack_dir,
                                     attack_up,
                                     min_range,
                                     3.0f,
                                     min_vert_range,
                                     max_vert_range,
                                     -1.1f,
                                     -1.1f,
                                     flags,
                                     nullptr,
                                     0,
                                     0,
                                     0x14);
}

// ==== spiderman_rvb_air_attack ==== //

string_hash spiderman_rvb_air_attack::my_type_key { 0x9606D43E }; // "spiderman_rvb_air_attack"

// sub_71AEE0
spiderman_rvb_air_attack::~spiderman_rvb_air_attack() {}

// sub_99CD60
void spiderman_rvb_air_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_air_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_air_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71AEA0
mash::virtual_types_key spiderman_rvb_air_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_air_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_air_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9B6C50
bool spiderman_rvb_air_attack::attack_possible(const player_interface* plr_ifc) {
    if (!plr_combat_attack::attack_possible(plr_ifc))
        return false;

    actor* owner = plr_ifc->my_actor;

    if (owner->has_standing_ifc() && owner->standing_ifc()->is_effectively_standing())
        return false;

    return true;
}

// ==== spiderman_rvb_air_jump_attack ==== //

string_hash spiderman_rvb_air_jump_attack::my_type_key { 0x7A045479 }; // "spiderman_rvb_air_jump_attack"

// sub_71AF80
spiderman_rvb_air_jump_attack::~spiderman_rvb_air_jump_attack() {}

// sub_99CD60
void spiderman_rvb_air_jump_attack::construct_mashed_class() {
    spiderman_rvb_air_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_air_jump_attack::destruct_mashed_class() {
    spiderman_rvb_air_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_air_jump_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    spiderman_rvb_air_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71AF40
mash::virtual_types_key spiderman_rvb_air_jump_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AF50
bool spiderman_rvb_air_jump_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (spiderman_rvb_air_attack::get_virtual_type_key() == parent_class)
        return true;

    if (spiderman_rvb_air_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_air_jump_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9B6CA0
bool spiderman_rvb_air_jump_attack::attack_possible(const player_interface* plr_ifc) {
    if (plr_ifc->get_mode() == 9)
        return true;

    if (plr_ifc->get_next_attack_enum() == combat_num_moves)
        return false;

    actor* owner = plr_ifc->my_actor;

    if (!owner->has_standing_ifc())
        return false;

    return owner->standing_ifc()->is_effectively_standing() || owner->standing_ifc()->is_adv_standing();
}

// ==== spiderman_rvb_car_finish_attack ==== //

string_hash spiderman_rvb_car_finish_attack::my_type_key { 0xC4B1CF98 }; // "spiderman_rvb_car_finish_attack"

// inlined @ sub_9B3DC0
spiderman_rvb_car_finish_attack::spiderman_rvb_car_finish_attack() {
    initialize(mash::ALLOCATED);
}

// sub_71C940
spiderman_rvb_car_finish_attack::~spiderman_rvb_car_finish_attack() {}

// sub_9A7590
void spiderman_rvb_car_finish_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();

    initialize(mash::FROM_MASH);
}

// sub_99CD70
void spiderman_rvb_car_finish_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_car_finish_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71C930
mash::virtual_types_key spiderman_rvb_car_finish_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_car_finish_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4E1E90
i32 spiderman_rvb_car_finish_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9B6D10
bool spiderman_rvb_car_finish_attack::update(e_combat_move            move,
                                             const player_interface*  plr_ifc,
                                             als_inode*               my_als,
                                             f32                      delta_t,
                                             f32*                     best_time,
                                             e_combat_move            current_chosen_move,
                                             const plr_combat_attack* current_chosen_attack,
                                             const void*              unk) {

    auto* car = (entity_base*)plr_ifc->get_combat_vehicle().resolve();

    // the player has to be the vehicle's car-combat attacker, and the vehicle the first target
    if (car                                    &&
        car->is_a_vehicle()                    &&
        car->occupant_ifc()->unk_044.resolve() &&
        car->occupant_ifc()->unk_044.resolve() == plr_ifc->my_actor) {

        f32  own_best = -1.0f;
        bool found    = plr_combat_attack::update(move,
                                                  plr_ifc,
                                                  my_als,
                                                  delta_t,
                                                  &own_best,
                                                  current_chosen_move,
                                                  current_chosen_attack,
                                                  unk);

        if (found                  &&
            has_targets()          &&
            own_best >= *best_time &&
            targets->front().target == plr_ifc->get_combat_vehicle()) {

            *best_time = own_best;

            return true;
        }
    }

    clear();

    return false;
}

// sub_9B6E30
void spiderman_rvb_car_finish_attack::clear() {
    combat_vehicle = arch_base_vhandle();

    plr_combat_attack::clear();
}

// sub_9C2C30
bool spiderman_rvb_car_finish_attack::build_target_list(const player_interface* plr_ifc, const void*, bool store_targets, bool) {
    targets->clear();

    auto* car = (entity_base*)plr_ifc->get_combat_vehicle().resolve();

    if (!car || !car->is_a_vehicle())
        return false;

    // the player has to be the vehicle's car-combat attacker
    if (!car->occupant_ifc()->unk_044.resolve() ||
        car->occupant_ifc()->unk_044.resolve() != plr_ifc->my_actor)

        return false;

    if (!car->is_a_vehicle() || !car->occupant_ifc()->is_seat_occupied(0))
        return false;

    i32 condition = ((vehicle*)car)->condition;

    if (condition == 1 || condition == 3)
        return false;

    // the vehicle, then whoever's in seat 0
    if (store_targets) {
        combat_vehicle = plr_ifc->get_combat_vehicle();

        targets->push_back(ai_target(combat_vehicle));
        targets->push_back(ai_target(car->occupant_ifc()->m_seat_occupants[0]));
    }

    return true;
}

// sub_9B6E40
bool spiderman_rvb_car_finish_attack::target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) {
    auto* car = (entity_base*)combat_vehicle.resolve();

    if (!car || !car->is_a_vehicle())
        return false;

    if (tmp_target->target != combat_vehicle)
        return false;

    // the player has to be the vehicle's car-combat attacker
    if (!car->occupant_ifc()->unk_044.resolve() ||
        car->occupant_ifc()->unk_044.resolve() != plr_ifc->my_actor)

        return false;

    return car->occupant_ifc()->is_seat_occupied(0);
}

// inlined @ sub_9A7590
// inlined @ sub_9B3DC0
void spiderman_rvb_car_finish_attack::initialize(mash::allocation_scope) {
    combat_vehicle = arch_base_vhandle();
}

// ==== spiderman_rvb_throw_manip_attack ==== //

string_hash spiderman_rvb_throw_manip_attack::my_type_key { 0x26EF210A }; // "spiderman_rvb_throw_manip_attack"

// inlined @ sub_9B3DC0
spiderman_rvb_throw_manip_attack::spiderman_rvb_throw_manip_attack() {
    initialize(mash::ALLOCATED);
}

// sub_71C9D0
spiderman_rvb_throw_manip_attack::~spiderman_rvb_throw_manip_attack() {}

// sub_9A7590
void spiderman_rvb_throw_manip_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();

    initialize(mash::FROM_MASH);
}

// sub_99CD70
void spiderman_rvb_throw_manip_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_throw_manip_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71C9C0
mash::virtual_types_key spiderman_rvb_throw_manip_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_throw_manip_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_4E1E90
i32 spiderman_rvb_throw_manip_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9B6F00
bool spiderman_rvb_throw_manip_attack::update(e_combat_move            move,
                                              const player_interface*  plr_ifc,
                                              als_inode*               my_als,
                                              f32                      delta_t,
                                              f32*                     best_time,
                                              e_combat_move            current_chosen_move,
                                              const plr_combat_attack* current_chosen_attack,
                                              const void*              unk) {

    // against the light ranged attack only a press of its own, with targets, wins
    if (current_chosen_move == combat_move_light_ranged) {
        if (update_against_own_best(move,
                                    plr_ifc,
                                    my_als,
                                    delta_t,
                                    best_time,
                                    current_chosen_move,
                                    current_chosen_attack,
                                    unk,
                                    true))

            return true;

        clear();

        return false;
    }

    if (current_chosen_move == combat_num_moves || current_chosen_move == combat_move_throw_manip)
        return plr_combat_attack::update(move,
                                         plr_ifc,
                                         my_als,
                                         delta_t,
                                         best_time,
                                         current_chosen_move,
                                         current_chosen_attack,
                                         unk);

    clear();

    return false;
}

// sub_9B6E30
void spiderman_rvb_throw_manip_attack::clear() {
    manip_to_throw = arch_base_vhandle();

    plr_combat_attack::clear();
}

// sub_9C2D60
bool spiderman_rvb_throw_manip_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    ai_target manip_result;

    actor*          owner = plr_ifc->my_actor;
    const vector3d &pos   = owner->my_abs_po->get_position();
    vector3d        dir   = get_attack_direction(plr_ifc, unk);

    auto find_best_ready_manip_obj = (manip_obj_interface* (__cdecl*)(actor*, const vector3d*, const vector3d*, f32, f32, f32, f32, f32, f32, void*, ai_target*))retail::sub_63FAA0;

    manip_obj_interface* ifc = find_best_ready_manip_obj(owner,
                                                         &pos,
                                                         &dir,
                                                         0.0f,
                                                         4.0f,
                                                         -3.0f,
                                                         3.0f,
                                                         -1.1f,
                                                         -1.1f,
                                                         nullptr,
                                                         &manip_result);

    if (!ifc)
        return false;

    if (!store_targets)
        return true;

    manip_to_throw = ifc->my_actor->my_handle;

    // the object's collisions are off for the base build
    bool col_active = (ifc->my_actor->flags & entity_flag_collisions_active) != 0;

    ifc->my_actor->set_collisions_active(false, true);

    targets->clear();

    plr_combat_attack::build_target_list(plr_ifc, unk, store_targets, check_possible);

    ifc->my_actor->set_collisions_active(col_active, true);

    for (ai_target* targ = targets->begin(); targ != targets->end();) {
        if (targ->target == manip_to_throw)
            targ = targets->erase(targ);
        else
            ++targ;
    }

    // the object first, then the base build's best
    if (!targets->empty()) {
        ai_target throw_targ = targets->front();

        targets->clear();
        targets->push_back(manip_result);
        targets->push_back(throw_targ);
    } else {
        targets->clear();
        targets->push_back(manip_result);
    }

    return true;
}

// sub_9B6ED0
bool spiderman_rvb_throw_manip_attack::attack_possible(const player_interface* plr_ifc) {
    if (!plr_ifc->my_actor->standing_ifc()->is_effectively_standing())
        return false;

    return plr_combat_attack::attack_possible(plr_ifc);
}

// sub_9B6FF0
bool spiderman_rvb_throw_manip_attack::target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) {
    auto* manip = (entity_base*)manip_to_throw.resolve();

    if (!manip)
        return false;

    if (tmp_target->target != manip_to_throw)
        return plr_combat_attack::target_still_valid(tmp_target, plr_ifc);

    if (!manip->has_manip_obj_ifc())
        return false;

    if (manip->manip_obj_ifc()->is_active())
        return false;

    return manip->is_alive();
}

// inlined @ sub_9A7590
// inlined @ sub_9B3DC0
void spiderman_rvb_throw_manip_attack::initialize(mash::allocation_scope) {
    manip_to_throw = arch_base_vhandle();
}

// ==== spiderman_rvb_web_yank_attack ==== //

string_hash spiderman_rvb_web_yank_attack::my_type_key { 0xF604D552 }; // "spiderman_rvb_web_yank_attack"

// sub_71AFF0
spiderman_rvb_web_yank_attack::~spiderman_rvb_web_yank_attack() {}

// sub_99CD60
void spiderman_rvb_web_yank_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_web_yank_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_web_yank_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71AFE0
mash::virtual_types_key spiderman_rvb_web_yank_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_web_yank_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_web_yank_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9BF550
bool spiderman_rvb_web_yank_attack::update(e_combat_move            move,
                                           const player_interface*  plr_ifc,
                                           als_inode*               my_als,
                                           f32                      delta_t,
                                           f32*                     best_time,
                                           e_combat_move            current_chosen_move,
                                           const plr_combat_attack* current_chosen_attack,
                                           const void*              unk) {

    // outside combat the button needs a fresh press
    if (plr_ifc->get_mode() != 9 && (!button || !button->triggered_or_missed_press()))
        return false;

    bool found = plr_combat_attack::update(move,
                                           plr_ifc,
                                           my_als,
                                           delta_t,
                                           best_time,
                                           current_chosen_move,
                                           current_chosen_attack,
                                           unk);

    // a miss can still win over the grapple or the throw it's up against
    if (found                                                                                          ||
        (current_chosen_move != combat_move_grapple && current_chosen_move != combat_move_throw_manip) ||
        !current_chosen_attack                                                                         ||
        !((f64)timer > (f64)*best_time - (f64)0.01f)                                                   ||
        !has_targets()                                                                                 ||
        !current_chosen_attack->targets                                                                ||
        current_chosen_attack->targets->empty())

        return found;

    auto* current_best = (entity_base*)current_chosen_attack->targets->front().target.resolve();
    auto* my_best      = (entity_base*)targets->front().target.resolve();

    if (current_best && current_best->has_damage_ifc())
        current_best->damage_ifc()->flags &= ~damage_ifc_flag_die_on_fall_damage;

    if (my_best && my_best->has_damage_ifc())
        my_best->damage_ifc()->flags &= ~damage_ifc_flag_die_on_fall_damage;

    bool take = false;

    if (!current_best && my_best)
        take = true;
    else if (current_best && my_best) {
        actor*   owner         = plr_ifc->my_actor;
        vector3d current_delta = current_best->my_abs_po->get_position() - owner->my_abs_po->get_position();

        // the current one's well above or below, or both picked the same object
        if (!owner->standing_ifc()->is_effectively_standing() && std::fabs(current_delta.y) > 2.0f)
            take = true;
        else if (current_best == my_best && my_best->has_manip_obj_ifc())
            take = true;
        else {
            vector3d dir = get_attack_direction(plr_ifc, unk);

            f32      current_length = current_delta.length_float();
            vector3d current_unit   = dir;

            if (!((f64)current_length <= (f64)0.0001f)) {
                current_unit  = current_delta;
                current_unit *= 1.0f / current_length;
            }

            // the current one's behind, this one's in front and far enough
            if (dir * current_unit < 0.0f) {
                vector3d my_delta  = my_best->my_abs_po->get_position() - owner->my_abs_po->get_position();
                f32      my_length = my_delta.length_float();
                vector3d my_unit   = dir;

                if (!((f64)my_length <= (f64)0.0001f)) {
                    my_unit  = my_delta;
                    my_unit *= 1.0f / my_length;
                }

                if (dir * my_unit > 0.0f && (my_length >= 5.0f || move == combat_move_grab_civ))
                    take = true;
            }
        }
    }

    if (!take)
        return found;

    *best_time = timer;

    return true;
}

// sub_9C2F50
bool spiderman_rvb_web_yank_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    bool found = false;

    if (store_targets)
        targets->clear();

    bool black_mode  = plr_ifc->is_in_rvb_black_mode();
    f32  cos_pi      = math::cos(math::math_pi);
    f32  cos_half_pi = math::cos(math::math_pi / 2.0f);

    auto owned = [](const char* upgrade) {
        string_hash name;
        name.initialize(mash::ALLOCATED, upgrade);

        i32 id = retail::sub_9210B0((u32*)references::upgrade_manager.read(), (u32*)&name); // get_upgrade_id

        return id >= 0 && retail::sub_9219C0((u32*)references::upgrade_manager.read(), id); // is_upgrade_owned
    };

    f32 reach;  // the zlock entity's
    f32 radius; // everyone else's

    if (black_mode) {
        if (owned("ICHOR_PULL_3")) {
            reach  = 54.0f;
            radius = 50.0f;
        } else if (owned("ICHOR_PULL_2")) {
            reach  = 44.0f;
            radius = 40.0f;
        } else if (owned("ICHOR_PULL_1")) {
            reach  = 37.0f;
            radius = 30.0f;
        } else {
            reach  = 18.0f;
            radius = 15.0f;
        }
    } else {
        if (owned("WEB_UPGRADE_RANGE_3"))
            radius = 50.0f;
        else if (owned("WEB_UPGRADE_RANGE_2"))
            radius = 40.0f;
        else if (owned("WEB_UPGRADE_RANGE_1"))
            radius = 30.0f;
        else
            radius = 15.0f;

        reach = radius;
    }

    if (check_possible && !attack_possible(plr_ifc))
        return found;

    vector3d pos = plr_ifc->my_actor->my_abs_po->get_position();

    attack_dir = get_attack_direction(plr_ifc, unk);
    attack_up  = get_attack_up(plr_ifc, unk);

    vc8::vector<ai_target> found_targets;
    ai_target              candidate;

    arch_base_vhandle zlock = plr_ifc->unk_048();

    // in black mode a hostless symbiote is never picked
    bool zlock_taken = false;

    if (zlock.resolve() && (!black_mode || retail::sub_40F4A0((i32)zlock.resolve()))) {
        vector3d delta = ((entity_base*)zlock.resolve())->my_abs_po->get_position() - pos;

        if ((f64)reach * reach > delta.length2()) {
            delta.normalize();

            f32 dotp = attack_dir * delta;

            zlock_taken = dotp >= cos_pi;
        }
    }

    if (zlock_taken) {
        found = true;

        if (!store_targets)
            return found;

        candidate.set_target(zlock);
        candidate.score = 1.0f;

        found_targets.push_back(candidate);
    } else {
        // the cone opens up to the radius and stays that way
        min_range      = 0.0f;
        max_range      = radius;
        max_vert_range = radius;
        min_vert_range = -radius;
        min_dotp       = cos_half_pi;

        engine_lock_scope list_lock(&references::ai_core_list_lock.get());

        u32 search_flags = ((target_flags & ai_target::target_allow_dead_target) | 0x81800) >> 11; // alive, visible and 0x100, plus allow dead

        ai_core_list* list = &references::ai_core_list_high.get();

        if (list->count)
            for (ai_core_list_node* node = list->head; node; node = node->next) {
                if (!node->core->get_actor()->has_ifc(entity_ifc_damage))
                    continue;

                const vector3d &owner_pos = plr_ifc->my_actor->my_abs_po->get_position();

                if (!((bool (__thiscall*)(damage_interface*, const vector3d*, f32, u32))retail::sub_621270)(node->core->get_actor()->damage_ifc(),
                                                                                                       &owner_pos,
                                                                                                       max_range,
                                                                                                       search_flags))

                    continue;

                if (black_mode && !retail::sub_40F4A0((i32)node->core->get_actor()))
                    continue;

                candidate.set_target(node->core->get_actor()->my_handle);

                f32 score = compute_target_score(&candidate, plr_ifc, 0);

                if (!((f64)score > (f64)0.0001f))
                    continue;

                candidate.score = score;
                found           = true;

                if (!store_targets)
                    break;

                found_targets.push_back(candidate);
            }
    }

    if (store_targets && found) {
        if (found_targets.size() > 1)
            std::qsort(found_targets.begin(),
                       found_targets.size(),
                       sizeof(ai_target),
                       ai_target::compare_score);

        for (const ai_target &targ : found_targets)
            targets->push_back(targ);
    }

    return found;
}

// sub_9A6CC0
bool spiderman_rvb_web_yank_attack::attack_possible(const player_interface* plr_ifc) {
    return plr_combat_attack::attack_possible(plr_ifc);
}

// ==== spiderman_rvb_melee_or_car_combat_attack ==== //

string_hash spiderman_rvb_melee_or_car_combat_attack::my_type_key { 0x15549B34 }; // "spiderman_rvb_melee_or_car_combat_attack"

// sub_71B060
spiderman_rvb_melee_or_car_combat_attack::~spiderman_rvb_melee_or_car_combat_attack() {}

// sub_99CD60
void spiderman_rvb_melee_or_car_combat_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_melee_or_car_combat_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_melee_or_car_combat_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B050
mash::virtual_types_key spiderman_rvb_melee_or_car_combat_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_melee_or_car_combat_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_melee_or_car_combat_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9B7220
bool spiderman_rvb_melee_or_car_combat_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    auto* car = (entity_base*)plr_ifc->get_combat_vehicle().resolve();

    // nothing while the player is the vehicle's car-combat attacker and its driver is alone in it
    if (car                                                          &&
        car->occupant_ifc()->unk_044.resolve()                       &&
        car->occupant_ifc()->unk_044.resolve() == plr_ifc->my_actor  &&
        car->occupant_ifc()->m_vehicle_occupant_count == 1           &&
        car->occupant_ifc()->m_seat_occupants[0].resolve()) {

        clear();

        return false;
    }

    return plr_combat_attack::build_target_list(plr_ifc, unk, store_targets, check_possible);
}

// sub_9A6CF0
bool spiderman_rvb_melee_or_car_combat_attack::attack_possible(const player_interface* plr_ifc) {
    auto* car = (entity_base*)plr_ifc->get_combat_vehicle().resolve();

    if (car && car->is_a_vehicle() && ((vehicle*)car)->condition)
        return false;

    return plr_combat_attack::attack_possible(plr_ifc);
}

// sub_9B72D0
f32 spiderman_rvb_melee_or_car_combat_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    u32 flags = flag_override ? flag_override : target_flags;

    auto* car = (entity_base*)plr_ifc->get_combat_vehicle().resolve();

    if (!car ||
        !car->occupant_ifc()->unk_044.resolve() ||
        car->occupant_ifc()->unk_044.resolve() != plr_ifc->my_actor)

        return plr_combat_attack::compute_target_score(tmp_target, plr_ifc, flags);

    // the player is its vehicle's car-combat attacker, so only someone else riding in it scores
    auto* ent = (entity_base*)tmp_target->target.resolve();

    if (!ent || !ent->is_an_actor() || !((actor*)ent)->get_ai_core())
        return -1.0f;

    auto* car_node = (ai_car_inode*)((actor*)ent)->get_ai_core()->get_info_node(info_node_type_ai_car);

    if (!car_node                               ||
        !car_node->is_in_car                    ||
        car_node->selected_car.resolve() != car ||
        !car_node->selected_seat)

        return -1.0f;

    actor* owner = plr_ifc->my_actor;

    return tmp_target->compute_score(owner,
                                     owner->my_abs_po->get_position(),
                                     attack_dir,
                                     attack_up,
                                     0.0f,
                                     20.0f,
                                     -20.0f,
                                     20.0f,
                                     min_dotp,
                                     min_vert_dotp,
                                     (flags & ~0x400C0) | 0x20000,
                                     nullptr,
                                     0,
                                     0,
                                     0x14);
}

// sub_9B7450
bool spiderman_rvb_melee_or_car_combat_attack::target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) {
    auto* car = (entity_base*)plr_ifc->get_combat_vehicle().resolve();

    if (!car ||
        !car->occupant_ifc()->unk_044.resolve() ||
        car->occupant_ifc()->unk_044.resolve() != plr_ifc->my_actor)

        return plr_combat_attack::target_still_valid(tmp_target, plr_ifc);

    u32 flags = (target_flags & ~0x400C0) | 0x20000;

    // the player is its vehicle's car-combat attacker, so a target outside the vehicle stays valid
    auto* ent = (entity_base*)tmp_target->target.resolve();

    if (!ent || !ent->is_an_actor() || !((actor*)ent)->get_ai_core())
        return true;

    auto* car_node = (ai_car_inode*)((actor*)ent)->get_ai_core()->get_info_node(info_node_type_ai_car);

    if (!car_node                               ||
        !car_node->is_in_car                    ||
        car_node->selected_car.resolve() != car ||
        !car_node->selected_seat)

        return true;

    return tmp_target->target_valid(plr_ifc->my_actor, flags);
}

// ==== spiderman_rvb_momentum_attack ==== //

string_hash spiderman_rvb_momentum_attack::my_type_key { 0x0E3E2154 }; // "spiderman_rvb_momentum_attack"

// sub_71B0D0
spiderman_rvb_momentum_attack::~spiderman_rvb_momentum_attack() {}

// sub_99CD60
void spiderman_rvb_momentum_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_momentum_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_momentum_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B0C0
mash::virtual_types_key spiderman_rvb_momentum_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_momentum_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_momentum_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9BA970
bool spiderman_rvb_momentum_attack::has_potential_targets(const player_interface* plr_ifc, bool check_possible) {
    if (check_possible && !attack_possible(plr_ifc))
        return false;

    return !plr_ifc->unk_040().null();
}

// sub_9C3820
bool spiderman_rvb_momentum_attack::build_target_list(const player_interface* plr_ifc, const void*, bool store_targets, bool check_possible) {
    if (!store_targets)
        return has_potential_targets(plr_ifc, check_possible);

    targets->clear();

    if (check_possible && !attack_possible(plr_ifc))
        return false;

    arch_base_vhandle target = plr_ifc->unk_040();

    if (!target.resolve())
        return false;

    push_target(target);

    return true;
}

// sub_9B7610
bool spiderman_rvb_momentum_attack::attack_possible(const player_interface* plr_ifc) {
    if (!plr_combat_attack::attack_possible(plr_ifc))
        return false;

    if (!plr_ifc->unk_029())
        return false;

    i32 mode = plr_ifc->get_mode();

    if (mode != 9 && mode != 4 && mode != 5 && mode != 3 && mode != 14 && mode != 1)
        return false;

    return has_potential_targets(plr_ifc, false);
}

// sub_9A6CD0
f32 spiderman_rvb_momentum_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    return plr_combat_attack::compute_target_score(tmp_target, plr_ifc, flag_override ? flag_override : target_flags);
}

// ==== spiderman_rvb_civ_save_attack ==== //

string_hash spiderman_rvb_civ_save_attack::my_type_key { 0x4F0A3212 }; // "spiderman_rvb_civ_save_attack"

// sub_71B140
spiderman_rvb_civ_save_attack::~spiderman_rvb_civ_save_attack() {}

// sub_99CD60
void spiderman_rvb_civ_save_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_civ_save_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_civ_save_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B130
mash::virtual_types_key spiderman_rvb_civ_save_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_civ_save_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_civ_save_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9BA970
bool spiderman_rvb_civ_save_attack::has_potential_targets(const player_interface* plr_ifc, bool check_possible) {
    if (check_possible && !attack_possible(plr_ifc))
        return false;

    return !plr_ifc->unk_040().null();
}

// sub_9C36C0
bool spiderman_rvb_civ_save_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    if (!store_targets)
        return has_potential_targets(plr_ifc, check_possible);

    targets->clear();

    if (check_possible && !attack_possible(plr_ifc))
        return plr_combat_attack::build_target_list(plr_ifc, unk, store_targets, check_possible);

    arch_base_vhandle civ = plr_ifc->unk_040();

    // only the zlocked civilian, when it can be picked up
    if (civ.null()                                    ||
        !civ.resolve()                                ||
        !((entity_base*)civ.resolve())->is_an_actor() ||
        ((entity_base*)civ.resolve())->is_a_vehicle() ||
        civ != plr_ifc->unk_048())

        return plr_combat_attack::build_target_list(plr_ifc, unk, store_targets, check_possible);

    auto* carry = (std_carry_inode*)((actor*)civ.resolve())->get_ai_core()->get_info_node(info_node_type_std_carry);

    if (!carry || !carry->get_allow_pickup())
        return plr_combat_attack::build_target_list(plr_ifc, unk, store_targets, check_possible);

    push_target(civ);

    return true;
}

// sub_9B7070
bool spiderman_rvb_civ_save_attack::attack_possible(const player_interface* plr_ifc) {
    if (!plr_combat_attack::attack_possible(plr_ifc))
        return false;

    if (plr_ifc->get_mode() == 3 || plr_ifc->unk_0251)
        return false;

    return has_potential_targets(plr_ifc, false);
}

// sub_9A6CD0
f32 spiderman_rvb_civ_save_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    return plr_combat_attack::compute_target_score(tmp_target, plr_ifc, flag_override ? flag_override : target_flags);
}

// ==== spiderman_rvb_wall_attack ==== //

string_hash spiderman_rvb_wall_attack::my_type_key { 0x2AB57492 }; // "spiderman_rvb_wall_attack"

// sub_71B1B0
spiderman_rvb_wall_attack::~spiderman_rvb_wall_attack() {}

// sub_99CD60
void spiderman_rvb_wall_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_wall_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_wall_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B1A0
mash::virtual_types_key spiderman_rvb_wall_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_wall_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_71CEB0
i32 spiderman_rvb_wall_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9B70C0
bool spiderman_rvb_wall_attack::update(e_combat_move            move,
                                       const player_interface*  plr_ifc,
                                       als_inode*               my_als,
                                       f32                      delta_t,
                                       f32*                     best_time,
                                       e_combat_move            current_chosen_move,
                                       const plr_combat_attack* current_chosen_attack,
                                       const void*              unk) {

    if (!update_against_own_best(move,
                                 plr_ifc,
                                 my_als,
                                 delta_t,
                                 best_time,
                                 current_chosen_move,
                                 current_chosen_attack,
                                 unk,
                                 false))

        return false;

    actor*          owner = plr_ifc->my_actor;
    const vector3d &pos   = owner->my_abs_po->get_position();
    const vector3d &up    = references::unk_00f4dc6c.get();

    math::vector4 facing_dir(-unk_58.x, -unk_58.y, -unk_58.z, -0.0f);
    math::vector4 facing_up(up.x, up.y, up.z, 0.0f);
    math::vector4 facing_pos(pos.x, pos.y, pos.z, 0.0f);

    po facing;
    facing.set_po(facing_dir, facing_up, facing_pos);

    retail::sub_627180((i32)owner, (f32*)&facing); // entity_base::set_abs_po

    return true;
}

// sub_71FF10
bool spiderman_rvb_wall_attack::attack_possible(const player_interface*) {
    return false;
}

// ==== spiderman_rvb_pole_swing_attack ==== //

string_hash spiderman_rvb_pole_swing_attack::my_type_key { 0xAFF9DBD9 }; // "spiderman_rvb_pole_swing_attack"

// sub_71B220
spiderman_rvb_pole_swing_attack::~spiderman_rvb_pole_swing_attack() {}

// sub_99CD60
void spiderman_rvb_pole_swing_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_pole_swing_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_pole_swing_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B210
mash::virtual_types_key spiderman_rvb_pole_swing_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_pole_swing_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_pole_swing_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9A7470
bool spiderman_rvb_pole_swing_attack::update(e_combat_move            move,
                                             const player_interface*  plr_ifc,
                                             als_inode*               my_als,
                                             f32                      delta_t,
                                             f32*                     best_time,
                                             e_combat_move            current_chosen_move,
                                             const plr_combat_attack* current_chosen_attack,
                                             const void*              unk) {

    if (!update_against_own_best(move,
                                 plr_ifc,
                                 my_als,
                                 delta_t,
                                 best_time,
                                 current_chosen_move,
                                 current_chosen_attack,
                                 unk,
                                 false))

        return false;

    f32 orientation = (f32)((player_interface*)plr_ifc)->unk_388();

    my_als->my_als->get_als_layer(als::base_layer)->set_desired_param(als::param_pole_swing_orientation, orientation);

    return true;
}

// sub_9BAB10
vector3d spiderman_rvb_pole_swing_attack::get_attack_direction(const player_interface* plr_ifc, const void* unk) {
    vector3d dir = math::unitize(plr_ifc->my_actor->my_abs_po->get_z_facing());
    vector3d up  = get_attack_up(plr_ifc, unk);

    if (std::fabs(up * dir) >= 0.9999f)
        dir = math::unitize(plr_ifc->my_actor->my_abs_po->get_y_facing());

    const vector3d &zero = references::unk_01112ca0.get();

    math::vector4 basis_dir(dir.x, dir.y, dir.z, 0.0f);
    math::vector4 basis_up(up.x, up.y, up.z, 0.0f);
    math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

    po basis;
    basis.set_po(basis_dir, basis_up, basis_origin);

    return basis.get_z_facing();
}

// sub_9BAA80
bool spiderman_rvb_pole_swing_attack::attack_possible(const player_interface* plr_ifc) {
    if (plr_ifc->unk_076() != 0 || plr_ifc->unk_077() != 1)
        return false;

    if (!plr_combat_attack::attack_possible(plr_ifc))
        return false;

    i32 mode = plr_ifc->get_mode();

    if (mode != 9 && mode != 4 && mode != 5 && mode != 14)
        return false;

    if (!has_potential_targets(plr_ifc, false))
        return false;

    return ((player_interface*)plr_ifc)->unk_386(0.25f, true);
}

// ==== spiderman_rvb_swing_attack ==== //

string_hash spiderman_rvb_swing_attack::my_type_key { 0xAC39DF2A }; // "spiderman_rvb_swing_attack"

// sub_71B290
spiderman_rvb_swing_attack::~spiderman_rvb_swing_attack() {}

// sub_99CD60
void spiderman_rvb_swing_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_swing_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_swing_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B280
mash::virtual_types_key spiderman_rvb_swing_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_swing_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_swing_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9BA9B0
bool spiderman_rvb_swing_attack::attack_possible(const player_interface* plr_ifc) {
    if (!plr_combat_attack::attack_possible(plr_ifc))
        return false;

    actor* owner = plr_ifc->my_actor;

    if (owner->has_physical_ifc() && 3.0f > owner->physical_ifc()->get_velocity().length())
        return false;

    i32 mode = plr_ifc->get_mode();

    if (mode == 3)
        return true;

    if (mode != 9)
        return false;

    return plr_ifc->unk_0251;
}

// ==== spiderman_rvb_melee_or_car_combat_ground_attack ==== //

string_hash spiderman_rvb_melee_or_car_combat_ground_attack::my_type_key { 0x5F901D02 }; // "spiderman_rvb_melee_or_car_combat_ground_attack"

// sub_71B300
spiderman_rvb_melee_or_car_combat_ground_attack::~spiderman_rvb_melee_or_car_combat_ground_attack() {}

// sub_99CD60
void spiderman_rvb_melee_or_car_combat_ground_attack::construct_mashed_class() {
    spiderman_rvb_melee_or_car_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_melee_or_car_combat_ground_attack::destruct_mashed_class() {
    spiderman_rvb_melee_or_car_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_melee_or_car_combat_ground_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    spiderman_rvb_melee_or_car_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B2F0
mash::virtual_types_key spiderman_rvb_melee_or_car_combat_ground_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71B370
bool spiderman_rvb_melee_or_car_combat_ground_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (spiderman_rvb_melee_or_car_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (spiderman_rvb_melee_or_car_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_melee_or_car_combat_ground_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9B7560
bool spiderman_rvb_melee_or_car_combat_ground_attack::attack_possible(const player_interface* plr_ifc) {
    if (!spiderman_rvb_melee_or_car_combat_attack::attack_possible(plr_ifc))
        return false;

    auto* car = (entity_base*)plr_ifc->get_combat_vehicle().resolve();

    // the player is its vehicle's car-combat attacker
    if (car &&
        car->occupant_ifc()->unk_044.resolve() &&
        car->occupant_ifc()->unk_044.resolve() == plr_ifc->my_actor)

        return true;

    actor* owner = plr_ifc->my_actor;

    return owner->has_standing_ifc() && owner->standing_ifc()->is_effectively_standing();
}

// ==== spiderman_rvb_special_attack ==== //

string_hash spiderman_rvb_special_attack::my_type_key { 0x943600A3 }; // "spiderman_rvb_special_attack"

// sub_71B3A0
spiderman_rvb_special_attack::~spiderman_rvb_special_attack() {}

// sub_99CD60
void spiderman_rvb_special_attack::construct_mashed_class() {
    spiderman_rvb_melee_or_car_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_special_attack::destruct_mashed_class() {
    spiderman_rvb_melee_or_car_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_special_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    spiderman_rvb_melee_or_car_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B360
mash::virtual_types_key spiderman_rvb_special_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71B370
bool spiderman_rvb_special_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (spiderman_rvb_melee_or_car_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (spiderman_rvb_melee_or_car_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_special_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9BAD50
bool spiderman_rvb_special_attack::attack_possible(const player_interface* plr_ifc) {
    if (plr_ifc->get_mode() == 3)
        return false;

    if (!(retail::sub_933A90((i32*)references::game_meter_manager.read(), 0) >= 300.0f)) // meter 0's level
        return false;

    return spiderman_rvb_melee_or_car_combat_attack::attack_possible(plr_ifc);
}

// inlined @ sub_9BAD90
// inlined @ sub_9BAE90
// inlined @ sub_9BAF90
bool spiderman_rvb_special_attack::attack_possible_with_upgrades(const player_interface* plr_ifc, const char* black_upgrade, const char* red_upgrade) {
    string_hash name;

    name.initialize(mash::ALLOCATED, black_upgrade);
    i32 black_id = retail::sub_9210B0((u32*)references::upgrade_manager.read(), (u32*)&name); // get_upgrade_id

    name.initialize(mash::ALLOCATED, red_upgrade);
    i32 red_id = retail::sub_9210B0((u32*)references::upgrade_manager.read(), (u32*)&name);

    bool black_owned = black_id >= 0 && retail::sub_9219C0((u32*)references::upgrade_manager.read(), black_id); // is_upgrade_owned
    bool red_owned   = red_id   >= 0 && retail::sub_9219C0((u32*)references::upgrade_manager.read(), red_id);

    if (!(plr_ifc->is_in_rvb_black_mode() && black_owned) &&
        (plr_ifc->is_in_rvb_black_mode() || !red_owned))

        return false;

    if (plr_ifc->get_mode() == 3)
        return false;

    if (!(retail::sub_933A90((i32*)references::game_meter_manager.read(), 0) >= 300.0f))
        return false;

    return spiderman_rvb_melee_or_car_combat_attack::attack_possible(plr_ifc);
}

// ==== spiderman_rvb_special_x_attack ==== //

string_hash spiderman_rvb_special_x_attack::my_type_key { 0x382ADC7A }; // "spiderman_rvb_special_x_attack"

// sub_71B450
spiderman_rvb_special_x_attack::~spiderman_rvb_special_x_attack() {}

// sub_99CD60
void spiderman_rvb_special_x_attack::construct_mashed_class() {
    spiderman_rvb_special_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_special_x_attack::destruct_mashed_class() {
    spiderman_rvb_special_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_special_x_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    spiderman_rvb_special_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B400
mash::virtual_types_key spiderman_rvb_special_x_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71B410
bool spiderman_rvb_special_x_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (spiderman_rvb_special_attack::get_virtual_type_key() == parent_class)
        return true;

    if (spiderman_rvb_special_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_special_x_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9BAD90
bool spiderman_rvb_special_x_attack::attack_possible(const player_interface* plr_ifc) {
    return attack_possible_with_upgrades(plr_ifc, "GROUND_POUND_1", "RISING_WEB_1");
}

// ==== spiderman_rvb_special_y_attack ==== //

string_hash spiderman_rvb_special_y_attack::my_type_key { 0x246CB15B }; // "spiderman_rvb_special_y_attack"

// sub_71B4C0
spiderman_rvb_special_y_attack::~spiderman_rvb_special_y_attack() {}

// sub_99CD60
void spiderman_rvb_special_y_attack::construct_mashed_class() {
    spiderman_rvb_special_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_special_y_attack::destruct_mashed_class() {
    spiderman_rvb_special_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_special_y_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    spiderman_rvb_special_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B4B0
mash::virtual_types_key spiderman_rvb_special_y_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71B410
bool spiderman_rvb_special_y_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (spiderman_rvb_special_attack::get_virtual_type_key() == parent_class)
        return true;

    if (spiderman_rvb_special_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_special_y_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9BAE90
bool spiderman_rvb_special_y_attack::attack_possible(const player_interface* plr_ifc) {
    return attack_possible_with_upgrades(plr_ifc, "BEATDOWN_1", "MULTI_FURY_1");
}

// ==== spiderman_rvb_special_b_attack ==== //

string_hash spiderman_rvb_special_b_attack::my_type_key { 0xEA829124 }; // "spiderman_rvb_special_b_attack"

// sub_71B530
spiderman_rvb_special_b_attack::~spiderman_rvb_special_b_attack() {}

// sub_99CD60
void spiderman_rvb_special_b_attack::construct_mashed_class() {
    spiderman_rvb_special_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_special_b_attack::destruct_mashed_class() {
    spiderman_rvb_special_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_special_b_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    spiderman_rvb_special_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B520
mash::virtual_types_key spiderman_rvb_special_b_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71B410
bool spiderman_rvb_special_b_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (spiderman_rvb_special_attack::get_virtual_type_key() == parent_class)
        return true;

    if (spiderman_rvb_special_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_special_b_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9BAF90
bool spiderman_rvb_special_b_attack::attack_possible(const player_interface* plr_ifc) {
    return attack_possible_with_upgrades(plr_ifc, "ICHOR_GRASP_1", "WEB_BLIZZARD_1");
}

// ==== spiderman_rvb_counter_attack ==== //

string_hash spiderman_rvb_counter_attack::my_type_key { 0xE2F5FAA2 }; // "spiderman_rvb_counter_attack"

// sub_71B5A0
spiderman_rvb_counter_attack::~spiderman_rvb_counter_attack() {}

// sub_99CD60
void spiderman_rvb_counter_attack::construct_mashed_class() {
    plr_combat_attack::construct_mashed_class();
}

// sub_99CD70
void spiderman_rvb_counter_attack::destruct_mashed_class() {
    plr_combat_attack::destruct_mashed_class();
}

// sub_54B5E0
void spiderman_rvb_counter_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    plr_combat_attack::unmash(mash_info, containing_class_ptr, buffer);
}

// sub_71B590
mash::virtual_types_key spiderman_rvb_counter_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_71AEB0
bool spiderman_rvb_counter_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    if (plr_combat_attack::get_virtual_type_key() == parent_class)
        return true;

    if (plr_combat_attack::is_subclass_of(parent_class))
        return true;

    return false;
}

// sub_5AE580
i32 spiderman_rvb_counter_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9A7520
bool spiderman_rvb_counter_attack::update(e_combat_move            move,
                                          const player_interface*  plr_ifc,
                                          als_inode*               my_als,
                                          f32                      delta_t,
                                          f32*                     best_time,
                                          e_combat_move            current_chosen_move,
                                          const plr_combat_attack* current_chosen_attack,
                                          const void*              unk) {

    if (plr_ifc && plr_ifc->in_counter_attack_move && attack_possible(plr_ifc)) {
        *best_time = timer;

        return true;
    }

    return plr_combat_attack::update(move,
                                     plr_ifc,
                                     my_als,
                                     delta_t,
                                     best_time,
                                     current_chosen_move,
                                     current_chosen_attack,
                                     unk);
}

// sub_9C38E0
bool spiderman_rvb_counter_attack::build_target_list(const player_interface* plr_ifc, const void*, bool store_targets, bool check_possible) {
    if (!store_targets)
        return attack_possible(plr_ifc);

    targets->clear();

    if (check_possible && !attack_possible(plr_ifc))
        return false;

    arch_base_vhandle target = plr_ifc->unk_258();

    if (!target.resolve())
        return false;

    push_target(target);

    target = plr_ifc->unk_244();

    if (target.resolve())
        push_target(target);

    return true;
}

// sub_9BACF0
bool spiderman_rvb_counter_attack::attack_possible(const player_interface* plr_ifc) {
    if (!plr_combat_attack::attack_possible(plr_ifc))
        return false;

    if (plr_ifc->get_mode() != 19)
        return false;

    return !plr_ifc->unk_258().null() && plr_ifc->unk_258().resolve();
}
