#include <cmath>

#include "retail.hh"
#include "game/wds/entity/interface/spiderman_rvb_attacks.hh"
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
#include "game/game.hh"
#include "game/game_meter_manager.hh"
#include "game/math/po.hh"
#include "game/math/references.hh"

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

// slot 9
bool spiderman_rvb_grapple_attack::update(e_combat_move            move,
                                          const player_interface*  plr_ifc,
                                          als_inode*               my_als,
                                          f32                      delta_t,
                                          f32*                     best_time,
                                          e_combat_move            current_chosen_move,
                                          const plr_combat_attack* current_chosen_attack,
                                          const void*              unk) {

    return ((bool (__thiscall*)(spiderman_rvb_grapple_attack*, e_combat_move, const player_interface*, als_inode*, f32, f32*, e_combat_move, const plr_combat_attack*, const void*))retail::sub_9B6B90)(this,
                                                                                                                                                                                                        move,
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

// slot 9
bool spiderman_rvb_car_finish_attack::update(e_combat_move            move,
                                             const player_interface*  plr_ifc,
                                             als_inode*               my_als,
                                             f32                      delta_t,
                                             f32*                     best_time,
                                             e_combat_move            current_chosen_move,
                                             const plr_combat_attack* current_chosen_attack,
                                             const void*              unk) {

    return ((bool (__thiscall*)(spiderman_rvb_car_finish_attack*, e_combat_move, const player_interface*, als_inode*, f32, f32*, e_combat_move, const plr_combat_attack*, const void*))retail::sub_9B6D10)(this,
                                                                                                                                                                                                           move,
                                                                                                                                                                                                           plr_ifc,
                                                                                                                                                                                                           my_als,
                                                                                                                                                                                                           delta_t,
                                                                                                                                                                                                           best_time,
                                                                                                                                                                                                           current_chosen_move,
                                                                                                                                                                                                           current_chosen_attack,
                                                                                                                                                                                                           unk);
}

// sub_9B6E30
void spiderman_rvb_car_finish_attack::clear() {
    combat_vehicle = arch_base_vhandle();

    plr_combat_attack::clear();
}

// slot 12
bool spiderman_rvb_car_finish_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_car_finish_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C2C30)(this,
                                                                                                                                        plr_ifc,
                                                                                                                                        unk,
                                                                                                                                        store_targets,
                                                                                                                                        check_possible);
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

// slot 9
bool spiderman_rvb_throw_manip_attack::update(e_combat_move            move,
                                              const player_interface*  plr_ifc,
                                              als_inode*               my_als,
                                              f32                      delta_t,
                                              f32*                     best_time,
                                              e_combat_move            current_chosen_move,
                                              const plr_combat_attack* current_chosen_attack,
                                              const void*              unk) {

    return ((bool (__thiscall*)(spiderman_rvb_throw_manip_attack*, e_combat_move, const player_interface*, als_inode*, f32, f32*, e_combat_move, const plr_combat_attack*, const void*))retail::sub_9B6F00)(this,
                                                                                                                                                                                                            move,
                                                                                                                                                                                                            plr_ifc,
                                                                                                                                                                                                            my_als,
                                                                                                                                                                                                            delta_t,
                                                                                                                                                                                                            best_time,
                                                                                                                                                                                                            current_chosen_move,
                                                                                                                                                                                                            current_chosen_attack,
                                                                                                                                                                                                            unk);
}

// sub_9B6E30
void spiderman_rvb_throw_manip_attack::clear() {
    manip_to_throw = arch_base_vhandle();

    plr_combat_attack::clear();
}

// slot 12
bool spiderman_rvb_throw_manip_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_throw_manip_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C2D60)(this,
                                                                                                                                         plr_ifc,
                                                                                                                                         unk,
                                                                                                                                         store_targets,
                                                                                                                                         check_possible);
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

// slot 9
bool spiderman_rvb_web_yank_attack::update(e_combat_move            move,
                                           const player_interface*  plr_ifc,
                                           als_inode*               my_als,
                                           f32                      delta_t,
                                           f32*                     best_time,
                                           e_combat_move            current_chosen_move,
                                           const plr_combat_attack* current_chosen_attack,
                                           const void*              unk) {

    return ((bool (__thiscall*)(spiderman_rvb_web_yank_attack*, e_combat_move, const player_interface*, als_inode*, f32, f32*, e_combat_move, const plr_combat_attack*, const void*))retail::sub_9BF550)(this,
                                                                                                                                                                                                         move,
                                                                                                                                                                                                         plr_ifc,
                                                                                                                                                                                                         my_als,
                                                                                                                                                                                                         delta_t,
                                                                                                                                                                                                         best_time,
                                                                                                                                                                                                         current_chosen_move,
                                                                                                                                                                                                         current_chosen_attack,
                                                                                                                                                                                                         unk);
}

