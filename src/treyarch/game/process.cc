#include "retail.hh"
#include "treyarch/amalga/resource_amalgatoc.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_partition.hh"
#include "treyarch/app/app.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/game/cutscene/cutscene_player.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/igo/igo_3d_loading_screen.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/game_data.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/shared/development_options.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/memory/heap.hh"
#include "treyarch/soap/online.hh"
#include "treyarch/soap/profile.hh"
#include "treyarch/soap/storage.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    // only advance_state_running touches these.
    // the milestone hands the copy of the string to a debug console object once, like KSPS's processCommand(g_console_command, 1);
    // retail keeps only the copy.
    util::memory_reference<u8> unk_01036e68 { 0x01036E68 };

    util::memory_reference<mash::string> unk_01030110 { 0x01030110 };

    // advance_state_unk_2's statics
    util::memory_reference<u8>                          unk_01036e74 { 0x01036E74 };
    util::memory_reference<f32>                         unk_01036e70 { 0x01036E70 };
    util::memory_reference<amalga::resource_partition*> unk_01036e6c { 0x01036E6C };

    // advance_state_load_level's static
    util::memory_reference<u8> unk_01036e44 { 0x01036E44 };

    // .rdata 0
    util::memory_reference<u8> unk_00be9c9f { 0x00BE9C9F };
}} // treyarch::references

using namespace treyarch;

// sub_756080
game_state_e game::get_cur_state() {
    return process_stack.back().get_cur_state();
}

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

// sub_76BC30
void game::soft_reset_process() {
    process_stack.clear();
    push_process(references::main_process.get());
}

// sub_97AFE0
void game::frame_advance_frontend(f32 time_inc) {
    retail::sub_6B1370((u8*)&references::frontend.get(), time_inc, level.unk_03c);
    frame_advance_soap(data);
}

// sub_770310
void game::advance_state_unk_2(f32 time_inc) {
    if (!references::unk_01036e74.read()) {
        engine_recursive_lock &lock = amalga::resource_manager::references::unk_010300a8.get();

        lock.acquire();

        amalga::resource_pack_slot* slot =
            (*amalga::resource_manager::references::partitions.read())[amalga::resource_partition_game]->pack_slots[0];

        lock.release();

        // pushed with both locks held; advance_state_load_level pops and releases
        lock.acquire();
        amalga::resource_manager::references::resource_context_stack_mutex.read()->acquire();

        retail::sub_767760((i32)slot); // push_resource_context

        level.name = &references::level_name_buffer.get();
        level.reset_level_load_data();

        level.look_up_level_descriptor();
        retail::sub_824850((u32*)amalga::resource_manager::references::unk_010f7760.read());
        level.construct_loading_widgets();

        level.unk_03c = 1;

        load_this_level(true, false);

        if (i_quit)
            return;

        references::unk_01036e74.write(1);
    }

    frame_advance_frontend(time_inc);

    amalga::resource_partition* common =
        (*amalga::resource_manager::references::partitions.read())[amalga::resource_partition_common];

    const f32 timer = (f32)((f64)references::unk_01036e70.read() + (f64)time_inc);

    references::unk_01036e70.write(timer);
    references::unk_01036e6c.write(common);

    amalga::resource_amalgatoc* toc = amalga::resource_manager::references::amalgatoc.read();

    // the toc's blocking bit loads everything right away, otherwise it's polled after 4 seconds
    if (toc && (toc->flags & 1)) {
        references::unk_01036e6c.read()->streamer.flush((amalga::resource_pack_streamer::flush_callback)retail::sub_771960,
                                                        0.02f);

        references::unk_01036e70.write(5.0f);
    } else if (timer <= 4.0f)
        return;

    if (references::unk_01036e6c.read()->streamer.is_idle()) {
        level.unk_03c = 2;
        process_stack.back().go_next_state();
    }
}

// sub_75D040
void game::advance_state_load_level(f32 time_inc) {
    level.name = &references::level_name_buffer.get();

    if (!references::unk_01036e44.read()) {
        retail::sub_757240(); // pop_resource_context

        amalga::resource_manager::references::resource_context_stack_mutex.read()->release();
        amalga::resource_manager::references::unk_010300a8.get().release();

        load_this_level(false, true);

        level.unk_03c = 3;
        references::unk_01036e44.write(1);
    }

    retail::sub_77B960(the_world->the_terrain, time_inc); // terrain::frame_advance

    if (!level.load_complete_called)
        return;

    frontend_manager &frontend = references::frontend.get();

    if (retail::sub_687F30((u8*)&frontend) && !references::unk_00be9c9f.read()) {
        frame_advance_frontend(time_inc);

        return;
    }

    retail::sub_687F40((u32**)&frontend);
    level.destroy_loading_widgets();
    retail::sub_707E90((u8*)frontend.igo->loading_screen, 0.0f, 1, 0, 0, 0, 45.0f);

    mission_manager* missions = mission_manager::inst();

    if (missions->first_act_request) {
        retail::sub_980570((i32)missions, references::global_act.read(), 1); // mission_manager::set_current_act

        missions->first_act_request = 0;
    }

    app::inst()->skip_some_frames(3);

    level_is_loaded = 1;
    load_new_level  = 0;

    references::unk_01036e44.write(0);

    // dead ad client

    process_stack.back().go_next_state();
}

