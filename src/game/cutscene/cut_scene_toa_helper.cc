#include <new>

#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/cutscene/cut_scene.hh"
#include "game/cutscene/cut_scene_player.hh"
#include "game/cutscene/cut_scene_toa_helper.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/frontend/igo/igo_3d_agility_test.hh"
#include "game/frontend/igo/igo_3d_face_button_system.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "game/resource_key.hh"

using namespace treyarch;

// sub_807F30
cut_scene_toa_helper::cut_scene_toa_helper() {}

// sub_807FB0
cut_scene_toa_helper::~cut_scene_toa_helper() {}

// sub_809590
cut_scene_toa_helper* cut_scene_toa_helper::get() {
    u32 &guard = references::cut_scene_toa_helper_guard.get();

    if (!(guard & 1)) {
        guard |= 1;

        new (&references::cut_scene_toa_helper.get()) cut_scene_toa_helper();
        retail::sub_ADE5FD(&release_inst); // the game's CRT atexit
    }

    return &references::cut_scene_toa_helper.get();
}

// sub_B72E70
void cut_scene_toa_helper::release_inst() {
    references::cut_scene_toa_helper.get().~cut_scene_toa_helper();
}

// sub_809BB0
bool cut_scene_toa_helper::setup(chuck::vm::script_dynamic_array<const char*> cutscenes,
                                 chuck::vm::script_dynamic_array<f32>         button_nums,
                                 chuck::vm::script_dynamic_array<f32>         button_ids,
                                 chuck::vm::script_dynamic_array<f32>         show_times,
                                 chuck::vm::script_dynamic_array<f32>         end_times,
                                 bool                                         with_fades) {

    fade_endings = with_fades;

    for (i32 i = 0; i < 25; ++i) {
        buttons[i].set_device_id(current_joystick_id);
        buttons[i].set_control(i);
        buttons[i].enable(i > 0);
    }

    cutscene_list.clear();
    button_num_per_sequence.clear();
    button_list.clear();
    show_button_time_list.clear();
    end_button_time_list.clear();

    for (i32 i = 0; i < cutscenes.size(); ++i)
        cutscene_list.push_back(mash::string(cutscenes[i]));

    for (i32 i = 0; i < button_nums.size(); ++i)
        button_num_per_sequence.push_back((i32)button_nums[i]);

    for (i32 i = 0; i < button_ids.size(); ++i)
        button_list.push_back(get_agility_test_button_corresponding_to_num((i32)button_ids[i]));

    for (i32 i = 0; i < show_times.size(); ++i)
        show_button_time_list.push_back(show_times[i]);

    for (i32 i = 0; i < end_times.size(); ++i)
        end_button_time_list.push_back(end_times[i]);

    if (references::frontend.get().igo->face_button_system)
        references::frontend.get().igo->face_button_system->remove_all_buttons();

    current_sequence = 0;

    cut_scene* first = get_success_category(0);
    current_cutscene = 2 * current_sequence;

    if (!first)
        return false;

    first->flags &= ~cut_scene::takes_igo;

    cut_scene_player* player = references::cut_scene_player.read();

    player->state_flags &= ~cut_scene_player::toa_middle;
    player->state_flags  = (player->state_flags & ~cut_scene_player::toa_last) | cut_scene_player::toa_first;

    references::cut_scene_player.read()->play(first);

    cut_scene_started = true;

    retail::sub_6AAA00((u32**)references::frontend.get().igo->agility_test, false); // hides the prompt

    current_button       = 0;
    current_button_index = 0;
    failed               = false;
    succeeded            = false;
    window_open          = false;
    running              = true;

    references::g_world_ptr.read()->get_hero_ptr()->raise_event(references::cut_scene_toa_begin.read());

    scene_anim_paused = false;

    return true;
}

// sub_808070
void cut_scene_toa_helper::stop() {
    button_num_per_sequence.clear();
    button_list.clear();
    show_button_time_list.clear();
    end_button_time_list.clear();
    cutscene_list.clear();

    retail::sub_6AAA00((u32**)references::frontend.get().igo->agility_test, false); // hides the prompt

    references::frontend.get().igo->agility_test->waits_for_press = false;

    running = false;
}

