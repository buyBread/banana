#include <algorithm>
#include <bit>
#include <cstring>

#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/app.hh"
#include "game/cutscene/cut_scene.hh"
#include "game/cutscene/cut_scene_player.hh"
#include "game/event/anim_event.hh"
#include "game/event/event_manager.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/frontend/igo/igo_3d_zoom_map.hh"
#include "game/game.hh"
#include "game/glass_house_manager.hh"
#include "game/input/input_mgr.hh"
#include "game/mission/mission_manager.hh"
#include "game/vehicle_model.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/camera/camera.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/interface/morph_interface.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/vehicle.hh"
#include "game/wds/references.hh"
#include "game/wds/region.hh"
#include "game/wds/terrain.hh"
#include "game/hash/algo.hh"
#include "treyarch/mash/avl_tree.hh"
#include "game/math/po.hh"

using namespace treyarch;

// sub_8079F0
cut_scene_player::cut_scene_player() : current_cut_scene(nullptr),
                                       spawned_entities(),
                                       lip_sync_entities(),
                                       forced_regions(),
                                       state_flags(0),
                                       saved_glass_house_level(-1),
                                       loops_left(0),
                                       fade_state(0),
                                       fade_timer(-1.0f),
                                       fade_hold_time(0.0f),
                                       fade_in_time(0.0f),
                                       skip_buttons{ game_button(1), game_button(11) },
                                       loading_time(0.0f),
                                       segment_time(0.0f),
                                       scene_anim_handle((scene_anim_handle_t)-1),
                                       current_scene_anim(nullptr) {}

// sub_428BC0
void cut_scene_player::create_inst() {
    references::cut_scene_player.write(new cut_scene_player());
}

// sub_805020
bool cut_scene_player::is_playing() const {
    return (state_flags & playing)  != 0 ||
           (state_flags & skipping) != 0 ||
           fade_state;
}

// sub_805050
bool cut_scene_player::is_skipping() const {
    return (state_flags & skipping) != 0;
}

// sub_805060
cut_scene* cut_scene_player::get_current_cut_scene() const {
    return current_cut_scene;
}

// sub_8054F0
bool cut_scene_player::hero_unaffected() const {
    return current_cut_scene ? (current_cut_scene->flags & cut_scene::hero_unaffected) != 0 : false;
}

// sub_8056E0
bool cut_scene_player::has_sound() const {
    return current_cut_scene ? current_cut_scene->has_sound() : false;
}

// sub_805680
f32 cut_scene_player::get_sound_time() const {
    f32 sound_time = segment_time;

    // the cut scene's sound stream is playing
    if (retail::sub_8FF160())
        sound_time = (f32)retail::sub_8FF100(nullptr);

    f32 result = sound_time - get_current_segment_start_time();

    return result > 0.0f ? result : 0.0f;
}

// sub_8095F0
void cut_scene_player::frame_advance(f32 time_inc) {
    if (!frame_advance_lite(time_inc))
        return;

    time_inc = std::min(time_inc, 0.1f);

    bool skip_all     = false;
    bool skip_segment = false;

    skip_buttons[0].update(time_inc, false, false);
    skip_buttons[1].update(time_inc, false, false);

    if ((skip_buttons[1].triggered() || skip_buttons[0].triggered()) && segment_time > 2.0f) {
        if (current_cut_scene->flags & cut_scene::skippable) {
            state_flags  |= skipping;
            skip_all      = true;
            skip_segment  = true;
        } else if ((*current_segment)->flags & cut_scene_segment::skippable) {
            state_flags  |= skipping;
            skip_segment  = true;
        } else
            state_flags &= ~skipping;
    } else
        state_flags &= ~skipping;

    if (!current_cut_scene)
        return;

    world_dynamics_system* world = references::g_world_ptr.read();

    if (skip_all || skip_segment) {
        world->scene_anim_time_remaining(scene_anim_handle); // the result goes unused

        if (skip_all)
            time = get_total_time();
        else if (skip_segment)
            time = get_current_segment_end_time();
    }

    bool finished = world->is_scene_anim_finished(scene_anim_handle);

    raise_animation_events(&current_cut_scene->entity_class_entries, time, last_time, skip_segment || skip_all);
    raise_hero_animation_events(time, last_time, skip_segment || skip_all);

    if (references::skip_next_segment_step.read()) {
        references::skip_next_segment_step.write(false);

        return;
    }

    if (!finished && !skip_segment)
        return;

    if (current_segment + 1 != current_cut_scene->segments.end() && !skip_all) {
        release_current_segment();

        current_segment++;

        play_current_segment();
    } else if (!--loops_left || skip_all)
        stop(nullptr);
    else {
        release_current_segment();

        current_segment = current_cut_scene->segments.begin();

        play_current_segment();
    }
}

// sub_8093C0
bool cut_scene_player::frame_advance_lite(f32 time_inc) {
    time_inc = std::min(time_inc, 0.1f);

    update_focus();

    if (state_flags & playing) {
        player_interface* player = references::g_world_ptr.read()->get_hero_ptr()->player_ifc();

        last_time = time;
        time      = get_time();

        ((void (__thiscall*)(player_interface*, f32))((void**)player->vtable)[540])(player, time - last_time); // slot 540
    }

    if (!advance_fade(time_inc)) {
        if (state_flags & playing) {
            raise_animation_events(&current_cut_scene->entity_class_entries, time, last_time, false);
            raise_hero_animation_events(time, last_time, false);
        }

        return false;
    }

    if (!(state_flags & playing))
        return false;

    if (state_flags & hooked_up) {
        segment_time = (f32)((f64)segment_time + (f64)time_inc);

        return true;
    }

    if (loading_time >= 60.0)
        stop(nullptr);

    if (!districts_loaded(time_inc))
        return false;

    start_current_segment();

    state_flags  |= hooked_up;
    segment_time  = 0.0f;

    hookup_current_segment();

    return true;
}

