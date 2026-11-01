#include <cmath>
#include <cstdlib>

#include "retail.hh"
#include "game/wds/entity/interface/plr_combat_attack.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity_base.hh"
#include "game/input/game_axis.hh"
#include "game/input/game_button.hh"
#include "game/math/po.hh"
#include "game/math/references.hh"
#include "game/mutex.hh"

using namespace treyarch;

// ==== plr_combat_attack ==== //

string_hash plr_combat_attack::my_type_key { 0x04BE9B7A }; // "plr_combat_attack"

// sub_999DD0
// inlined @ sub_99A4B0
plr_combat_attack::plr_combat_attack() {
    initialize(mash::ALLOCATED);
}

// sub_718F40
plr_combat_attack::~plr_combat_attack() {
    finalize(mash::ALLOCATED);
}

// sub_999E20
void plr_combat_attack::construct_mashed_class() {
    initialize(mash::FROM_MASH);
}

// sub_99B2F0
void plr_combat_attack::destruct_mashed_class() {
    finalize(mash::FROM_MASH);
}

// sub_7AE090
void plr_combat_attack::unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}

// sub_718F30
mash::virtual_types_key plr_combat_attack::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_718FB0
bool plr_combat_attack::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_5AE580
i32 plr_combat_attack::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9943C0
bool plr_combat_attack::update(e_combat_move            move,
                               const player_interface*  plr_ifc,
                               als_inode*               my_als,
                               f32,
                               f32*                     best_time,
                               e_combat_move,
                               const plr_combat_attack* current_chosen_attack,
                               const void*              unk) {

    // the input record's button
    const game_button* input_button = unk ? *(const game_button* const*)((const u8*)unk + 0x0C) : nullptr;

    if ((unk && input_button != button)            ||
        (!button && move != combat_move_dodge)     ||
        !attack_possible(plr_ifc)) {

        clear();

        return claim_best_time(false, best_time, current_chosen_attack);
    }

    // a press of the input's button counts without looking at the button's flags
    if (button && ((unk && input_button == button) || button->triggered_or_missed_press())) {
        timer = -1.0f;

        f32 time = my_als->get_current_time(als::base_layer);

        if (!(time >= unk_022(move, plr_ifc, my_als)) ||
            !(unk_023(move, plr_ifc, my_als) >= time))

            return claim_best_time(false, best_time, current_chosen_attack);

        bool found = build_target_list(plr_ifc, unk, true, true);

        button_held = !unk && button->pressed();
        timer       = time;

        return claim_best_time(found, best_time, current_chosen_attack);
    }

    if (!(timer >= 0.0f))
        return claim_best_time(false, best_time, current_chosen_attack);

    if (button_held && (!button || !button->pressed()))
        button_held = false;

    if (move == combat_move_dodge) {
        if (update_target_list(plr_ifc))
            return claim_best_time(true, best_time, current_chosen_attack);

        clear();

        return claim_best_time(false, best_time, current_chosen_attack);
    }

    bool found;

    // rebuild once the heading or the up has turned away from the last build
    if (targets->empty()                                                                 ||
        (plr_ifc->player_axis[0]->get_force() > 0.5f                                     &&
         (f64)0.85f > get_attack_direction(plr_ifc, unk) * attack_dir)                   ||
        (f64)0.85f > get_attack_up(plr_ifc, unk) * attack_up)

        found = build_target_list(plr_ifc, unk, true, true);
    else
        found = update_target_list(plr_ifc);

    return claim_best_time(found, best_time, current_chosen_attack);
}

// sub_99B1A0
void plr_combat_attack::clear() {
    timer      = -1.0f;
    attack_dir = references::unk_01111bf0.get();

    targets->clear();

    button_held = false;
}

// sub_9878F0
bool plr_combat_attack::has_potential_targets(const player_interface* plr_ifc, bool check_possible) {
    return build_target_list(plr_ifc, nullptr, false, check_possible);
}

