#include <algorithm>

#include "retail.hh"
#include "treyarch/game/cutscene/cut_scene.hh"
#include "treyarch/game/cutscene/cut_scene_player.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/references.hh"

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

    retail::sub_806380((u32*)&current_cut_scene->entity_class_entries, time, last_time, skip_segment || skip_all);
    retail::sub_806410(this, time, last_time, skip_segment || skip_all);

    if (references::skip_next_segment_step.read()) {
        references::skip_next_segment_step.write(false);

        return;
    }

    if (!finished && !skip_segment)
        return;

    if (current_segment + 1 != current_cut_scene->segments.end() && !skip_all) {
        retail::sub_806660((u32*)this); // releases the segment

        current_segment++;

        retail::sub_807C90((u32*)this); // play_current_segment
    } else if (!--loops_left || skip_all)
        retail::sub_808FA0((i32*)this, 0); // stop
    else {
        retail::sub_806660((u32*)this);

        current_segment = current_cut_scene->segments.begin();

        retail::sub_807C90((u32*)this);
    }
}

// sub_8093C0
bool cut_scene_player::frame_advance_lite(f32 time_inc) {
    time_inc = std::min(time_inc, 0.1f);

    retail::sub_805070((u32*)this); // update_focus

    if (state_flags & playing) {
        player_interface* player = references::g_world_ptr.read()->get_hero_ptr()->player_ifc();

        last_time = time;
        time      = get_time();

        ((void (__thiscall*)(player_interface*, f32))((void**)player->vtable)[540])(player, time - last_time); // slot 540
    }

    if (!retail::sub_8091A0((i32)this, time_inc)) { // the fade step
        if (state_flags & playing) {
            retail::sub_806380((u32*)&current_cut_scene->entity_class_entries, time, last_time, false);

            current_cut_scene->hero_entry.entity = references::g_world_ptr.read()->get_hero_ptr()->my_handle;

            retail::sub_805FB0((u32*)&current_cut_scene->hero_entry, time, last_time, false);
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

    if (!retail::sub_806590((f32*)this, time_inc)) // districts_loaded
        return false;

    retail::sub_806770((u32*)this); // hookup_current_segment

    state_flags  |= hooked_up;
    segment_time  = 0.0f;

    retail::sub_806450((u32**)this); // start_lip_syncing

    return true;
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
