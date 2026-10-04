#include "retail.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/cutscene/cutscene_player.hh"
#include "treyarch/game/cutscene/toa_cutscene.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/frontend/igo/igo_3d_loading_screen.hh"
#include "treyarch/game/frontend/igo/igo_3d_scrapbook.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/game_data.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/region_pack_manager.hh"
#include "treyarch/game/summon_state.hh"
#include "treyarch/soap/message_box_manager.hh"
#include "treyarch/soap/notification_manager.hh"
#include "treyarch/soap/online.hh"
#include "treyarch/soap/profile.hh"
#include "treyarch/soap/soap.hh"
#include "treyarch/soap/storage.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

// sub_7973D0
void game_data::read_notifications() {
    soap::notification_list* notifications = ((soap::notification_manager*)retail::sub_9EF540())->notifications();

    for (i32* notification = notifications->begin(); notification != notifications->end(); ++notification) {
        if (*notification == 0)
            m->blocked_by_notification = 1;
        else if (*notification == 1)
            m->blocked_by_notification = 0;
    }
}

// sub_7A19F0
void game_data::frame_advance() {
    read_notifications();

    if (m->online_context_requested && !m->online_context_set) {
        m->online_context_set = 1;

        if (soap::references::enable_online.read()) {
            retail::sub_9EB5D0((u32*)retail::sub_9ED060(), 1, 4); // online::set_context
            ((soap::online*)retail::sub_9ED060())->method_024(0);
        }
    }

    auto* boxes = (soap::message_box_manager*)retail::sub_9EEC20();

    if (m->blocked_by_notification || boxes->is_box_showing())
        return;

    if (((soap::profile*)retail::sub_9ED670())->busy())
        return;

    if (((soap::storage*)retail::sub_9EDA50())->busy())
        return;

    if (m->saving_or_loading) {
        m->autosave_pending    = 0;
        m->idle_frames         = 0;
        m->saving_or_loading   = 0;
        m->request_outstanding = 0;

        if (m->created_new_slot) {
            ((soap::storage*)retail::sub_9EDA50())->method_048();

            for (i32 slot = 0; slot < 4; ++slot) {
                soap::storage_article* article = ((soap::storage*)retail::sub_9EDA50())->article(0, slot);

                if ((u8)retail::sub_9ED910((u32*)article)) // article state is 3
                    m->slot_status[slot] = 3;
            }

            m->created_new_slot = 0;
        }
    }

    if (m->state != game_data_state_idle) {
        if (((soap::storage*)retail::sub_9EDA50())->must_show_active_device_unavailable) {
            boxes->show(soap::message_box_storage_active_device_unavailable);

            m->state        = game_data_state_device_unavailable;
            m->request_kind = game_data_request_enumerate;

            ((soap::storage*)retail::sub_9EDA50())->set_must_show_active_device_unavailable(false);
        } else
            retail::sub_79CAF0((u32*)this); // one step of the state machine

        return;
    }

    if (m->request_kind == game_data_request_copy_slot)
        retail::sub_77C8C0((u32*)this, m->requested_slot, 1); // the load request

    if (m->waiting_for_result) {
        if (((soap::storage*)retail::sub_9EDA50())->test_04c()) {
            m->waiting_for_result = 0;
            m->request_kind       = game_data_request_plain;
        } else {
            // an enumerate is consumed right away, a load has to wait for a quiet moment
            if (m->request_kind != game_data_request_enumerate) {
                summon_state* summons = references::summon_state.read();

                if (summons->active_summon) {
                    retail::sub_90FC00(summons); // end the summon

                    return;
                }

                if (retail::sub_920420((u32*)references::region_pack_manager.read()))
                    return;

                mission_manager* missions = references::mission_manager.read();

                if (missions->is_mission_running()) {
                    retail::sub_9855F0((i32)missions, 0, 1); // mission_manager::terminate_script

                    return;
                }

                if (!missions->first_act_request && !missions->is_idle())
                    return;
            }

            u8 keep_waiting = 0;

            retail::sub_79CDB0((u32*)this, &keep_waiting); // game_data::load_remainder

            if (!keep_waiting)
                m->waiting_for_result = 0;
        }
    }

    // state 8 fills in the level name before a level loads
    if (((soap::storage*)retail::sub_9EDA50())->must_show_active_device_unavailable &&
        (i32)references::game.read()->get_cur_state() != 8) {

        bool open_scrapbook = true;

        if (references::cutscene_player.read() && references::cutscene_player.read()->is_playing())
            open_scrapbook = false;

        if (toa_cutscene::get() && toa_cutscene::get()->is_running())
            open_scrapbook = false;

        if (!references::frontend.get().igo->scrapbook->is_active() && open_scrapbook) {
            references::frontend.get().igo->scrapbook->activate(0);

            retail::sub_6BC990((u32*)references::frontend.get().igo->scrapbook, 2, 0, 1);
        }
    }

    ++m->idle_frames;

    if (m->autosave_pending && !references::game.read()->game_paused) {
        mission_manager* missions = references::mission_manager.read();

        if (!m->autosave_enabled) {
            m->autosave_pending = 0;
            m->idle_frames      = 0;
            m->force_autosave   = 0;
        } else if (m->force_autosave || missions->is_idle() || missions->mission_finished_screen_has_appeared) {
            // idle_frames also counts up above, so this waits about 1800 frames
            if (!m->force_autosave && m->idle_frames <= 3600)
                ++m->idle_frames;
            else {
                if (m->slot_status[0] == 3) {
                    m->autosave_pending = 0;
                    m->idle_frames      = 0;
                } else if (retail::sub_797420((u32*)this, 1, 1)) { // game_data::save
                    m->autosave_pending = 0;
                    m->idle_frames      = 0;
                }

                m->force_autosave = 0;
            }
        }
    }

    bool interface_free = !references::game.read()->disable_interface &&
                          !retail::sub_967DF0() &&
                          !references::cutscene_player.read()->is_playing();

    bool loading_screen_visible = references::frontend.get().igo->loading_screen->is_visible();

    if (m->unk_0bd && interface_free && !loading_screen_visible)
        m->unk_0bd = 0;

    if (!m->unk_0be)
        return;

    if (m->unk_0bf &&
        (references::cutscene_player.read()->is_playing() || retail::sub_6884D0((u32*)references::frontend.get().igo)))

        return;

    m->unk_0be = 0;

    if (m->unk_0bf) {
        if (!references::game.read()->game_paused) {
            retail::sub_97C970((i32)references::game.read()); // game::pause_action

            m->unk_0c0 = 1;
        }
    } else if (m->unk_0c0) {
        m->unk_0c0 = 0;

        if (references::game.read()->game_paused)
            retail::sub_97BB70((i32)references::game.read()); // game::unpause_action
    }
}

// sub_76BAF0
void treyarch::frame_advance_soap(game_data* data) {
    if (soap::references::enable_profiles.read() ||
        soap::references::enable_storage .read() ||
        soap::references::enable_online  .read()) {

        retail::sub_9EDF60(); // the message box and notification managers' frame_advance

        if (soap::references::enable_profiles.read())
            ((soap::profile*)retail::sub_9ED670())->frame_advance();

        if (soap::references::enable_storage.read())
            ((soap::storage*)retail::sub_9EDA50())->frame_advance();

        if (soap::references::enable_online.read())
            ((soap::online*)retail::sub_9ED060())->frame_advance();
    }

    data->frame_advance();

    retail::sub_76B430(); // achievement_tracker::inst, result unused
}
