#include <cstring>

#include "retail.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/shared/memory/heap.hh"

using namespace treyarch;

// sub_97D590
game::game() {
    hero_freeze_depth = 0;
    unk_078           = 0;
    unk_07c           = 0;

    unk_1cc = 0;
    unk_1cd = 0;
    unk_1ce = 0;

    retail::sub_773A80();
    retail::sub_97AB10();
    retail::sub_843300();
    retail::sub_83EF20();
    retail::sub_603E00();

    level_time     = 0.0f;
    frame_sequence = 0;

    mash::string level_name(references::default_level_name.get());
    strcpy(&references::level_name_buffer.get(), level_name.c_str());

    void* world_allocation = memory::heap::allocate(sizeof(world_dynamics_system));
    the_world = world_allocation ? (world_dynamics_system*)retail::sub_97A860(world_allocation) : nullptr;
    references::g_world_ptr.write(the_world);

    mb = nullptr;

    void* data_allocation = memory::heap::allocate(8);
    game_data* constructed_data = data_allocation ?
        (game_data*)retail::sub_7A4FB0((u32*)data_allocation) : nullptr;

    frame_timing.total_delta = 0.0f;
    frame_timing.flip_delta  = 0.0f;
    frame_timing.limit_delta = 0.0f;
    current_frame_delta      = 0.0f;
    data                     = constructed_data;
    unk_198                  = 0;

    disable_interface               = 0;
    disable_start_menu              = 0;
    i_quit                          = 0;
    level_is_loaded                 = 0;
    level_is_unloading              = 0;
    load_new_level                  = 0;
    unk_04d                         = 1;
    debug_single_step               = 0;
    debug_stop_physics              = 0;
    unk_050                         = 0;
    unk_054                         = 0.45f;
    game_paused                     = 0;
    use_default_hero_start_position = 1;
    wait_for_intro_scene_anim       = 0;

    base_camera         = nullptr;
    current_game_camera = nullptr;
    current_view_camera = nullptr;
    unk_078             = 0;
    string_localizer    = nullptr;

    retail::sub_738990();
    references::unk_01110e90.write('\0');

    push_process(references::main_process.get());
    push_process(references::start_process.get());

    unk_1cc = 0;
    unk_1cd = 0;
    unk_1ac = 0;

    retail::sub_95A2F0();
    retail::sub_77EA20();
    retail::sub_7B48D0();
}
