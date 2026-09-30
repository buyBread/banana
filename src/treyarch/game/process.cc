#include "retail.hh"
#include "treyarch/game/game.hh"
#include "treyarch/shared/development_options.hh"

using namespace treyarch;

// sub_76BA50
void game::push_process(const game_process &process) {
    process_stack.push_back(process);
    process_stack.back().index = 0;
    process_stack.back().timer = 0.0f;
}

// sub_764E80
void game::pop_process() {
    if (process_stack.size())
        process_stack.pop_back();
}

// sub_770480
void game::handle_game_states(f32* time_inc) {
    /*
        the static flows only reach some of these states:
        start (1, 2, 3, 4, 5, 20), main (8, 9, 10, 11, 20), pause (12, 20), movie (18, 20)
    */
    switch ((i32)process_stack.back().get_cur_state()) {
        case 1:
            retail::sub_76F6F0((u32*)this, *time_inc);

            return;

        case 2:
            retail::sub_770310((i32)this, *time_inc);

            return;

        case 3:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 19:
            process_stack.back().go_next_state();

            return;

        case 4:
            retail::sub_76BB70((u32*)this, *time_inc, 1);
            
            return;

        case 5:
            retail::sub_97AFE0((u32*)this, *time_inc);
            process_stack.back().go_next_state();

            if (!references::restart_skip_the_title.read())
                retail::sub_76BA90((u32*)this, "aspyr");

            return;

        case 6:
        case 7:
            retail::sub_97AFE0((u32*)this, *time_inc);
            process_stack.back().go_next_state();

            return;

        case 8:
            retail::sub_75D040((i32)this, *time_inc);

            return;

        case 9:
            retail::sub_76D700((i32)this, *time_inc);

            return;

        case 10:
            retail::sub_76BBD0((u32*)this, *time_inc);

            return;

        case 11:
            retail::sub_9C8B90();

            return;

        case 12:
            retail::sub_72C930((i32)this, *time_inc);

            return;

        case 18:
            retail::sub_75D180((u32*)this, *time_inc);

            return;

        case 20:
            pop_process();

            return;

        default:
            return;
    }
}
