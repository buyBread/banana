#include "retail.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "game/cutscene/cut_scene.hh"
#include "game/cutscene/cut_scene_player.hh"
#include "game/cutscene/cut_scene_toa_helper.hh"
#include "game/region_pack_manager.hh"
#include "game/script/chuck_lib_scene_anim.hh"
#include "game/summon_state.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"

using namespace treyarch;

bool slf__create_cut_scene__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876390)(this, stack);
}

bool slf__create_preregistered_cut_scene__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876490)(this, stack);
}

// sub_876590
bool slf__wait_play__cut_scene__t::operator()(chuck::vm::vm_stack &stack) {
    auto to_play = (cut_scene*)stack.pop_uint();

    f32 result = 0.0f;

    if (references::g_world_ptr.read()->get_hero_ptr()->damage_ifc()->hit_points.val <= 0) {
        stack.push(&result, sizeof(result));

        return true;
    }

    if (references::summon_state.read() && region_pack_manager::inst())
        retail::sub_90FC00(references::summon_state.read()); // end the summon

    if (!to_play) {
        stack.push(&result, sizeof(result));

        return true;
    }

    cut_scene_player* player = references::cut_scene_player.read();

    bool busy = player->is_playing() && !player->is_skipping();

    // returning false keeps the thread waiting here; it comes back with entry set
    if (stack.thread->entry) {
        if (busy)
            return false;
    } else if (!busy && player->play(to_play))
        return false;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_824EA0
bool slf__stop_cut_scene__t::operator()(chuck::vm::vm_stack &stack) {
    cut_scene_player* player = references::cut_scene_player.read();

    if (player->is_playing() && !player->is_skipping())
        player->stop(nullptr);

    return true;
}

// sub_876680
bool slf__wait_run_toa_cutscene__dynamic_array__dynamic_array__dynamic_array__dynamic_array__dynamic_array__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18; // all six arguments

    f32 result = 1.0f;

    // returning false keeps the thread waiting here; it comes back with entry set
    if (stack.thread->entry) {
        if (cut_scene_toa_helper::get()->is_running()) {
            retail::sub_80A250((i32)cut_scene_toa_helper::get(), references::g_world_ptr.read()->time_mgr.time_inc);

            return false;
        }

        cut_scene_toa_helper::get()->stop();
    } else {
        chuck::vm::script_dynamic_array<const char*> cutscenes  ((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x00));
        chuck::vm::script_dynamic_array<f32>         button_nums((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04));
        chuck::vm::script_dynamic_array<f32>         button_ids ((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x08));
        chuck::vm::script_dynamic_array<f32>         show_times ((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x0C));
        chuck::vm::script_dynamic_array<f32>         end_times  ((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x10));

        result = 0.0f;

        if ((f32)references::g_world_ptr.read()->get_hero_ptr()->damage_ifc()->hit_points.val > 0.0f &&
            cut_scene_toa_helper::get()->setup(cutscenes, button_nums, button_ids, show_times, end_times, stack.local_num(0x14) > 0.0f))

            return false;
    }

    stack.push(&result, sizeof(result));

    return true;
}