// sub_809850
bool cut_scene_player::play(cut_scene* to_play) {
    using change_suit_method   = void (__thiscall*)(player_interface* self, f32 unk, bool black);
    using is_black_suit_method = bool (__thiscall*)(player_interface* self);

    if (!to_play || !to_play->play_count || !to_play->segments.size)
        return false;

    if (!std::strncmp(to_play->get_name().m_data, "igc_ele_shot", 0xFFFF))
        to_play->flags &= ~cut_scene::skippable;

    world_dynamics_system* world = references::g_world_ptr.read();

    if (world->camera_mgr.marky_camera)
        retail::sub_640910((u32*)world->camera_mgr.marky_camera); // entity_base::clear_parent

    entity* hero = references::g_world_ptr.read()->get_hero_ptr();

    if (hero->flags & entity_flag_collisions_active)
        state_flags |= hero_collisions_active;
    else
        state_flags &= ~hero_collisions_active;

    player_interface* player = hero->player_ifc();

    auto change_suit = (change_suit_method)((void**)player->vtable)[572];

    if (to_play->red_suit)
        change_suit(player, 0.0f, false);
    else if (to_play->black_suit)
        change_suit(player, 0.0f, true);
    else
        change_suit(player, 0.0f, ((is_black_suit_method)((void**)player->vtable)[305])(player));

    state_flags &= ~skipping;

    saved_glass_house_level = references::glass_house_manager.read()->glass_house_level;

    stop(nullptr);

    retail::sub_730F10((u32*)references::glass_house_manager.read(), -1); // set_glass_house_level

    current_cut_scene = to_play;
    loops_left        = (to_play->flags & cut_scene::play_once) ? 1 : to_play->play_count;
    current_segment   = to_play->segments.begin();

    start_entities(&current_cut_scene->entity_class_entries);
    start_fade();

    if (current_cut_scene->flags & cut_scene::takes_igo)
        take_igo(false);

    if (!(current_cut_scene->flags & cut_scene::hero_unaffected))
        app::inst()->get_game()->freeze_hero(true);

    state_flags |= playing;

    clamp_settings();

    if (!(current_cut_scene->flags & cut_scene::hero_unaffected)) {
        player_interface* hero_player = hero->player_ifc();

        ((void (__thiscall*)(player_interface*, i32, i32))((void**)hero_player->vtable)[369])(hero_player, 0, 1); // slot 369

        if (hero->player_ifc()->get_reflexes())
            hero->player_ifc()->get_reflexes()->disable(false);

        if (hero->player_ifc()->get_spider_senses())
            hero->player_ifc()->get_spider_senses()->disable(true, false);

        if (hero->has_ifc(entity_ifc_standing)) {
            retail::sub_60CF20((u32*)hero->my_ifc_storage->get_ifc(entity_ifc_standing));

            hero->standing_ifc()->enable(false);
        }

        i32 unk_object = retail::sub_602F30((u32*)hero);

        if (unk_object) {
            i32 unk_result = ((i32 (__thiscall*)(i32, i32))(*(void***)unk_object)[1])(unk_object, 0); // slot 1

            if (unk_result)
                retail::sub_5BBBF0((u32*)unk_result, unk_object, true);
        }

        if (hero->has_ifc(entity_ifc_ise)) {
            generic_interface* ise = hero->my_ifc_storage->get_ifc(entity_ifc_ise);

            ((void (__thiscall*)(generic_interface*))((void**)ise->vtable)[21])(ise); // slot 21
        }

        ((actor*)hero)->get_ai_core()->idle_type = 1;
    }

    auto chase = (spiderman_camera*)references::g_world_ptr.read()->get_chase_cam_ptr();

    if (chase->is_a_spiderman_camera())
        chase->unk_278 = (current_cut_scene->flags & 0x0010) != 0;

    for (auto it = current_cut_scene->entity_class_entries.begin(); it.node; it.iterate()) {
        auto shown = (entity*)retail::sub_7DF6E0((u32*)it.node->element);

        if (shown) {
            shown->ext_flags |= 0xF;

            retail::sub_6123B0((u32*)shown, 0xF, (i32)0xDEADBEEF);
        }
    }

    time      = 0.0f;
    last_time = 0.0f;

    retail::sub_7F7340((u32*)input_mgr::inst()->unk_004);

    reset_interfaces(&to_play->entity_class_entries);

    return true;
}

// sub_805220
void cut_scene_player::clamp_settings() {
    if (!current_cut_scene)
        return;

    i32 unk_58 = current_cut_scene->unk_58;

    if (unk_58 < -1)
        unk_58 = -1;
    else if (unk_58 > 10)
        unk_58 = 10;

    current_cut_scene->unk_58 = unk_58;

    if (current_cut_scene->unk_5c < -1.0f)
        current_cut_scene->unk_5c = -1.0f;
    else if (current_cut_scene->unk_5c > 20.0f)
        current_cut_scene->unk_5c = 20.0f;

    if (current_cut_scene->unk_60 < -1.0f)
        current_cut_scene->unk_60 = -1.0f;
    else if (current_cut_scene->unk_60 > 1.0f)
        current_cut_scene->unk_60 = 1.0f;

    current_cut_scene->flags  |= 0x0800;
    current_cut_scene->unk_64  = 1;
    current_cut_scene->unk_68  = 1.5f;
}