// slot 12
bool spiderman_rvb_web_yank_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_web_yank_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C2F50)(this,
                                                                                                                                      plr_ifc,
                                                                                                                                      unk,
                                                                                                                                      store_targets,
                                                                                                                                      check_possible);
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

// slot 12
bool spiderman_rvb_melee_or_car_combat_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_melee_or_car_combat_attack*, const player_interface*, const void*, bool, bool))retail::sub_9B7220)(this,
                                                                                                                                                 plr_ifc,
                                                                                                                                                 unk,
                                                                                                                                                 store_targets,
                                                                                                                                                 check_possible);
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

// slot 12
bool spiderman_rvb_momentum_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_momentum_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C3820)(this,
                                                                                                                                      plr_ifc,
                                                                                                                                      unk,
                                                                                                                                      store_targets,
                                                                                                                                      check_possible);
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

// slot 12
bool spiderman_rvb_civ_save_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_civ_save_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C36C0)(this,
                                                                                                                                      plr_ifc,
                                                                                                                                      unk,
                                                                                                                                      store_targets,
                                                                                                                                      check_possible);
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

// slot 9
bool spiderman_rvb_wall_attack::update(e_combat_move            move,
                                       const player_interface*  plr_ifc,
                                       als_inode*               my_als,
                                       f32                      delta_t,
                                       f32*                     best_time,
                                       e_combat_move            current_chosen_move,
                                       const plr_combat_attack* current_chosen_attack,
                                       const void*              unk) {

    return ((bool (__thiscall*)(spiderman_rvb_wall_attack*, e_combat_move, const player_interface*, als_inode*, f32, f32*, e_combat_move, const plr_combat_attack*, const void*))retail::sub_9B70C0)(this,
                                                                                                                                                                                                     move,
                                                                                                                                                                                                     plr_ifc,
                                                                                                                                                                                                     my_als,
                                                                                                                                                                                                     delta_t,
                                                                                                                                                                                                     best_time,
                                                                                                                                                                                                     current_chosen_move,
                                                                                                                                                                                                     current_chosen_attack,
                                                                                                                                                                                                     unk);
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

// slot 9
bool spiderman_rvb_pole_swing_attack::update(e_combat_move            move,
                                             const player_interface*  plr_ifc,
                                             als_inode*               my_als,
                                             f32                      delta_t,
                                             f32*                     best_time,
                                             e_combat_move            current_chosen_move,
                                             const plr_combat_attack* current_chosen_attack,
                                             const void*              unk) {

    return ((bool (__thiscall*)(spiderman_rvb_pole_swing_attack*, e_combat_move, const player_interface*, als_inode*, f32, f32*, e_combat_move, const plr_combat_attack*, const void*))retail::sub_9A7470)(this,
                                                                                                                                                                                                           move,
                                                                                                                                                                                                           plr_ifc,
                                                                                                                                                                                                           my_als,
                                                                                                                                                                                                           delta_t,
                                                                                                                                                                                                           best_time,
                                                                                                                                                                                                           current_chosen_move,
                                                                                                                                                                                                           current_chosen_attack,
                                                                                                                                                                                                           unk);
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

// slot 9
bool spiderman_rvb_counter_attack::update(e_combat_move            move,
                                          const player_interface*  plr_ifc,
                                          als_inode*               my_als,
                                          f32                      delta_t,
                                          f32*                     best_time,
                                          e_combat_move            current_chosen_move,
                                          const plr_combat_attack* current_chosen_attack,
                                          const void*              unk) {

    return ((bool (__thiscall*)(spiderman_rvb_counter_attack*, e_combat_move, const player_interface*, als_inode*, f32, f32*, e_combat_move, const plr_combat_attack*, const void*))retail::sub_9A7520)(this,
                                                                                                                                                                                                        move,
                                                                                                                                                                                                        plr_ifc,
                                                                                                                                                                                                        my_als,
                                                                                                                                                                                                        delta_t,
                                                                                                                                                                                                        best_time,
                                                                                                                                                                                                        current_chosen_move,
                                                                                                                                                                                                        current_chosen_attack,
                                                                                                                                                                                                        unk);
}

// slot 12
bool spiderman_rvb_counter_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_counter_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C38E0)(this,
                                                                                                                                     plr_ifc,
                                                                                                                                     unk,
                                                                                                                                     store_targets,
                                                                                                                                     check_possible);
}

// sub_9BACF0
bool spiderman_rvb_counter_attack::attack_possible(const player_interface* plr_ifc) {
    if (!plr_combat_attack::attack_possible(plr_ifc))
        return false;

    if (plr_ifc->get_mode() != 19)
        return false;

    return !plr_ifc->unk_258().null() && plr_ifc->unk_258().resolve();
}
