#include <new>

#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/cutscene/cut_scene.hh"
#include "game/cutscene/cut_scene_player.hh"
#include "game/cutscene/cut_scene_toa_helper.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/frontend/igo/igo_3d_agility_test.hh"
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
        retail::sub_6DE2E0((u32*)references::frontend.get().igo->face_button_system);

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
    unk_7d0              = false;
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

    references::frontend.get().igo->agility_test->unk_15c = false;

    running = false;
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