// sub_808740
void cut_scene_player::start_fade() {
    cut_scene* fading = current_cut_scene;

    if ((fading->flags & cut_scene::no_fades) ||
        (fading->start_fade.fade_out_time <= 0.0f && fading->start_fade.fade_in_time <= 0.0f)) {

        play_current_segment();

        fade_state = 0;

        return;
    }

    fade_state     = 1;
    fade_hold_time = fading->start_fade.hold_time;

    state_flags &= ~(skip_fade_in | 0x0800);

    fade_in_time = fading->start_fade.fade_in_time;

    ui_frontend* igo = references::frontend.get().igo;

    retail::sub_698B50((u32*)igo, &fading->start_fade.color); // the loading screen's color

    // the loading screen is already up
    if (retail::sub_6884D0((u32*)igo)) {
        fade_timer = 0.0f;

        return;
    }

    fade_timer = fading->start_fade.fade_out_time;

    retail::sub_70A8B0((u32*)igo, true, fade_timer, true); // shows the loading screen
}

// sub_808DB0
void cut_scene_player::start_entities(mash::avl_tree<entity_class_entry>* entries) {
    for (auto it = entries->begin(); it.node; it.iterate())
        start_entity(it.node->element);
}

// sub_808900
void cut_scene_player::start_entity(entity_class_entry* entry) {
    using set_morph_method = void (__thiscall*)(morph_interface* self, mash::string morph_name);

    entry->entity = arch_base_vhandle();

    auto found = (entity*)retail::sub_7DF6E0((u32*)entry);

    if (found) {
        vector3d far_away(-999.0f);

        retail::sub_626CA0((u32*)found, (u32*)&far_away);

        found->flags |= 0x1000000;

        if (found->has_ifc(entity_ifc_physical))
            found->physical_ifc()->cancel_all_velocity(zerovec);

        if (found->is_an_actor()) {
            if (((actor*)found)->get_ai_core() && !retail::sub_601C50((u32*)found))
                retail::sub_4DF470((i32*)((actor*)found)->get_ai_core(), 0); // ai_core::push_ai_disable

            if (found->has_ifc(entity_ifc_physical)) {
                if (found->flags & entity_flag_collisions_active)
                    entry->flags |= entity_class_entry::collisions_active;
                else
                    entry->flags &= ~entity_class_entry::collisions_active;
            }

            // slot 13
            if (((bool (__thiscall*)(entity*))found->vtable[13])(found) && entry->variant.valid()) {
                if (found->has_ifc(entity_ifc_variant)) {
                    retail::sub_65F730((u32*)found->my_ifc_storage->get_ifc(entity_ifc_variant), (i32)entry->variant.source_hash_code);

                    i32* values = entry->variant_values;

                    if (values[0] <= -1)
                        retail::sub_610510((u32*)found->my_ifc_storage->get_ifc(entity_ifc_variant));
                    else
                        retail::sub_601030((u32*)found->my_ifc_storage->get_ifc(entity_ifc_variant),
                                           values[0],
                                           values[1],
                                           values[2],
                                           values[3],
                                           values[4],
                                           values[5],
                                           values[6],
                                           values[7]);
                }
            } else if (entry->morph_name.m_size && found->has_ifc(entity_ifc_morph)) {
                auto morph = (morph_interface*)found->my_ifc_storage->get_ifc(entity_ifc_morph);

                arch_base_vhandle handle = found->my_handle;

                ((set_morph_method)retail::sub_61FD20)(morph, entry->morph_name);

                lip_sync_entities.push_back(handle);
            }
        }

        if (entry->unk_78) {
            if (found->has_ifc(entity_ifc_ise)) {
                generic_interface* ise = found->my_ifc_storage->get_ifc(entity_ifc_ise);

                ((void (__thiscall*)(generic_interface*))((void**)ise->vtable)[20])(ise); // slot 20
            }

            if (found->has_ifc(entity_ifc_tentacle))
                retail::sub_61FC80((u32*)found->my_ifc_storage->get_ifc(entity_ifc_tentacle));

            if (found->is_an_actor() && ((actor*)found)->get_ai_core())
                retail::sub_4C57B0((i32)((actor*)found)->get_ai_core());
        }

        entry->entity = found->my_handle;

        return;
    }

    if (!entry->entity_class.valid())
        return;

    auto spawned = (entity*)retail::sub_8047E0((u32*)&references::g_world_ptr.read()->ent_mgr,
                                               (i32)entry->entity_class.source_hash_code,
                                               (u32*)entry,
                                               0x2000);
    if (!spawned)
        return;

    arch_base_vhandle handle = spawned->my_handle;

    if (((bool (__thiscall*)(entity*))spawned->vtable[41])(spawned)) { // slot 41
        retail::sub_6048A0((u32*)spawned);
        ((vehicle*)spawned)->enable_physics(false);

        if (entry->variant.valid()) {
            auto model = (u32*)retail::sub_58EF50((u32*)references::vehicle_model_depot.read(), (i32)entry->variant.source_hash_code);

            if (model)
                ((vehicle*)spawned)->set_model(string_hash(*model));
        }
    } else if (((bool (__thiscall*)(entity*))spawned->vtable[13])(spawned) && entry->variant.valid()) { // slot 13
        if (retail::sub_5FCB20((u32*)spawned)) {
            retail::sub_65F730((u32*)spawned->my_ifc_storage->get_ifc(entity_ifc_variant), (i32)entry->variant.source_hash_code);

            i32* values = entry->variant_values;

            if (values[0] <= -1)
                retail::sub_610510((u32*)spawned->my_ifc_storage->get_ifc(entity_ifc_variant));
            else
                retail::sub_601030((u32*)spawned->my_ifc_storage->get_ifc(entity_ifc_variant),
                                   values[0],
                                   values[1],
                                   values[2],
                                   values[3],
                                   values[4],
                                   values[5],
                                   values[6],
                                   values[7]);
        }
    } else if (entry->morph_name.m_size && retail::sub_5A7FC0((u32*)spawned)) {
        auto morph = (morph_interface*)spawned->my_ifc_storage->get_ifc(entity_ifc_morph);

        ((set_morph_method)retail::sub_61FD20)(morph, entry->morph_name);

        lip_sync_entities.push_back(handle);
    }

    vector3d far_away(-999.0f);

    retail::sub_626CA0((u32*)spawned, (u32*)&far_away);

    spawned->flags |= 0x1000000;

    if (spawned->has_ifc(entity_ifc_physical))
        spawned->physical_ifc()->cancel_all_velocity(zerovec);

    if (spawned->is_an_actor() && ((actor*)spawned)->get_ai_core() && !retail::sub_601C50((u32*)spawned))
        retail::sub_4DF470((i32*)((actor*)spawned)->get_ai_core(), 0); // ai_core::push_ai_disable

    if (!(entry->flags & entity_class_entry::keep_spawned))
        spawned_entities.push_back(handle);

    entry->entity = spawned->my_handle;
}

