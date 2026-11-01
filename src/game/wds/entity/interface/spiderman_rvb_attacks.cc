#include "retail.hh"
#include "game/wds/entity/interface/spiderman_rvb_attacks.hh"
#include "game/wds/entity/interface/player_interface.hh"

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

// slot 19
bool spiderman_rvb_grapple_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_grapple_attack*, const player_interface*))retail::sub_9A6CC0)(this, plr_ifc);
}

// slot 20
f32 spiderman_rvb_grapple_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    return ((f32 (__thiscall*)(spiderman_rvb_grapple_attack*, ai_target*, const player_interface*, u32))retail::sub_9B6AD0)(this,
                                                                                                                            tmp_target,
                                                                                                                            plr_ifc,
                                                                                                                            flag_override);
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

// slot 19
bool spiderman_rvb_air_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_air_attack*, const player_interface*))retail::sub_9B6C50)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_air_jump_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_air_jump_attack*, const player_interface*))retail::sub_9B6CA0)(this, plr_ifc);
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

// slot 10
void spiderman_rvb_car_finish_attack::clear() {
    ((void (__thiscall*)(spiderman_rvb_car_finish_attack*))retail::sub_9B6E30)(this);
}

// slot 12
bool spiderman_rvb_car_finish_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_car_finish_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C2C30)(this,
                                                                                                                                        plr_ifc,
                                                                                                                                        unk,
                                                                                                                                        store_targets,
                                                                                                                                        check_possible);
}

// slot 21
bool spiderman_rvb_car_finish_attack::target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_car_finish_attack*, ai_target*, const player_interface*))retail::sub_9B6E40)(this, tmp_target, plr_ifc);
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

// slot 10
void spiderman_rvb_throw_manip_attack::clear() {
    ((void (__thiscall*)(spiderman_rvb_throw_manip_attack*))retail::sub_9B6E30)(this);
}

// slot 12
bool spiderman_rvb_throw_manip_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_throw_manip_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C2D60)(this,
                                                                                                                                         plr_ifc,
                                                                                                                                         unk,
                                                                                                                                         store_targets,
                                                                                                                                         check_possible);
}

// slot 19
bool spiderman_rvb_throw_manip_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_throw_manip_attack*, const player_interface*))retail::sub_9B6ED0)(this, plr_ifc);
}

// slot 21
bool spiderman_rvb_throw_manip_attack::target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_throw_manip_attack*, ai_target*, const player_interface*))retail::sub_9B6FF0)(this, tmp_target, plr_ifc);
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

// slot 19
bool spiderman_rvb_web_yank_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_web_yank_attack*, const player_interface*))retail::sub_9A6CC0)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_melee_or_car_combat_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_melee_or_car_combat_attack*, const player_interface*))retail::sub_9A6CF0)(this, plr_ifc);
}

// slot 20
f32 spiderman_rvb_melee_or_car_combat_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    return ((f32 (__thiscall*)(spiderman_rvb_melee_or_car_combat_attack*, ai_target*, const player_interface*, u32))retail::sub_9B72D0)(this,
                                                                                                                                        tmp_target,
                                                                                                                                        plr_ifc,
                                                                                                                                        flag_override);
}

// slot 21
bool spiderman_rvb_melee_or_car_combat_attack::target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_melee_or_car_combat_attack*, ai_target*, const player_interface*))retail::sub_9B7450)(this, tmp_target, plr_ifc);
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

// slot 11
bool spiderman_rvb_momentum_attack::has_potential_targets(const player_interface* plr_ifc, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_momentum_attack*, const player_interface*, bool))retail::sub_9BA970)(this, plr_ifc, check_possible);
}

// slot 12
bool spiderman_rvb_momentum_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_momentum_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C3820)(this,
                                                                                                                                      plr_ifc,
                                                                                                                                      unk,
                                                                                                                                      store_targets,
                                                                                                                                      check_possible);
}

// slot 19
bool spiderman_rvb_momentum_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_momentum_attack*, const player_interface*))retail::sub_9B7610)(this, plr_ifc);
}

// slot 20
f32 spiderman_rvb_momentum_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    return ((f32 (__thiscall*)(spiderman_rvb_momentum_attack*, ai_target*, const player_interface*, u32))retail::sub_9A6CD0)(this,
                                                                                                                             tmp_target,
                                                                                                                             plr_ifc,
                                                                                                                             flag_override);
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

// slot 11
bool spiderman_rvb_civ_save_attack::has_potential_targets(const player_interface* plr_ifc, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_civ_save_attack*, const player_interface*, bool))retail::sub_9BA970)(this, plr_ifc, check_possible);
}

// slot 12
bool spiderman_rvb_civ_save_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    return ((bool (__thiscall*)(spiderman_rvb_civ_save_attack*, const player_interface*, const void*, bool, bool))retail::sub_9C36C0)(this,
                                                                                                                                      plr_ifc,
                                                                                                                                      unk,
                                                                                                                                      store_targets,
                                                                                                                                      check_possible);
}

// slot 19
bool spiderman_rvb_civ_save_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_civ_save_attack*, const player_interface*))retail::sub_9B7070)(this, plr_ifc);
}

// slot 20
f32 spiderman_rvb_civ_save_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    return ((f32 (__thiscall*)(spiderman_rvb_civ_save_attack*, ai_target*, const player_interface*, u32))retail::sub_9A6CD0)(this,
                                                                                                                             tmp_target,
                                                                                                                             plr_ifc,
                                                                                                                             flag_override);
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

// slot 19
bool spiderman_rvb_wall_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_wall_attack*, const player_interface*))retail::sub_71FF10)(this, plr_ifc);
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

// slot 17
vector3d spiderman_rvb_pole_swing_attack::get_attack_direction(const player_interface* plr_ifc, const void* unk) {
    vector3d result;

    ((vector3d* (__thiscall*)(spiderman_rvb_pole_swing_attack*, vector3d*, const player_interface*, const void*))retail::sub_9BAB10)(this,
                                                                                                                                     &result,
                                                                                                                                     plr_ifc,
                                                                                                                                     unk);

    return result;
}

// slot 19
bool spiderman_rvb_pole_swing_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_pole_swing_attack*, const player_interface*))retail::sub_9BAA80)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_swing_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_swing_attack*, const player_interface*))retail::sub_9BA9B0)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_melee_or_car_combat_ground_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_melee_or_car_combat_ground_attack*, const player_interface*))retail::sub_9B7560)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_special_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_special_attack*, const player_interface*))retail::sub_9BAD50)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_special_x_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_special_x_attack*, const player_interface*))retail::sub_9BAD90)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_special_y_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_special_y_attack*, const player_interface*))retail::sub_9BAE90)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_special_b_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_special_b_attack*, const player_interface*))retail::sub_9BAF90)(this, plr_ifc);
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

// slot 19
bool spiderman_rvb_counter_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(spiderman_rvb_counter_attack*, const player_interface*))retail::sub_9BACF0)(this, plr_ifc);
}
