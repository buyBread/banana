#include <algorithm>
#include <bit>

#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/app/app.hh"
#include "treyarch/game/cutscene/cut_scene.hh"
#include "treyarch/game/cutscene/cut_scene_player.hh"
#include "treyarch/game/event/anim_event.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/frontend/frontend_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/wds/camera/camera.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/region.hh"
#include "treyarch/game/wds/terrain.hh"
#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/mash/avl_tree.hh"

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
        retail::sub_808FA0((i32*)this, 0); // stop
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

    if (!retail::sub_8091A0((i32)this, time_inc)) { // the fade step
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
        retail::sub_808FA0((i32*)this, 0); // stop

    if (!districts_loaded(time_inc))
        return false;

    start_current_segment();

    state_flags  |= hooked_up;
    segment_time  = 0.0f;

    hookup_current_segment();

    return true;
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
                if (next_regions.data[j] == district) {
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
        !(state_flags & 0x2000) &&
        !(state_flags & 0x8000)) {

        using igo_method = void (__thiscall*)(ui_frontend* self, f32 unk_1, f32 unk_2);

        ((igo_method)retail::sub_6E7000)(references::frontend.get().igo, 1.5f, 60.0f);
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