// sub_805EF0
void cut_scene_player::reset_interfaces(mash::avl_tree<entity_class_entry>* entries) {
    entity* hero = references::g_world_ptr.read()->get_hero_ptr();

    retail::sub_4C6340((i32)((actor*)hero)->get_ai_core());
    retail::sub_6566F0((i32)references::g_world_ptr.read()->get_hero_ptr()->my_ifc_storage->get_ifc(entity_ifc_ise));
    retail::sub_61F7E0((u32*)references::g_world_ptr.read()->get_hero_ptr()->my_ifc_storage->get_ifc(entity_ifc_tentacle));

    for (auto it = entries->begin(); it.node; it.iterate())
        reset_interfaces(it.node->element);
}

// sub_805890
void cut_scene_player::reset_interfaces(entity_class_entry* entry) {
    auto reset = (entity*)retail::sub_7DF6E0((u32*)entry);

    if (!reset || !reset->is_an_actor())
        return;

    if (((actor*)reset)->get_ai_core())
        retail::sub_4C6340((i32)((actor*)reset)->get_ai_core());

    if (reset->has_ifc(entity_ifc_ise))
        retail::sub_6566F0((i32)reset->my_ifc_storage->get_ifc(entity_ifc_ise));

    if (reset->has_ifc(entity_ifc_tentacle))
        retail::sub_61F7E0((u32*)reset->my_ifc_storage->get_ifc(entity_ifc_tentacle));
}

// sub_808FA0
void cut_scene_player::stop(cut_scene* only) {
    state_flags &= ~(0x20000 | segment_paused);

    if (current_cut_scene && (current_cut_scene->flags & 0x0800))
        current_cut_scene->flags &= ~0x0800;

    retail::sub_730F10((u32*)references::glass_house_manager.read(), saved_glass_house_level); // set_glass_house_level

    if (only && only != current_cut_scene)
        return;

    ui_frontend* igo = references::frontend.get().igo;

    if (current_cut_scene) {
        u32 flags = current_cut_scene->flags;

        if (flags & cut_scene::takes_igo) {
            if (!(flags & 0x0004) || !(flags & 0x4000) || (flags & cut_scene::no_fades))
                take_igo(true);
            else
                state_flags |= give_back_igo;
        }

        flags = current_cut_scene->flags;

        if (flags & 0x0004) {
            if ((flags & 0x4000) && !(flags & cut_scene::no_fades))
                state_flags |= 0x0800;
            else if (on_last_segment() && !(state_flags & toa_middle) && !(state_flags & toa_first))
                retail::sub_6E7000((u32*)igo, 0.8f, 0.0f); // letterbox
        }
    }

    if (state_flags & playing) {
        if (current_cut_scene && !(current_cut_scene->flags & cut_scene::hero_unaffected)) {
            app::inst()->get_game()->freeze_hero(false);

            entity* hero = references::g_world_ptr.read()->get_hero_ptr();

            if (hero->has_ifc(entity_ifc_standing))
                hero->standing_ifc()->enable(true);

            ((actor*)references::g_world_ptr.read()->get_hero_ptr())->get_ai_core()->idle_type = 0;

            player_interface* player = references::g_world_ptr.read()->get_hero_ptr()->player_ifc();

            ((void (__thiscall*)(player_interface*, i32))((void**)player->vtable)[187])(player, 0); // slot 187
        }

        start_end_fade();

        if (!fade_state ||
            (current_cut_scene->end_fade.fade_out_time <= 0.0f && current_cut_scene->end_fade.fade_in_time <= 0.0f))

            finish();
    }

    state_flags &= ~(playing | skipping);
}

