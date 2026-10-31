#include "retail.hh"
#include "game/wds/ai/actions/ai_action_nugget.hh"

using namespace treyarch;

// ==== ai_action_nugget ==== //

// sub_492DF0
ai_action_nugget::~ai_action_nugget() {
    if (my_action_info.active)
        deactivate(true);
}

// sub_492DD0
void ai_action_nugget::set_action_running() {
    my_action_info.current_mode = mode_running;
}

// sub_492DE0
void ai_action_nugget::set_action_blocked() {
    my_action_info.current_mode = mode_blocked;
}

// ==== play_anim_ai_action ==== //

play_anim_ai_action::play_anim_ai_action(ai_core* owner, const string_hash &my_hash) { // the retail constructor builds the nugget part too
    ((play_anim_ai_action* (__thiscall*)(play_anim_ai_action*, ai_core*, const string_hash*))retail::sub_492E40)(this, owner, &my_hash);
}

// slot 1
ai_action_nugget::e_action_exit_status play_anim_ai_action::frame_advance(f32 delta_t) {
    return ((e_action_exit_status (__thiscall*)(play_anim_ai_action*, f32))retail::sub_492EC0)(this, delta_t);
}

// sub_48E9D0
// inlined @ sub_4A1DA0
void play_anim_ai_action::activate() {
    start_anim();

    ai_action_nugget::activate();
}

// slot 3
void play_anim_ai_action::deactivate(bool on_finalize) {
    ((void (__thiscall*)(play_anim_ai_action*, bool))retail::sub_492FD0)(this, on_finalize);
}

// sub_48E9B0
void play_anim_ai_action::setup(als::category_id _category, als::layer_id _layer, bool exit_on_anim_finish) {
    category       = _category;
    layer          = _layer;
    exit_on_finish = exit_on_anim_finish;
}

// slot 7
void play_anim_ai_action::start_anim() {
    ((void (__thiscall*)(play_anim_ai_action*))retail::sub_492F40)(this);
}

// sub_667F80
bool play_anim_ai_action::should_succeed() const {
    return true;
}

// sub_71F870
bool play_anim_ai_action::is_master_request() const {
    return false;
}

// sub_71F870
bool play_anim_ai_action::ignore_same_cat_protection() const {
    return false;
}
