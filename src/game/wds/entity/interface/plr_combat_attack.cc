#include "retail.hh"
#include "game/wds/entity/interface/plr_combat_attack.hh"

using namespace treyarch;

// ==== plr_combat_attack ==== //

// inlined @ sub_99A4B0
plr_combat_attack::plr_combat_attack() {
    initialize(mash::ALLOCATED);
}

// slot 0
plr_combat_attack::~plr_combat_attack() {
    ((void (__thiscall*)(plr_combat_attack*, u32))retail::sub_718F40)(this, 0);
}

// slot 1
void plr_combat_attack::construct_mashed_class() {
    ((void (__thiscall*)(plr_combat_attack*))retail::sub_999E20)(this);
}

// slot 2
void plr_combat_attack::destruct_mashed_class() {
    ((void (__thiscall*)(plr_combat_attack*))retail::sub_99B2F0)(this);
}

// slot 3
void plr_combat_attack::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_combat_attack*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_7AE090)(this,
                                                                                                                   mash_info,
                                                                                                                   containing_class_ptr,
                                                                                                                   buffer);
}

// slot 4
mash::virtual_types_key plr_combat_attack::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_combat_attack*))retail::sub_718F30)(this);
}

// slot 6
bool plr_combat_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_combat_attack*, mash::virtual_types_key))retail::sub_718FB0)(this, parent_class);
}

// slot 8
i32 plr_combat_attack::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const plr_combat_attack*))retail::sub_5AE580)(this);
}

// slot 9
bool plr_combat_attack::update(e_combat_move            move,
                               const player_interface*  plr_ifc,
                               als_inode*               my_als,
                               f32                      delta_t,
                               f32*                     best_time,
                               e_combat_move            current_chosen_move,
                               const plr_combat_attack* current_chosen_attack,
                               const void*              unk) {

    return ((bool (__thiscall*)(plr_combat_attack*,
                                e_combat_move,
                                const player_interface*,
                                als_inode*,
                                f32,
                                f32*,
                                e_combat_move,
                                const plr_combat_attack*,
                                const void*))retail::sub_9943C0)(this,
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
void plr_combat_attack::clear() {
    ((void (__thiscall*)(plr_combat_attack*))retail::sub_99B1A0)(this);
}

// slot 11
bool plr_combat_attack::has_potential_targets(const player_interface* plr_ifc, bool unk) {
    return ((bool (__thiscall*)(plr_combat_attack*, const player_interface*, bool))retail::sub_9878F0)(this, plr_ifc, unk);
}

// slot 12
bool plr_combat_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool unk_4) {
    return ((bool (__thiscall*)(plr_combat_attack*, const player_interface*, const void*, bool, bool))retail::sub_99B900)(this,
                                                                                                                         plr_ifc,
                                                                                                                         unk,
                                                                                                                         store_targets,
                                                                                                                         unk_4);
}

// slot 13
bool plr_combat_attack::update_target_list(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(plr_combat_attack*, const player_interface*))retail::sub_99B200)(this, plr_ifc);
}

// slot 14
bool plr_combat_attack::should_execute(const player_interface* plr_ifc, f32 tolerance) {
    return ((bool (__thiscall*)(plr_combat_attack*, const player_interface*, f32))retail::sub_994310)(this, plr_ifc, tolerance);
}

// slot 15
void plr_combat_attack::forced_set_up(const player_interface* plr_ifc, e_combat_move move, const vc8::vector<ai_target>* targs, i32 script_move) {
    ((void (__thiscall*)(plr_combat_attack*, const player_interface*, e_combat_move, const vc8::vector<ai_target>*, i32))retail::sub_99B7D0)(this,
                                                                                                                                              plr_ifc,
                                                                                                                                              move,
                                                                                                                                              targs,
                                                                                                                                              script_move);
}

// slot 16
void plr_combat_attack::forced_set_up(const player_interface* plr_ifc, e_combat_move move, const ai_target* targ, i32 script_move) {
    ((void (__thiscall*)(plr_combat_attack*, const player_interface*, e_combat_move, const ai_target*, i32))retail::sub_99B710)(this,
                                                                                                                                 plr_ifc,
                                                                                                                                 move,
                                                                                                                                 targ,
                                                                                                                                 script_move);
}

// slot 17
vector3d plr_combat_attack::get_attack_direction(const player_interface* plr_ifc, const void* unk) {
    vector3d result;
    ((vector3d* (__thiscall*)(plr_combat_attack*, vector3d*, const player_interface*, const void*))retail::sub_994690)(this, &result, plr_ifc, unk);

    return result;
}