// sub_8087F0
void cut_scene_player::finish() {
    if (!current_cut_scene)
        return;

    retail::sub_63CD10(); // empties the morph of every morph interface whose entity has a variant interface

    release_entities();

    entity* hero = references::g_world_ptr.read()->get_hero_ptr();

    hero->set_collisions_active((state_flags & hero_collisions_active) != 0, true);

    state_flags &= ~hero_collisions_active;

    if (*current_segment && current_segment != current_cut_scene->segments.end())
        release_current_segment();

    restore_entities();

    if (current_cut_scene->unk_30.m_size) {
        string_hash name;

        name.initialize(mash::ALLOCATED, current_cut_scene->unk_30.m_data);

        string_hash id = name;

        auto target = (entity*)retail::sub_7DF6E0((u32*)&id);

        // slot 28
        if (target && ((bool (__thiscall*)(entity*))target->vtable[28])(target)) {
            auto chase = (spiderman_camera*)references::g_world_ptr.read()->get_chase_cam_ptr();

            if (chase->is_a_spiderman_camera())
                ((void (__thiscall*)(spiderman_camera*, entity*))chase->vtable[158])(chase, target); // slot 158
        }
    }

    retail::sub_901010(nullptr); // stops the cut scene sound

    current_cut_scene = nullptr;
}

// sub_805EC0
bool cut_scene_player::on_last_segment() const {
    return current_cut_scene && current_segment + 1 == current_cut_scene->segments.end();
}

// sub_8091A0
bool cut_scene_player::advance_fade(f32 time_inc) {
    if (!fade_state)
        return true;

    if ((state_flags & playing) && !districts_loaded(0.0f) && fade_state != 1)
        return false;

    fade_timer = (f32)((f64)fade_timer - (f64)time_inc);

    if (fade_timer >= 0.0f)
        return false;

    ui_frontend* igo = references::frontend.get().igo;

    if (fade_state == 1) {
        if (state_flags & playing) {
            play_current_segment();

            if (current_cut_scene && (current_cut_scene->flags & 0x0004) && (current_cut_scene->flags & 0x2000))
                retail::sub_6E7000((u32*)igo, 0.0f, 60.0f); // letterbox
        } else {
            finish();

            if (state_flags & 0x0800) {
                state_flags &= ~0x0800;

                retail::sub_6E7000((u32*)igo, 0.0f, 0.0f);

                if (state_flags & give_back_igo) {
                    state_flags &= ~give_back_igo;

                    take_igo(true);
                }
            }
        }

        if (fade_hold_time > 0.0f) {
            fade_timer = fade_hold_time;
            fade_state = 2;

            return false;
        }
    } else if (fade_state != 2) {
        if (fade_state != 3)
            return false;

        u32 black = 0xFF000000;

        retail::sub_698B50((u32*)igo, &black); // the loading screen's color

        fade_state = 0;

        return true;
    }

    // the hold is over, or there was none
    if (state_flags & skip_fade_in) {
        u32 black = 0xFF000000;

        retail::sub_698B50((u32*)igo, &black);
        retail::sub_6884F0((u32*)igo); // lets go of the hero freezes the loading screen took

        fade_state = 0;

        return true;
    }

    retail::sub_70A8B0((u32*)igo, false, fade_in_time, true); // hides the loading screen

    fade_timer = fade_in_time;
    fade_state = 3;

    return (state_flags & playing) != 0;
}

// sub_8056F0
void cut_scene_player::start_end_fade() {
    cut_scene* fading = current_cut_scene;

    if ((fading->flags & cut_scene::no_fades) ||
        (fading->end_fade.fade_out_time <= 0.0f && fading->end_fade.fade_in_time <= 0.0f)) {

        fade_state = 0;

        return;
    }

    fade_state     = 1;
    fade_timer     = fading->end_fade.fade_out_time;
    fade_hold_time = fading->end_fade.hold_time;
    fade_in_time   = fading->end_fade.fade_in_time;

    if (!(state_flags & 0x20000) && (fading->flags & cut_scene::end_on_black))
        state_flags |= skip_fade_in;
    else
        state_flags &= ~skip_fade_in;

    ui_frontend* igo = references::frontend.get().igo;

    retail::sub_698B50((u32*)igo, &fading->end_fade.color); // the loading screen's color
    retail::sub_70A8B0((u32*)igo, true, fade_timer, true);  // shows the loading screen
}

// sub_8057A0
// inlined @ sub_808FA0
// inlined @ sub_8091A0
void cut_scene_player::take_igo(bool give_back) {
    ui_frontend* igo = references::frontend.get().igo;

    if (give_back) {
        igo->radar_widget->set_visible((state_flags & radar_was_visible) != 0);

        igo->zoom_map->unk_8f95 = false;
        igo->zoom_map->unk_8f96 = false;

        references::game.read()->disable_interface = false;

        return;
    }

    if (igo->radar_widget->is_visible())
        state_flags |= radar_was_visible;
    else
        state_flags &= ~radar_was_visible;

    if (state_flags & radar_was_visible)
        igo->radar_widget->set_visible(false);

    igo->zoom_map->unk_8f95 = true;
    igo->zoom_map->unk_8f96 = true;

    references::game.read()->disable_interface = true;

    player_interface* player = references::g_world_ptr.read()->get_hero_ptr()->player_ifc();

    ((void (__thiscall*)(player_interface*, u32))((void**)player->vtable)[47])(player, 0); // slot 47
}

