#include "retail.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/game/cutscene/cut_scene.hh"
#include "treyarch/game/cutscene/cut_scene_player.hh"
#include "treyarch/game/region_pack_manager.hh"
#include "treyarch/game/script/chuck_lib_scene_anim.hh"
#include "treyarch/game/summon_state.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"

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

bool slf__wait_run_toa_cutscene__dynamic_array__dynamic_array__dynamic_array__dynamic_array__dynamic_array__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_876680)(this, stack);
}
