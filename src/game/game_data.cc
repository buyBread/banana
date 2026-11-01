#include "retail.hh"
#include "game/app.hh"
#include "game/cutscene/cut_scene_player.hh"
#include "game/cutscene/cut_scene_toa_helper.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/igo/igo_3d_loading_screen.hh"
#include "game/frontend/igo/igo_3d_scrapbook.hh"
#include "game/frontend/igo/igo_3d_zoom_map.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/game.hh"
#include "game/game_data.hh"
#include "game/game_meter_manager.hh"
#include "game/math/references.hh"
#include "game/mission/mission_manager.hh"
#include "game/region_pack_manager.hh"
#include "game/script/script_access.hh"
#include "game/summon_state.hh"
#include "game/wds/camera/camera.hh"
#include "game/wds/entity/interface/combo_interface.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/soap/message_box_manager.hh"
#include "treyarch/soap/notification_manager.hh"
#include "treyarch/soap/online.hh"
#include "treyarch/soap/profile.hh"
#include "treyarch/soap/soap.hh"
#include "treyarch/soap/storage.hh"
#include "banana/util/memory_reference.hh"

using namespace treyarch;

// sub_7973D0
void game_data::read_notifications() {
    soap::notification_list* notifications = soap::notification_manager::inst()->notifications();

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
            retail::sub_9EB5D0((u32*)soap::online::inst(), 1, 4); // online::set_context
            soap::online::inst()->method_024(0);
        }
    }

    auto* boxes = soap::message_box_manager::inst();

    if (m->blocked_by_notification || boxes->is_box_showing())
        return;

    if (soap::profile::inst()->busy())
        return;

    if (soap::storage::inst()->busy())
        return;

    if (m->saving_or_loading) {
        m->autosave_pending    = 0;
        m->idle_frames         = 0;
        m->saving_or_loading   = 0;
        m->request_outstanding = 0;

        if (m->created_new_slot) {
            soap::storage::inst()->method_048();

            for (i32 slot = 0; slot < 4; ++slot) {
                soap::storage_article* article = soap::storage::inst()->article(0, slot);

                if ((u8)retail::sub_9ED910((u32*)article)) // article state is 3
                    m->slot_status[slot] = 3;
            }

            m->created_new_slot = 0;
        }
    }

    if (m->state != game_data_state_idle) {
        if (soap::storage::inst()->must_show_active_device_unavailable) {
            boxes->show(soap::message_box_storage_active_device_unavailable);

            m->state        = game_data_state_device_unavailable;
            m->request_kind = game_data_request_enumerate;

            soap::storage::inst()->set_must_show_active_device_unavailable(false);
        } else
            retail::sub_79CAF0((u32*)this); // one step of the state machine

        return;
    }

    if (m->request_kind == game_data_request_copy_slot)
        retail::sub_77C8C0((u32*)this, m->requested_slot, 1); // the load request

    if (m->waiting_for_result) {
        if (soap::storage::inst()->test_04c()) {
            m->waiting_for_result = 0;
            m->request_kind       = game_data_request_plain;
        } else {
            // an enumerate is consumed right away, a load has to wait for a quiet moment
            if (m->request_kind != game_data_request_enumerate) {
                summon_state* summons = references::summon_state.read();

                if (summons->active_summon) {
                    summons->end_requested = true;

                    return;
                }

                if (retail::sub_920420((u32*)region_pack_manager::inst()))
                    return;

                mission_manager* missions = mission_manager::inst();

                if (missions->is_mission_running()) {
                    missions->terminate_script(false, true);

                    return;
                }

                if (!missions->first_act_request && !missions->is_idle())
                    return;
            }

            u8 keep_waiting = 0;

            load_remainder(&keep_waiting);

            if (!keep_waiting)
                m->waiting_for_result = 0;
        }
    }

    // state 8 fills in the level name before a level loads
    if (soap::storage::inst()->must_show_active_device_unavailable &&
        (i32)references::game.read()->get_cur_state() != 8) {

        bool open_scrapbook = true;

        if (references::cut_scene_player.read() && references::cut_scene_player.read()->is_playing())
            open_scrapbook = false;

        if (cut_scene_toa_helper::get() && cut_scene_toa_helper::get()->is_running())
            open_scrapbook = false;

        if (!references::frontend.get().igo->scrapbook->is_active() && open_scrapbook) {
            references::frontend.get().igo->scrapbook->activate(false);

            retail::sub_6BC990((u32*)references::frontend.get().igo->scrapbook, 2, 0, 1);
        }
    }

    ++m->idle_frames;

    if (m->autosave_pending && !references::game.read()->game_paused) {
        mission_manager* missions = mission_manager::inst();

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
                          !references::cut_scene_player.read()->is_playing();

    bool loading_screen_visible = references::frontend.get().igo->loading_screen->is_shown();

    if (m->unk_0bd && interface_free && !loading_screen_visible)
        m->unk_0bd = 0;

    if (!m->unk_0be)
        return;

    if (m->unk_0bf &&
        (references::cut_scene_player.read()->is_playing() || retail::sub_6884D0((u32*)references::frontend.get().igo)))

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

// sub_79CDB0
bool game_data::load_remainder(u8* keep_waiting) {
    if (!retail::sub_7342B0(retail::sub_76B430())) // achievement_tracker::inst, then whether its data is in
        return false;

    if (soap::storage::inst()->busy()) {
        *keep_waiting = 1;

        return true;
    }

    if (m->request_kind == game_data_request_enumerate) {
        *keep_waiting         = 0;
        m->enumerate_finished = 1;

        for (u32 slot = 0; slot < 4; ++slot) {
            game_data_slot_record* record = slot < 4 ? &m->slot_records[slot] : nullptr;

            if (!soap::storage::inst()->article_exists(0, slot)) {
                record->valid        = 0;
                m->slot_status[slot] = 0;

                continue;
            }

            soap::storage_article* article = soap::storage::inst()->article(0, slot);

            retail::sub_9EDC90((u32*)&record->save_time, (u32*)&article->timestamp); // tm::operator=

            record->valid        = 1;
            m->slot_status[slot] = 1;

            if ((u8)retail::sub_9ED900((u32*)article)) // article state is 2
                m->slot_status[slot] = 2;
            else if ((u8)retail::sub_9ED910((u32*)article)) // article state is 3
                m->slot_status[slot] = 3;
        }

        m->request_kind = game_data_request_plain;

        return true;
    }

    m->buffer_size = 0;

    soap::storage* storage = soap::storage::inst();
    soap::storage* source  = soap::storage::inst();

    u32 size = (u32)retail::sub_A79D60((u32*)storage); // the save data's size

    retail::sub_789F60((i32)&m->buffer, (void*)retail::sub_9EDBF0((u32*)source), size); // appended to the emptied save buffer

    if (m->request_kind == game_data_request_read_then_save) {
        m->request_kind    = game_data_request_copy_slot;
        m->state           = game_data_state_sign_in;
        m->is_load_request = 0;
    } else if (m->apply_loaded_data) {
        retail::sub_78A5A0();           // resets the ui, the mission clocks, city life, the held region packs and the upgrades
        retail::sub_78A610((u32*)this); // reads the save buffer
        soft_load();
    }

    *keep_waiting = 0;

    return true;
}

// sub_7976E0
void game_data::soft_load() {
    references::game.read()->unk_1cd = 0;

    export_game_settings();

    retail::sub_78AB70((u32*)this); // reads the saved settings

    entity* hero = references::g_world_ptr.read()->hero_ptr;

    hero->player_ifc()->unk_369(0, 1);

    if (hero->has_standing_ifc()) {
        hero->standing_ifc()->enable(true);
        hero->standing_ifc()->manage_standing(true, 1.0f);
    }

    hero->player_ifc()->enable_axis(0, true);
    hero->player_ifc()->enable_axis(1, true);
    hero->player_ifc()->enable_axis(2, true);

    for (i32 ctrl = 0; ctrl < 11; ++ctrl)
        hero->player_ifc()->enable_button(ctrl, true);

    hero->player_ifc()->enable_button(12, true);

    player_interface* player = hero->player_ifc();

    player->clear_buttons();
    player->override_mode = false;

    world_dynamics_system* world = references::g_world_ptr.read();

    if (world->camera_mgr.marky_camera_enabled)
        references::game.read()->enable_marky_cam(false, false, ((marky_camera*)world->camera_mgr.marky_camera)->current_priority, 0.0f);

    string_hash function;
    function.initialize(mash::ALLOCATED, "on_soft_load()");

    script::run_thread(script::create_thread(nullptr, function));
}

// sub_78A890
void game_data::export_game_settings() {
    world_dynamics_system* world = references::g_world_ptr.read();

    if (world && world->hero_ptr) {
        vector3d position(0.0f, 0.0f, 0.0f);

        if (world->the_terrain) {
            string_hash name;
            name.initialize(mash::ALLOCATED, nullptr, m->save_record.unk_00);

            if (auto* found = (u8*)retail::sub_789960((i32)name.source_hash_code))
                position = *(const vector3d*)(found + 0xD0);
        }

        position = m->save_record.hero_position;

        const vector3d &origin = references::unk_01073f40.get();

        f32 dx = (f32)((f64)position.x - (f64)origin.x);
        f32 dy = (f32)((f64)position.y - (f64)origin.y);
        f32 dz = (f32)((f64)position.z - (f64)origin.z);

        f32 distance_sq = (f32)((f64)dx * (f64)dx + (f64)dy * (f64)dy + (f64)dz * (f64)dz);

        if (!(distance_sq <= 0.0001f))
            references::g_world_ptr.read()->malor_point(position, false, true);
        else
            retail::sub_981C90((i32)mission_manager::inst());

        igo_3d_zoom_map* zoom_map = references::frontend.get().igo->zoom_map;

        if (zoom_map->is_open)
            zoom_map->snap_to_point(position.x, position.z);

        entity* hero = references::g_world_ptr.read()->hero_ptr;

        if (hero) {
            if (hero->has_ifc(entity_ifc_damage)) {
                if ((i32)m->save_record.hero_hit_points)
                    hero->damage_ifc()->set_hit_points((i32)m->save_record.hero_hit_points);
                else
                    hero->damage_ifc()->set_hit_points(hero->damage_ifc()->get_max_hit_points());
            }

            if (hero->has_ifc(entity_ifc_combo)) {
                hero->combo_ifc()->unk_034 = m->save_record.unk_44;
                hero->combo_ifc()->unk_038 = m->save_record.unk_48;
            }
        }
    }

    if (game_meter_manager* meters = references::game_meter_manager.read())
        retail::sub_933C10((i32*)meters, 0, 0.0f);
}

// sub_76BAF0
void treyarch::frame_advance_soap(game_data* data) {
    if (soap::references::enable_profiles.read() ||
        soap::references::enable_storage .read() ||
        soap::references::enable_online  .read()) {

        retail::sub_9EDF60(); // the message box and notification managers' frame_advance

        if (soap::references::enable_profiles.read())
            soap::profile::inst()->frame_advance();

        if (soap::references::enable_storage.read())
            soap::storage::inst()->frame_advance();

        if (soap::references::enable_online.read())
            soap::online::inst()->frame_advance();
    }

    data->frame_advance();

    retail::sub_76B430(); // achievement_tracker::inst, result unused
}