// sub_8068A0
void cut_scene_player::restore_entities() {
    if (!current_cut_scene)
        return;

    for (auto it = current_cut_scene->entity_class_entries.begin(); it.node; it.iterate()) {
        entity_class_entry* entry = it.node->element;

        string_hash id = entry->name;

        auto restored = (entity*)retail::sub_7DF6E0((u32*)&id);

        if (!restored)
            continue;

        ai_core* ai = ((actor*)restored)->get_ai_core();

        if (ai && !retail::sub_601C50((u32*)restored))
            retail::sub_4DF4B0((i32*)ai, 0); // ai_core::pop_ai_disable

        if (entry->flags & entity_class_entry::place_at_end) {
            if (ai) {
                if (info_node* node = ai->get_info_node((e_info_node_type)23))
                    retail::sub_450480((u8*)node, true, false);
            }

            vector4d position(entry->end_position.x, entry->end_position.y, entry->end_position.z, 0.0f);
            vector4d rotation((f32)((f64)entry->end_rotation.x * 0.01745329238474369),
                              (f32)((f64)entry->end_rotation.y * 0.01745329238474369),
                              (f32)((f64)entry->end_rotation.z * 0.01745329238474369),
                              0.0f);

            po transform;

            retail::sub_606F80((u64*)&transform, (i32)&position, (f32*)&rotation, 1.0f);
            retail::sub_627180((i32)restored, (f32*)&transform);

            if (restored->is_an_actor() && ((actor*)restored)->get_ai_core()) {
                auto als = (als_inode*)((actor*)restored)->get_ai_core()->get_info_node(info_node_type_als);

                if (als) {
                    retail::sub_4029D0((u32**)als, 0x8F, (i32)&restored->my_abs_po->get_z_facing(), 0); // als_inode::set_desired_param

                    if (als->my_als)
                        ((void (__thiscall*)(void*))(*(void***)als->my_als)[22])(als->my_als); // slot 22
                }
            }
        }

        if (restored->has_ifc(entity_ifc_physical))
            restored->set_collisions_active((entry->flags & entity_class_entry::collisions_active) != 0, true);

        ((actor*)restored)->save_last_collision_free_state();
        ((actor*)restored)->invalidate_frame_delta();

        if (restored->has_ifc(entity_ifc_physical))
            restored->physical_ifc()->cancel_all_velocity(zerovec);

        ((void (__thiscall*)(entity*, terrain*, i32, i32))restored->vtable[89])(restored, references::g_world_ptr.read()->the_terrain, 0, 0); // slot 89
    }
}

// sub_807EC0
void cut_scene_player::release_entities() {
    using set_morph_method = void (__thiscall*)(morph_interface* self, mash::string morph_name);

    for (arch_base_vhandle* handle = lip_sync_entities.begin(); handle != lip_sync_entities.end(); handle++) {
        auto morphed = (entity*)handle->resolve();

        if (morphed && morphed->has_ifc(entity_ifc_morph)) {
            auto morph = (morph_interface*)morphed->my_ifc_storage->get_ifc(entity_ifc_morph);

            ((set_morph_method)retail::sub_61FD20)(morph, mash::string((const char*)nullptr));
        }
    }

    remove_entities(&spawned_entities);
}

// sub_807B60
void cut_scene_player::remove_entities(vc8::vector<arch_base_vhandle>* entities) {
    for (arch_base_vhandle* handle = entities->begin(); handle != entities->end(); handle++) {
        arch_base* removed = handle->resolve();

        if (removed)
            retail::sub_7FBF20((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)removed);
    }

    entities->clear();
}

// sub_805070
void cut_scene_player::update_focus() {
    has_focus = false;

    if (!(state_flags & playing) && !(state_flags & skipping) && !fade_state)
        return;

    if (!current_cut_scene)
        return;

    if (current_cut_scene->focus_from_camera) {
        camera* view_camera = references::game.read()->get_current_view_camera();

        if (!view_camera)
            return;

        bool unk;
        f32  distance_1;
        f32  distance_2;

        using get_focus_method = bool (__thiscall*)(camera* self, bool* unk, f32* distance_1, f32* distance_2);

        if (!((get_focus_method)view_camera->vtable[164])(view_camera, &unk, &distance_1, &distance_2)) // slot 164
            return;

        f32 scaled_2 = (f32)((f64)distance_2 / 100.0);
        f32 scaled_1 = (f32)((f64)distance_1 / 100.0);

        focus[0]  = (f32)(((f64)scaled_1 + (f64)scaled_2) * 0.5);
        focus[1]  = (f32)((f64)scaled_2 - (f64)focus[0]);
        focus[2]  = (f32)((f64)focus[1] * 2.0);
        has_focus = true;
    } else if (current_cut_scene->use_focus) {
        has_focus = true;
        focus[0]  = current_cut_scene->focus[0];
        focus[1]  = current_cut_scene->focus[1];
        focus[2]  = current_cut_scene->focus[2];
    }
}

