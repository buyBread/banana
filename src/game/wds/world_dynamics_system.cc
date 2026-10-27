#include "retail.hh"
#include "game/sm_mash.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_97A860
world_dynamics_system::world_dynamics_system() : unk_034(0),
                                                 unk_038(false),
                                                 unk_039(false),
                                                 saved_hero_enabled(true) {

    unk_01c          = 0;
    gv_busy_teaching = nullptr;
    unk_139          = false;
    hero_ptr         = nullptr;
    chase_cam_ptr    = nullptr;
    unk_13c          = -1;
    unk_134          = 4;
    the_terrain      = nullptr;
    last_snm_handle  = 0;

    sm_generate_vtable();

    retail::sub_A721A0(); // the ise library's own registration list
    retail::sub_600E20();
}