// sub_76D700
void game::advance_state_running(f32 time_inc) {
    if (i_quit) {
        unload_current_level();
        soft_reset_process();

        return;
    }

    if (load_new_level) {
        load_new_level = 0;

        unload_current_level();
        soft_reset_process();

        return;
    }

    retail::sub_8095F0((i32)references::cutscene_player.read(), time_inc); // cut_scene_player::frame_advance
    frame_advance_soap(data);

    mission_manager* missions = mission_manager::inst();

    if (missions->is_idle() && missions->script_globals_initialized && !retail::sub_77C9E0(data))
        process_stack.back().go_next_state();

    // retail reduces the stopped-physics branch to an empty call (nullsub_1)
    if (!debug_stop_physics || debug_single_step) {
        // message_board::frame_advance follows the world's; it is empty in retail (nullsub_2)
        if (!wait_for_intro_scene_anim)
            the_world->frame_advance(time_inc);

        frame_advance_game_overlays(time_inc);
        debug_single_step = 0;
    }

    if (!references::unk_01036e68.read()) {
        references::unk_01036e68.write(1);

        (void)mash::string(references::unk_01030110.get());
    }
}

// sub_72C930
void game::advance_state_paused(f32 time_inc) {
    if (unk_059)
        retail::sub_A1AE50((u32*)chuck::vm::script_manager::inst(), time_inc, 0); // script_manager::run

    frame_advance_game_overlays(time_inc);

    // the same sequence as frame_advance_soap, minus its soap switches
    retail::sub_9EDF60(); // the message box and notification managers' frame_advance

    soap::profile::inst()->frame_advance();
    soap::storage::inst()->frame_advance();
    soap::online::inst()->frame_advance();

    data->frame_advance();

    // retail ends with an empty call (nullsub_1)
}

// sub_76BBD0
void game::advance_state_credits(f32) {
    process_stack.back().go_next_state();

    retail::sub_8FF2F0();
    retail::sub_707E90((u8*)references::frontend.get().igo->loading_screen, 0.0f, 0, 0, 0, 0, 45.0f);

    clear_screen();

    retail::sub_76BA90((u32*)this, "credits"); // push movie process
}

// sub_770480
void game::handle_game_states(f32* time_inc) {
    /* the static flows only reach some of these states:
       start (1, 2, 3, 4, 5, 20), main (8, 9, 10, 11, 20), pause (12, 20), movie (18, 20) */
    switch ((i32)process_stack.back().get_cur_state()) {
        case 1:
            advance_state_legal(*time_inc);

            return;

        case 2:
            advance_state_unk_2(*time_inc);

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
            frame_advance_frontend(*time_inc);
            process_stack.back().go_next_state();

            if (!references::restart_skip_the_title.read())
                retail::sub_76BA90((u32*)this, "aspyr");

            return;

        case 6:
        case 7:
            frame_advance_frontend(*time_inc);
            process_stack.back().go_next_state();

            return;

        case 8:
            advance_state_load_level(*time_inc);

            return;

        case 9:
            advance_state_running(*time_inc);

            return;

        case 10:
            advance_state_credits(*time_inc);

            return;

        case 11:
            retail::sub_9C8B90();

            return;

        case 12:
            advance_state_paused(*time_inc);

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

// sub_76F6F0
void game::advance_state_legal(f32) {
    // retail constructs this and never reads it
    hires_clock_t unused_clock;

    retail::sub_72C8C0();

    amalga::resource_partition* partition =
        (*amalga::resource_manager::references::partitions.read())[amalga::resource_partition_game];

    partition->streamer.load_callback = (amalga::resource_pack_slot_callback)retail::sub_7DD220;
    partition->streamer.load("game", 0);
    partition->streamer.flush((amalga::resource_pack_streamer::flush_callback)retail::sub_771950, 0.02f);

    engine_recursive_lock &lock = amalga::resource_manager::references::unk_010300a8.get();

    lock.acquire();

    amalga::resource_pack_slot* slot =
        (*amalga::resource_manager::references::partitions.read())[amalga::resource_partition_game]->pack_slots[0];

    lock.release();

    {
        amalga::push_resource_context_stack_object context(slot);

        void* previous = references::unk_00fc64ec.read();

        if (previous) {
            retail::sub_5416B0((u32*)previous); // its destructor
            memory::heap::free(previous);
        }

        references::unk_00fc64ec.write(nullptr);

        retail::sub_99CC10();
        retail::sub_736F80();
        retail::sub_6ABA30();
        one_time_init_stuff();

        process_stack.back().go_next_state();
    }
}