// sub_807C90
void cut_scene_player::play_current_segment() {
    if (!current_cut_scene || !current_cut_scene->resource_context)
        return;

    amalga::push_resource_context_stack_object context(current_cut_scene->resource_context);

    state_flags &= ~hooked_up;

    world_dynamics_system* world = references::g_world_ptr.read();

    mash::vector_basic<string_hash> &regions = (*current_segment)->regions;

    for (string_hash* name = regions.begin(); name != regions.end(); name++) {
        auto forced = (region*)retail::sub_7A1990((u32*)world->the_terrain, (i32)name->source_hash_code); // terrain::find_region

        if (forced && !(forced->flags & 0x20000)) {
            forced_regions.push_back(forced);

            retail::sub_7A4BE0((u32*)world->the_terrain, (i32)forced->get_district_id().source_hash_code); // terrain::force_region
        }
    }

    state_flags |= segment_paused;

    world->kill_scene_anim(scene_anim_handle, false, true);

    nal::scene_anim* next_scene_anim = (*current_segment)->scene_anim;

    mash::string filename(next_scene_anim->name.get_text());

    world->load_scene_anim(filename);

    scene_anim_handle  = world->play_scene_anim(filename, 0, 0.0f);
    current_scene_anim = next_scene_anim;

    world->set_scene_anim_paused(scene_anim_handle, true);

    if ((*current_segment)->hour >= 0)
        retail::sub_97EC90((u32*)mission_manager::inst(), (*current_segment)->hour, 0); // sets the game clock

    retail::sub_77BAB0((u32*)world->the_terrain);

    loading_time = 0.0f;
}

// sub_806660
void cut_scene_player::release_current_segment() {
    world_dynamics_system* world = references::g_world_ptr.read();

    state_flags  &= ~hooked_up;
    loading_time  = 0.0f;

    cut_scene_segment** next = current_segment + 1;

    world->kill_scene_anim(scene_anim_handle, false, true);

    current_scene_anim = nullptr;

    bool last = next == current_cut_scene->segments.end();

    for (u32 i = 0; i < forced_regions.size(); i++) {
        region* forced = forced_regions[i];

        string_hash district = forced->get_district_id();

        // regions the next segment lists stay forced
        if (!last) {
            if (current_segment + 1 == current_cut_scene->segments.end())
                continue;

            mash::vector_basic<string_hash> &next_regions = (*(current_segment + 1))->regions;

            bool kept = false;

            for (u32 j = 0; j < next_regions.size; j++) {
                if (next_regions.array[j] == district) {
                    kept = true;

                    break;
                }
            }

            if (kept)
                continue;
        }

        retail::sub_7A4C60((i32)world->the_terrain, (i32)forced->get_district_id().source_hash_code); // terrain::unforce_region
    }
}

// sub_806590
bool cut_scene_player::districts_loaded(f32 time_inc) {
    world_dynamics_system* world = references::g_world_ptr.read();

    mash::vector_basic<string_hash> &regions = (*current_segment)->regions;

    for (string_hash* name = regions.begin(); name != regions.end(); name++) {
        auto loading = (region*)retail::sub_7A1990((u32*)world->the_terrain, (i32)name->source_hash_code); // terrain::find_region

        if (loading && !retail::sub_77EC90((i32)loading)) {
            loading_time = (f32)((f64)loading_time + (f64)time_inc);

            return false;
        }
    }

    if (!current_cut_scene)
        return false;

    return !current_cut_scene->wait_for_regions || retail::sub_77BF80() || time > 0.0f;
}

// sub_806770
bool cut_scene_player::start_current_segment() {
    if (!(state_flags & segment_paused))
        return false;

    state_flags &= ~segment_paused;

    references::g_world_ptr.read()->set_scene_anim_paused(scene_anim_handle, false);

    cut_scene_segment* segment = *current_segment;

    if (segment->flags & cut_scene_segment::silent) {
        if (retail::sub_8FF160())
            retail::sub_901010(nullptr); // stops the cut scene sound
    } else {
        string_hash sound = segment->get_sound();
        string_hash next_sound;

        if (retail::sub_8FF160()) {
            string_hash playing;

            retail::sub_8FDFB0((u32*)&playing);

            if (sound == playing)
                next_sound = string_hash(0);
            else {
                retail::sub_901010(nullptr);

                next_sound = sound;
            }
        } else
            next_sound = sound;

        string_hash none;

        none.initialize(mash::ALLOCATED);

        // starts the cut scene sound
        ((char (__cdecl*)(string_hash, bool))retail::sub_901F70)(next_sound, next_sound != none);
    }

    if ((current_cut_scene->flags & 0x0004) &&
        (!(current_cut_scene->flags & 0x2000) || (current_cut_scene->flags & cut_scene::no_fades)) &&
        current_cut_scene &&
        current_segment == current_cut_scene->segments.begin() &&
        !(state_flags & toa_middle) &&
        !(state_flags & toa_last)) {

        retail::sub_6E7000((u32*)references::frontend.get().igo, 1.5f, 60.0f); // letterbox
    }

    return true;
}

// sub_806450
void cut_scene_player::hookup_current_segment() {
    amalga::push_resource_context_stack_object context(current_cut_scene->resource_context);

    mash::vector<cut_scene_segment_attachment> &attachments = (*current_segment)->attachments;

    for (cut_scene_segment_attachment** attachment = attachments.begin(); attachment != attachments.end(); attachment++) {
        string_hash name;

        name.initialize(mash::ALLOCATED, (*attachment)->name.m_data);

        string_hash id = name;

        auto attached = (entity*)retail::sub_7DF6E0((u32*)&id);

        // slot 28
        if (attached && ((bool (__thiscall*)(entity*))attached->vtable[28])(attached))
            retail::sub_653DA0((u32*)attached, (i32*)references::g_world_ptr.read()->get_hero_ptr()); // entity_base::set_parent
    }
}