// sub_80A250
void cut_scene_toa_helper::frame_advance(f32 time_inc) {
    buttons[0].clear();

    for (i32 i = 1; i < 25; ++i)
        buttons[i].update(time_inc, false, false);

    cut_scene_player* player = references::cut_scene_player.read();

    if (current_sequence == (i32)(cutscene_list.size() / 2)) {
        if (!player->is_playing()) {
            references::g_world_ptr.read()->get_hero_ptr()->raise_event(failed ?
                references::cut_scene_toa_failure.read() :
                references::cut_scene_toa_success.read());

            running = false;
        }
    } else if (current_sequence < (i32)(cutscene_list.size() / 2)) {
        step_pressed_buttons();

        if (missed_window() || (window_open && accepts_press() && wrong_button_pressed())) {
            if (waits_for_press()) {
                if (!scene_anim_paused) {
                    references::g_world_ptr.read()->set_scene_anim_paused(player->scene_anim_handle, true);

                    scene_anim_paused = true;
                }
            } else {
                if (!failed) {
                    string_hash toa_failure;
                    toa_failure.initialize(mash::ALLOCATED, "TOA_FAILURE");

                    retail::sub_900D60(toa_failure.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
                }

                failed = true;
            }
        }

        if (failed && !player->is_playing()) {
            cut_scene* failure = get_failure_category(current_sequence);
            current_cutscene = 2 * current_sequence + 1;

            if (!failure)
                return;

            failure->flags &= ~cut_scene::takes_igo;

            player->state_flags &= ~cut_scene_player::toa_middle;
            player->state_flags  = (player->state_flags & ~cut_scene_player::toa_first) | cut_scene_player::toa_last;

            player->play(failure);

            cut_scene_started = true;
            current_sequence  = (i32)(cutscene_list.size() / 2);
        } else if (succeeded && !player->is_playing()) {
            ++current_sequence;

            current_button       = 0;
            current_button_index = 0;

            cut_scene* success = get_success_category(current_sequence);
            current_cutscene = 2 * current_sequence;

            if (!success)
                return;

            success->flags &= ~cut_scene::takes_igo;

            if (current_sequence + 1 < (i32)(cutscene_list.size() / 2)) {
                player->state_flags |= cut_scene_player::toa_middle;
                player->state_flags &= ~(cut_scene_player::toa_first | cut_scene_player::toa_last);
            } else {
                player->state_flags &= ~cut_scene_player::toa_middle;
                player->state_flags  = (player->state_flags & ~cut_scene_player::toa_first) | cut_scene_player::toa_last;
            }

            player->play(success);

            cut_scene_started = true;
            succeeded         = false;
        } else if (window_open &&
                   accepts_press() &&
                   current_button_index == button_num_per_sequence[current_sequence]) {

            unpause_scene_anim();

            succeeded = true;

            references::g_world_ptr.read()->get_hero_ptr()->raise_event(references::cut_scene_toa_hit_button_success.read());

            string_hash toa_success;
            toa_success.initialize(mash::ALLOCATED, "TOA_SUCCESS");

            retail::sub_900D60(toa_success.source_hash_code, 0x10000000, 0); // post the sound event on the global game object

            if (references::frontend.get().igo->agility_test)
                retail::sub_692330((i32)references::frontend.get().igo->agility_test);
        }
    }

    update_prompt();

    if (accepts_press() &&
        current_sequence < (i32)(cutscene_list.size() / 2) &&
        !failed &&
        !cut_scene_started)

        references::g_world_ptr.read()->get_hero_ptr()->raise_event(references::cut_scene_toa_button_shown.read());
    else
        references::g_world_ptr.read()->get_hero_ptr()->raise_event(references::cut_scene_toa_button_not_shown.read());

    cut_scene_started = false;
    window_open       = is_window_open();
}

// sub_808170
void cut_scene_toa_helper::step_pressed_buttons() { // takes this frame's press and moves the prompt on to the next button
    if (!window_open || !accepts_press())
        return;

    i32  sequence = current_sequence;
    i32  index    = current_button_index;
    i32 &count    = button_num_per_sequence[sequence];

    if (index == count)
        return;

    if (!buttons[get_button(sequence, index)].triggered())
        return;

    current_button       = get_button(sequence, index);
    current_button_index = index + 1;

    if (index + 1 == count)
        return;

    vc8::vector<i32> next_buttons;
    next_buttons.push_back(get_num_corresponding_to_agility_test_buttons_t(get_button(sequence, index + 1)));

    ((void (__thiscall*)(igo_3d_agility_test*, vc8::vector<i32>))retail::sub_708190)(references::frontend.get().igo->agility_test,
                                                                                     next_buttons);

    retail::sub_6AAA00((u32**)references::frontend.get().igo->agility_test, false);
    retail::sub_6AAA00((u32**)references::frontend.get().igo->agility_test, true);

    sequence = current_sequence;

    f32 end_time = get_end_button_time(sequence);
    f32 window   = end_time - get_show_button_time(sequence);

    retail::sub_6922C0((f32*)references::frontend.get().igo->agility_test, window / button_num_per_sequence[sequence]);

    references::frontend.get().igo->agility_test->waits_for_press = waits_for_press();

    if (get_button(current_sequence, current_button_index) != current_button)
        step_pressed_buttons();
}

// sub_808310
void cut_scene_toa_helper::update_prompt() {
    if (!is_window_open() || current_sequence >= (i32)(cutscene_list.size() / 2)) {
        retail::sub_6AAA00((u32**)references::frontend.get().igo->agility_test, false); // hides the prompt

        return;
    }

    if (failed || waits_for_press()) {
        igo_3d_agility_test* agility_test = references::frontend.get().igo->agility_test;

        if (!agility_test->unk_150)
            retail::sub_692250((i32)agility_test, true);

        return;
    }

    if (references::frontend.get().igo->agility_test->prompt_shown)
        return;

    vc8::vector<i32> prompt_buttons;
    prompt_buttons.push_back(get_num_corresponding_to_agility_test_buttons_t(get_button(current_sequence, current_button_index)));

    ((void (__thiscall*)(igo_3d_agility_test*, vc8::vector<i32>))retail::sub_708190)(references::frontend.get().igo->agility_test,
                                                                                     prompt_buttons);

    i32 sequence = current_sequence;

    f32 end_time = get_end_button_time(sequence);
    f32 window   = end_time - get_show_button_time(sequence);

    retail::sub_6922C0((f32*)references::frontend.get().igo->agility_test, window / button_num_per_sequence[sequence]);
    retail::sub_6AAA00((u32**)references::frontend.get().igo->agility_test, true); // shows the prompt

    references::frontend.get().igo->agility_test->waits_for_press = waits_for_press();

    string_hash toa_start;
    toa_start.initialize(mash::ALLOCATED, "TOA_START");

    retail::sub_900D60(toa_start.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
}

// sub_8059A0
i32 cut_scene_toa_helper::get_button(i32 _current_sequence, i32 index) const {
    i32 sequence_iterator = 0;
    i32 button_iterator   = 0;

    for (u32 i = 0; i < button_list.size(); ++i) {
        if (button_iterator == index && sequence_iterator == _current_sequence)
            return button_list[i];

        if (sequence_iterator > _current_sequence)
            break;

        if (++button_iterator >= button_num_per_sequence[sequence_iterator]) {
            button_iterator = 0;
            ++sequence_iterator;
        }
    }

    return 0;
}

// sub_805A00
bool cut_scene_toa_helper::waits_for_press() const {
    mash::string name(cutscene_list[current_cutscene]);

    resource_key key;
    retail::sub_739B10((u32*)&key, name.c_str(), 93); // a cut scene's key

    auto found = (cut_scene*)amalga::resource_manager::get_resource(key, nullptr, nullptr);

    if (found)
        return found->waits_for_press;

    return false;
}

// sub_8053A0
void cut_scene_toa_helper::unpause_scene_anim() {
    if (!scene_anim_paused)
        return;

    references::g_world_ptr.read()->set_scene_anim_paused(references::cut_scene_player.read()->scene_anim_handle, false);

    scene_anim_paused = false;
}

// sub_806B80
bool cut_scene_toa_helper::past_show_time() const {
    cut_scene_player* player = references::cut_scene_player.read();

    f32 segment_time = player->segment_time;
    f32 show_time    = get_show_button_time(current_sequence);
    f32 time         = segment_time;

    if (player->is_playing())
        time = player->segment_time + player->get_time_remaining();

    return (f64)segment_time >= (f64)time + (f64)show_time;
}

// sub_806C50
bool cut_scene_toa_helper::before_end_time() const {
    cut_scene_player* player = references::cut_scene_player.read();

    f32 segment_time = player->segment_time;
    f32 end_time     = get_end_button_time(current_sequence);
    f32 time         = segment_time;

    if (player->is_playing())
        time = player->segment_time + player->get_time_remaining();

    return (f64)time + (f64)end_time > (f64)segment_time;
}

// sub_806D20
bool cut_scene_toa_helper::past_end_time() const {
    cut_scene_player* player = references::cut_scene_player.read();

    f32 segment_time = player->segment_time;
    f32 end_time     = get_end_button_time(current_sequence);
    f32 time         = segment_time;

    if (player->is_playing())
        time = player->segment_time + player->get_time_remaining();

    return (f64)segment_time >= (f64)time + (f64)end_time;
}

// sub_806F10
bool cut_scene_toa_helper::is_window_open() const {
    bool past_show  = past_show_time();
    bool before_end = before_end_time();
    bool waits      = waits_for_press();

    bool prompt_ready = true;

    if (succeeded) {
        igo_3d_agility_test* agility_test = references::frontend.get().igo->agility_test;

        prompt_ready = agility_test ? (bool)retail::sub_692370((u32*)agility_test) : false;
    }

    bool open_to_end = !cut_scene_started && prompt_ready && past_show && before_end;
    bool open        = !cut_scene_started && prompt_ready && past_show;

    return waits ? open : open_to_end;
}

// sub_806FA0
bool cut_scene_toa_helper::accepts_press() const {
    bool past_show = past_show_time();

    waits_for_press(); // the answer is thrown away

    bool prompt_ready = true;

    if (references::frontend.get().igo->agility_test && !waits_for_press())
        prompt_ready = ((bool (__thiscall*)(igo_3d_agility_test*))retail::sub_6AAC70)(references::frontend.get().igo->agility_test);

    return !failed && !cut_scene_started && !succeeded && past_show && prompt_ready;
}

// sub_807020
bool cut_scene_toa_helper::missed_window() const {
    if (waits_for_press()) {
        if (cut_scene_started || succeeded)
            return false;

        return references::cut_scene_player.read()->get_time_remaining() < 0.1f;
    }

    if (cut_scene_started || succeeded)
        return false;

    return past_end_time();
}

// sub_806DF0
bool cut_scene_toa_helper::wrong_button_pressed() const {
    if (current_button_index == button_num_per_sequence[current_sequence])
        return false;

    i32 expected = get_button(current_sequence, current_button_index);

    for (i32 i = 0; i < 16; ++i) {
        i32 button = get_agility_test_button_corresponding_to_num(i);

        if (button != expected && button != current_button && buttons[button].triggered())
            return true;
    }

    return false;
}

// sub_805AA0
cut_scene* cut_scene_toa_helper::get_failure_category(i32 sequence) {
    mash::string name(cutscene_list[2 * sequence + 1]);

    resource_key key;
    retail::sub_739B10((u32*)&key, name.c_str(), 93); // a cut scene's key

    auto found = (cut_scene*)amalga::resource_manager::get_resource(key, nullptr, nullptr);

    if (found) {
        if (!fade_endings)
            found->flags |= cut_scene::no_fades;
        else
            found->flags &= ~cut_scene::no_fades;
    } else
        running = false;

    return found;
}

// sub_805B50
cut_scene* cut_scene_toa_helper::get_success_category(i32 sequence) {
    mash::string name(cutscene_list[2 * sequence]);

    resource_key key;
    retail::sub_739B10((u32*)&key, name.c_str(), 93); // a cut scene's key

    auto found = (cut_scene*)amalga::resource_manager::get_resource(key, nullptr, nullptr);

    if (found) {
        if (2 * sequence >= (i32)cutscene_list.size() - 1 && fade_endings)
            found->flags &= ~cut_scene::no_fades;
        else
            found->flags |= cut_scene::no_fades;
    } else
        running = false;

    return found;
}

// sub_8052C0
i32 cut_scene_toa_helper::get_agility_test_button_corresponding_to_num(i32 num) const {
    switch (num) {
        case 0:  return 13;
        case 1:  return 14;
        case 2:  return 16;
        case 3:  return 15;
        case 4:  return 17;
        case 5:  return 18;
        case 6:  return 20;
        case 7:  return 19;
        case 9:  return 3;
        case 10: return 2;
        case 11: return 4;
        case 12: return 8;
        case 13: return 5;
        case 14: return 9;
        case 15: return 6;
        default: return 1;
    }
}

// sub_90C8E0
i32 cut_scene_toa_helper::get_num_corresponding_to_agility_test_buttons_t(i32 button) {
    switch (button) {
        case 2:  return 10;
        case 3:  return 9;
        case 4:  return 11;
        case 5:  return 13;
        case 6:  return 15;
        case 8:  return 12;
        case 9:  return 14;
        case 13: return 0;
        case 14: return 1;
        case 15: return 3;
        case 16: return 2;
        case 17: return 4;
        case 18: return 5;
        case 19: return 7;
        case 20: return 6;
        default: return 8;
    }
}