// sub_99B900
bool plr_combat_attack::build_target_list(const player_interface* plr_ifc, const void* unk, bool store_targets, bool check_possible) {
    bool found = false;

    if (store_targets)
        targets->clear();

    if (check_possible && !attack_possible(plr_ifc))
        return found;

    // the input record's direction stands in for the movement axis
    f32 input_mag = unk ? ((const vector3d*)unk)->length2() : plr_ifc->player_axis[0]->get_force();

    bool has_input       = (f64)input_mag > (f64)0.0001f;
    f32  saved_min_range = min_range;

    if (inside_min_range_ignore_arc && !has_input && plr_ifc->get_mode() != 9)
        min_range = -2.0f;

    attack_dir = get_attack_direction(plr_ifc, unk);
    attack_up  = get_attack_up(plr_ifc, unk);

    vc8::vector<ai_target> found_targets;
    ai_target              candidate;

    f32 max_vert = std::fabs(max_vert_range);
    f32 min_vert = std::fabs(min_vert_range);
    f32 vert     = max_vert > min_vert ? max_vert : min_vert;
    f32 range    = max_range;

    arch_base_vhandle preferred = plr_ifc->unk_483(this);

    // the preferred target, when it scores, is the only one
    if (preferred.resolve()) {
        candidate.set_target(preferred);

        f32 score = compute_target_score(&candidate, plr_ifc, 0);

        if ((f64)score > (f64)0.0001f) {
            candidate.score = score;
            found           = true;

            if (store_targets)
                found_targets.push_back(candidate);
        }
    }

    // without store_targets each source stops at its first scoring candidate
    if (!found) {
        retail::sub_5FE3D0(); // damageable search lock

        engine_lock_scope list_lock(&references::ai_core_list_lock.get());

        f32 radius       = vector3d(vert, range, 0.0f).length();
        u32 search_flags = ((target_flags & ai_target::target_allow_dead_target) | 0x81800) >> 11; // alive, visible and 0x100, plus allow dead

        i32 num_found = retail::sub_643530((u32*)&plr_ifc->my_actor->my_abs_po->get_position(), radius, search_flags); // damage_interface::find_damageable

        for (ai_core_list* list : { &references::ai_core_list_high.get(), &references::ai_core_list_low.get() }) {
            if (!list->count)
                continue;

            for (ai_core_list_node* node = list->head; node; node = node->next) {
                if (!node->core->get_actor()->has_ifc(entity_ifc_damage))
                    continue;

                const vector3d &pos = plr_ifc->my_actor->my_abs_po->get_position();

                if (!((bool (__thiscall*)(damage_interface*, const vector3d*, f32, u32))retail::sub_621270)(node->core->get_actor()->damage_ifc(),
                                                                                                       &pos,
                                                                                                       radius,
                                                                                                       search_flags))

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

        // the cored ones were in the lists already
        if (num_found > 0) {
            vc8::list<damage_interface*>* found_list = references::found_damageable.read();

            for (auto* node = found_list->begin(); node != found_list->end(); node = node->next) {
                if (node->value->my_actor->get_ai_core())
                    continue;

                candidate.set_target(node->value->my_actor->my_handle);

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

        retail::sub_5FE3F0(); // damageable search unlock
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

    min_range = saved_min_range;

    return found;
}

// sub_99B200
bool plr_combat_attack::update_target_list(const player_interface* plr_ifc) {
    if (attack_possible(plr_ifc) || plr_ifc->get_mode() == 9)
        for (ai_target* targ = targets->begin(); targ != targets->end();) {
            if (target_still_valid(targ, plr_ifc))
                ++targ;
            else
                targ = targets->erase(targ);
        }
    else
        targets->clear();

    return !targets->empty();
}

// sub_994310
bool plr_combat_attack::should_execute(const player_interface* plr_ifc, f32 tolerance) {
    if (!(timer >= 0.0f))
        return false;

    if (button_held)
        return true;

    if (button && button->triggered_or_missed_press())
        return true;

    if (!((f64)tolerance > (f64)0.0001f))
        return false;

    auto* my_als = (als_inode*)plr_ifc->my_actor->get_ai_core()->get_info_node(info_node_type_als);

    if (!my_als)
        return false;

    return tolerance >= my_als->get_current_time(als::base_layer) - timer;
}

// sub_99B710
void plr_combat_attack::forced_set_up(const player_interface* plr_ifc, e_combat_move, const ai_target* targ, i32 script_move) {
    clear();

    auto* my_als = (als_inode*)plr_ifc->my_actor->get_ai_core()->get_info_node(info_node_type_als);

    timer           = my_als->get_current_time(als::base_layer);
    button_held     = false;
    script_move_num = script_move;
    attack_dir      = get_attack_direction(plr_ifc, nullptr);
    attack_up       = get_attack_up(plr_ifc, nullptr);

    if (!targ->target.null())
        targets->push_back(*targ);
}

// sub_99B7D0
void plr_combat_attack::forced_set_up(const player_interface* plr_ifc, e_combat_move, const vc8::vector<ai_target>* targs, i32 script_move) {
    clear();

    auto* my_als = (als_inode*)plr_ifc->my_actor->get_ai_core()->get_info_node(info_node_type_als);

    timer           = my_als->get_current_time(als::base_layer);
    button_held     = false;
    script_move_num = script_move;
    attack_dir      = get_attack_direction(plr_ifc, nullptr);
    attack_up       = get_attack_up(plr_ifc, nullptr);

    for (const ai_target &targ : *targs)
        targets->push_back(targ);
}

// sub_994690
vector3d plr_combat_attack::get_attack_direction(const player_interface* plr_ifc, const void* unk) {
    vector3d heading;

    // the input record's direction, else the stick's while it's pushed, else the facing
    if (unk)
        heading = *(const vector3d*)unk;
    else if ((f64)plr_ifc->player_axis[0]->get_force() > (f64)0.0001f)
        heading = plr_ifc->player_axis[0]->get_player_dir();
    else
        heading = plr_ifc->my_actor->my_abs_po->get_z_facing();

    vector3d dir = math::unitize(heading);
    vector3d up  = get_attack_up(plr_ifc, unk);

    if (!(std::fabs(up * dir) < 0.9999f)) {
        const po* abs_po = plr_ifc->my_actor->my_abs_po;

        dir = math::unitize(abs_po->get_z_facing());

        if (!(std::fabs(up * dir) < 0.9999f)) {
            dir = abs_po->get_y_facing();
            dir.normalize();
        }
    }

    const vector3d &zero = references::unk_01111bf0.get();

    math::vector4 basis_dir(dir.x, dir.y, dir.z, 0.0f);
    math::vector4 basis_up(up.x, up.y, up.z, 0.0f);
    math::vector4 basis_origin(zero.x, zero.y, zero.z, 0.0f);

    po basis;
    basis.set_po(basis_dir, basis_up, basis_origin);

    return basis.get_z_facing();
}

// sub_987840
vector3d plr_combat_attack::get_attack_up(const player_interface* plr_ifc, const void*) {
    vector3d up = plr_ifc->my_actor->my_abs_po->get_y_facing();
    up = math::unitize(up);

    return up;
}

// sub_823B70
bool plr_combat_attack::attack_possible(const player_interface*) {
    return true;
}

// sub_994900
f32 plr_combat_attack::compute_target_score(ai_target* tmp_target, const player_interface* plr_ifc, u32 flag_override) {
    f32 score = -1.0f;

    auto* ent = (entity_base*)tmp_target->target.resolve();

    if (!ent)
        return score;

    auto* my_als = (als_inode*)plr_ifc->my_actor->get_ai_core()->get_info_node(info_node_type_als);

    // only the current combat target counts while the als asks to keep it
    if (plr_ifc->get_mode() == 9 && my_als->get_optional_als_pb_int(maintain_current_target_key, 0, als::base_layer, nullptr))
        return plr_ifc->is_current_combat_target(tmp_target->target) ? 1.0f : score;

    u32    flags = flag_override ? flag_override : target_flags;
    actor* owner = plr_ifc->my_actor;

    score = tmp_target->compute_score(owner,
                                      owner->my_abs_po->get_position(),
                                      attack_dir,
                                      attack_up,
                                      min_range,
                                      max_range,
                                      min_vert_range,
                                      max_vert_range,
                                      min_dotp,
                                      min_vert_dotp,
                                      flags,
                                      nullptr,
                                      0,
                                      0,
                                      0x14);

    if (score == -1.0f)
        return score;

    // anything without an ai core or the special target flag, and the dead, rank below every scoring enemy
    if ((flags & ai_target::target_allow_dead_target)              &&
        ent->is_an_actor()                                         &&
        !ent->is_alive()                                           &&
        !plr_ifc->is_current_combat_target(ent->my_handle)) {

        if (!((actor*)ent)->get_ai_core())
            return -1.0f;
    } else if (((actor*)ent)->get_ai_core() || (ent->flags & entity_flag_special_target))
        return score;

    return (f32)(((f64)score + 1.0) * (f64)0.05f - (f64)0.9999f);
}

// sub_9878D0
bool plr_combat_attack::target_still_valid(ai_target* tmp_target, const player_interface* plr_ifc) {
    return tmp_target->target_valid(plr_ifc->my_actor, target_flags);
}

// sub_987910
f32 plr_combat_attack::unk_022(e_combat_move, const player_interface*, als_inode* my_als) {
    return my_als->get_optional_als_pb_float(combat_button_begin_time_key, 0.0f, als::base_layer, nullptr);
}

// sub_987940
f32 plr_combat_attack::unk_023(e_combat_move, const player_interface*, als_inode* my_als) {
    f32 duration = my_als->get_duration(als::base_layer);

    return my_als->get_optional_als_pb_float(combat_button_end_time_key, duration, als::base_layer, nullptr);
}

// sub_9877C0
void plr_combat_attack::init(const game_button* _button,
                             bool               _allow_no_targets,
                             u32                _target_flags,
                             f32                _min_range,
                             f32                _max_range,
                             f32                _min_dotp,
                             f32                _min_vert_range,
                             f32                _max_vert_range,
                             f32                _min_vert_dotp,
                             bool               _inside_min_range_ignore_arc,
                             bool               _unk_4b) {

    button                      = _button;
    allow_no_targets            = _allow_no_targets;
    min_dotp                    = _min_dotp;
    min_vert_dotp               = _min_vert_dotp;
    min_range                   = _min_range;
    max_range                   = _max_range;
    target_flags                = _target_flags;
    min_vert_range              = _min_vert_range;
    max_vert_range              = _max_vert_range;
    inside_min_range_ignore_arc = _inside_min_range_ignore_arc;
    unk_4b                      = _unk_4b;

    clear();
}

// inlined @ sub_9943C0
bool plr_combat_attack::claim_best_time(bool found, f32* best_time, const plr_combat_attack* current_chosen_attack) {
    if (!found && !allow_no_targets)
        return false;

    if (!(timer >= 0.0f))
        return false;

    f32  best   = *best_time;
    bool better = unk_4b ? timer >= best : timer > best;

    // with targets, a press within 0.01 of the current attack's also wins
    if (!better && found && current_chosen_attack)
        better = unk_4b ?
            (f64)timer >= (f64)best - (f64)0.01f :
            (f64)timer >  (f64)best - (f64)0.01f && !current_chosen_attack->has_targets();

    if (!better)
        return false;

    *best_time = timer;

    return true;
}

// sub_999B70
void plr_combat_attack::initialize(mash::allocation_scope) {
    void* allocation = mm::allocate(sizeof(vc8::vector<ai_target>));

    auto* list = allocation ? new (allocation) vc8::vector<ai_target>() : nullptr;

    timer                       = -1.0f;
    button                      = nullptr;
    targets                     = list;
    attack_dir                  = references::unk_01111bf0.get();
    allow_no_targets            = false;
    unk_4b                      = false;
    min_dotp                    = 0.5f;
    min_vert_dotp               = -1.1f;
    min_range                   = 0.0f;
    max_range                   = 6.0f;
    min_vert_range              = -5.0f;
    target_flags                = 0;
    max_vert_range              = 5.0f;
    button_held                 = false;
    inside_min_range_ignore_arc = true;
}

// sub_99B160
// inlined @ sub_99B2F0
void plr_combat_attack::finalize(mash::allocation_scope) {
    if (targets) {
        targets->~vector();

        mm::free(targets);
    }

    targets = nullptr;
}

// ==== plr_trick_move ==== //

string_hash plr_trick_move::my_type_key { 0x127EB5C0 }; // "plr_trick_move"

// sub_988220
plr_trick_move::plr_trick_move() {
    initialize(mash::ALLOCATED);
}

// sub_718FC0
plr_trick_move::~plr_trick_move() {
    finalize(mash::ALLOCATED);
}

// sub_988280
void plr_trick_move::construct_mashed_class() {
    timer.construct_mashed_class();

    initialize(mash::FROM_MASH);
}

// sub_5B04C0
void plr_trick_move::destruct_mashed_class() {
    timer.destruct_mashed_class();
}

// sub_5E2F30
void plr_trick_move::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    timer.unmash(mash_info, this, buffer);
}

// sub_718FA0
mash::virtual_types_key plr_trick_move::get_virtual_type_key() const {
    return my_type_key.source_hash_code;
}

// sub_718FB0
bool plr_trick_move::is_subclass_of(mash::virtual_types_key parent_class) const {
    return mash::mash_virtual_base::get_virtual_type_key() == parent_class;
}

// sub_4C6780
i32 plr_trick_move::get_mash_sizeof() const {
    return sizeof(*this);
}

// sub_9879A0
bool plr_trick_move::update(f32 delta_t, f32* best_time, bool) {
    if (!button)
        clear();
    else if (button->triggered_or_missed_press()) {
        button_held = button->pressed();
        timer       = 0.75f;
    } else if (timer.time > 0.0f) {
        // letting go cuts the window short
        if (button_held && !button->pressed()) {
            button_held = false;

            if (timer.time > 0.75f)
                timer = 0.75f;
        }

        timer = (f32)((f64)timer.time - delta_t);

        if (0.0f > timer.time)
            timer = 0.0f;
    }

    if (timer.time > *best_time) {
        *best_time = timer.time;

        return true;
    }

    return false;
}

// sub_987990
void plr_trick_move::clear() {
    timer       = 0.0f;
    button_held = false;
}

// sub_987A80
bool plr_trick_move::should_execute() const {
    if (!((f64)timer.time > (f64)0.0001f))
        return false;

    return button_held || (button && button->triggered_or_missed_press());
}

// sub_987970
void plr_trick_move::init(const game_button* _button) {
    button = _button;

    clear();
}

// inlined @ sub_988220
// inlined @ sub_988280
void plr_trick_move::initialize(mash::allocation_scope) {
    button      = nullptr;
    timer       = 0.0f;
    button_held = false;
}

// sub_689610
void plr_trick_move::finalize(mash::allocation_scope) {}