// sub_806380
void cut_scene_player::raise_animation_events(mash::avl_tree<entity_class_entry>* entries, f32 t, f32 last_t, bool skipping) {
    for (auto it = entries->begin(); it.node; it.iterate())
        raise_animation_events(it.node->element, t, last_t, skipping);
}

// sub_805FB0
void cut_scene_player::raise_animation_events(entity_class_entry* entry, f32 t, f32 last_t, bool skipping) {
    using is_black_suit_method = bool (__thiscall*)(player_interface* self);
    using change_suit_method   = void (__thiscall*)(player_interface* self, f32 unk, bool black);

    if (!entry->events.size)
        return;

    player_interface* player = references::g_world_ptr.read()->get_hero_ptr()->player_ifc();

    auto is_black_suit = (is_black_suit_method)((void**)player->vtable)[305];
    auto change_suit   = (change_suit_method)  ((void**)player->vtable)[572];

    for (entity_class_entry::cutscene_animation_event** it = entry->events.begin(); it != entry->events.end(); it++) {
        entity_class_entry::cutscene_animation_event* animation_event = *it;

        i32 event_time = (i32)((f64)(f32)((f64)animation_event->frame / 30.0) * 100000.0);

        if (event_time <= (i32)((f64)last_t * 100000.0) || event_time > (i32)((f64)t * 100000.0))
            continue;

        if (skipping && !animation_event->when_skipping)
            continue;

        u32 param_type = 0;
        u32 param      = 0;

        switch (animation_event->get_param_type()) {
            case 1:
                param      = (u32)animation_event->get_param_int();
                param_type = 4;
                break;

            case 2:
                param      = std::bit_cast<u32>(animation_event->get_param_float());
                param_type = 2;
                break;

            case 3:
                param      = animation_event->get_param_hash().source_hash_code;
                param_type = 1;
                break;
        }

        string_hash id = animation_event->event;

        static const string_hash suit_change_red   = string_hash(hash::djb2("suit_change_red"));
        static const string_hash suit_change_black = string_hash(hash::djb2("suit_change_black"));

        if (id == suit_change_red) {
            if (is_black_suit(player))
                change_suit(player, std::bit_cast<f32>(param), false);
        } else if (id == suit_change_black) {
            if (!is_black_suit(player))
                change_suit(player, std::bit_cast<f32>(param), true);
        } else {
            static const string_hash play_effect_blacksuit_only = string_hash(hash::djb2("event_play_effect_blacksuit_only"));
            static const string_hash play_effect_redsuit_only   = string_hash(hash::djb2("event_play_effect_redsuit_only"));
            static const string_hash play_effect                = string_hash(hash::djb2("PLAY_EFFECT"));

            if (id == play_effect_blacksuit_only) {
                if (!is_black_suit(player))
                    continue;

                id = play_effect;
            } else if (id == play_effect_redsuit_only) {
                if (is_black_suit(player))
                    continue;

                id = play_effect;
            }

            string_hash bone;

            bone.initialize(mash::ALLOCATED, nullptr, hash::djb2(""));

            anim_event raised(id, bone, param_type != 0, animation_event->frame);

            if (param_type) {
                raised.params->type  = param_type;
                raised.params->value = param;
            }

            event_manager::raise_event((event*)&raised, entry->entity);
        }
    }
}

// sub_806410
// inlined @ sub_8093C0
void cut_scene_player::raise_hero_animation_events(f32 t, f32 last_t, bool skipping) {
    current_cut_scene->hero_entry.entity = references::g_world_ptr.read()->get_hero_ptr()->my_handle;

    raise_animation_events(&current_cut_scene->hero_entry, t, last_t, skipping);
}

// sub_805CB0
f32 cut_scene_player::get_total_time() const {
    if (!current_cut_scene)
        return 0.0f;

    mash::vector<cut_scene_segment> &segments = current_cut_scene->segments;

    f32 total = 0.0f;

    for (cut_scene_segment** segment = segments.begin(); segment != segments.end(); segment++)
        total = (f32)((f64)total + (f64)(*segment)->get_duration());

    return total;
}

// sub_805510
f32 cut_scene_player::get_current_segment_start_time() const {
    mash::vector<cut_scene_segment> &segments = current_cut_scene->segments;

    f32 start = 0.0f;

    for (u32 i = 0; i < segments.size; i++) {
        if (segments[i] == *current_segment)
            break;

        start = (f32)((f64)start + (f64)segments[i]->get_duration());
    }

    return start;
}

// sub_805D20
f32 cut_scene_player::get_current_segment_end_time() const {
    if (!current_cut_scene)
        return 0.0f;

    mash::vector<cut_scene_segment> &segments = current_cut_scene->segments;

    cut_scene_segment** last = current_segment != segments.end() ? current_segment + 1 : segments.end();

    f32 end = 0.0f;

    for (cut_scene_segment** segment = segments.begin(); segment != last; segment++)
        end = (f32)((f64)end + (f64)(*segment)->get_duration());

    return end;
}

// sub_805DB0
f32 cut_scene_player::get_time() const {
    if (!current_cut_scene)
        return 0.0f;

    cut_scene_segment** segment = current_cut_scene->segments.begin();

    f32 t = references::g_world_ptr.read()->get_scene_anim_time(scene_anim_handle);

    for (; segment != current_segment; segment++)
        t = (f32)((f64)t + (f64)(*segment)->get_duration());

    return t;
}
