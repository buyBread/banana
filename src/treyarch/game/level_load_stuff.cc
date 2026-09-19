#include "treyarch/game/game.hh"

using namespace treyarch;

level_load_stuff::level_load_stuff() : descriptor(nullptr),
                                       name("m0_arena"),
                                       hero_name("ch_spiderman"),
                                       level_clock() {

    reset_level_load_data();
}

void level_load_stuff::reset_level_load_data() {
    descriptor               = nullptr;
    loading_meter_val        = 0;
    load_complete_called     = 0;
    load_this_level_finished = 0;
    load_widgets_created     = 0;
}