// slot 18
vector3d plr_combat_attack::get_attack_up(const player_interface* plr_ifc, const void* unk) {
    vector3d result;
    ((vector3d* (__thiscall*)(plr_combat_attack*, vector3d*, const player_interface*, const void*))retail::sub_987840)(this, &result, plr_ifc, unk);

    return result;
}

// slot 19
bool plr_combat_attack::attack_possible(const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(plr_combat_attack*, const player_interface*))retail::sub_823B70)(this, plr_ifc);
}

// slot 20
f32 plr_combat_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    return ((f32 (__thiscall*)(plr_combat_attack*, ai_target*, const player_interface*, u32))retail::sub_994900)(this,
                                                                                                                tmp_target,
                                                                                                                plr_ifc,
                                                                                                                flag_override);
}

// slot 21
bool plr_combat_attack::target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) {
    return ((bool (__thiscall*)(plr_combat_attack*, ai_target*, const player_interface*))retail::sub_9878D0)(this, tmp_target, plr_ifc);
}

// slot 22
f32 plr_combat_attack::unk_022(e_combat_move move, const player_interface* plr_ifc, als_inode* my_als) {
    return ((f32 (__thiscall*)(plr_combat_attack*, e_combat_move, const player_interface*, als_inode*))retail::sub_987910)(this,
                                                                                                                          move,
                                                                                                                          plr_ifc,
                                                                                                                          my_als);
}

// slot 23
f32 plr_combat_attack::unk_023(e_combat_move move, const player_interface* plr_ifc, als_inode* my_als) {
    return ((f32 (__thiscall*)(plr_combat_attack*, e_combat_move, const player_interface*, als_inode*))retail::sub_987940)(this,
                                                                                                                          move,
                                                                                                                          plr_ifc,
                                                                                                                          my_als);
}

void plr_combat_attack::initialize(mash::allocation_scope scope) {
    ((void (__thiscall*)(plr_combat_attack*, mash::allocation_scope))retail::sub_999B70)(this, scope);
}

// ==== plr_trick_move ==== //

plr_trick_move::plr_trick_move() {
    retail::sub_988220((u32*)this);
}

// slot 0
plr_trick_move::~plr_trick_move() {
    ((void (__thiscall*)(plr_trick_move*, u32))retail::sub_718FC0)(this, 0);
}

// slot 1
void plr_trick_move::construct_mashed_class() {
    ((void (__thiscall*)(plr_trick_move*))retail::sub_988280)(this);
}

// slot 2
void plr_trick_move::destruct_mashed_class() {
    ((void (__thiscall*)(plr_trick_move*))retail::sub_5B04C0)(this);
}

// slot 3
void plr_trick_move::unmash(mash::mash_info_struct* mash_info, void* containing_class_ptr, mash::buffer_type buffer) {
    ((void (__thiscall*)(plr_trick_move*, mash::mash_info_struct*, void*, mash::buffer_type))retail::sub_5E2F30)(this,
                                                                                                                mash_info,
                                                                                                                containing_class_ptr,
                                                                                                                buffer);
}

// slot 4
mash::virtual_types_key plr_trick_move::get_virtual_type_key() const {
    return ((mash::virtual_types_key (__thiscall*)(const plr_trick_move*))retail::sub_718FA0)(this);
}

// slot 6
bool plr_trick_move::is_subclass_of(mash::virtual_types_key parent_class) const {
    return ((bool (__thiscall*)(const plr_trick_move*, mash::virtual_types_key))retail::sub_718FB0)(this, parent_class);
}

// slot 8
i32 plr_trick_move::get_mash_sizeof() const {
    return ((i32 (__thiscall*)(const plr_trick_move*))retail::sub_4C6780)(this);
}

// slot 9
bool plr_trick_move::update(f32 delta_t, f32* best_time, bool has_next_trick) {
    return ((bool (__thiscall*)(plr_trick_move*, f32, f32*, bool))retail::sub_9879A0)(this, delta_t, best_time, has_next_trick);
}

// slot 10
void plr_trick_move::clear() {
    ((void (__thiscall*)(plr_trick_move*))retail::sub_987990)(this);
}

// slot 11
bool plr_trick_move::should_execute() const {
    return ((bool (__thiscall*)(const plr_trick_move*))retail::sub_987A80)(this);
}

// sub_987970
void plr_trick_move::init(const game_button* _button) {
    button = _button;

    clear();
}
