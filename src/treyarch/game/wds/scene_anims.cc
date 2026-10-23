#include <algorithm>

#include "retail.hh"
#include "treyarch/game/animation/scene_anim.hh"
#include "treyarch/game/audio/references.hh"
#include "treyarch/game/cutscene/cut_scene_player.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/shared/filespec.hh"

using namespace treyarch;

// sub_97A9C0
void world_dynamics_system::load_scene_anim(const mash::string &filename) {
    scene_anim* loaded = new scene_anim;

    loaded->load(filename);

    filespec spec(filename);

    spec.name.to_upper();

    scene_anim_map[spec.name] = loaded;
}

// sub_979AC0
scene_anim_handle_t world_dynamics_system::play_scene_anim(scene_anim* scene_anim_ptr, u16 flags, f32 offset) {
    if (!scene_anim_ptr)
        return (scene_anim_handle_t)-1;

    if (!scene_anim_ptr->scene_anim_inst) {
        if (player_reflexes* reflexes = references::g_world_ptr.read()->hero_ptr->player_ifc()->get_reflexes())
            reflexes->disable(false);

        if (player_spider_senses* senses = references::g_world_ptr.read()->hero_ptr->player_ifc()->get_spider_senses())
            senses->disable(true, false);

        ++last_snm_handle;

        references::saved_global_time_dilation.write(time_mgr.global_time_dilation);

        time_mgr.global_time_dilation = 1.0f;

        scene_anim_ptr->play(last_snm_handle, flags, offset);

        playing_scene_anims[last_snm_handle] = scene_anim_ptr;
    }

    return last_snm_handle;
}

// sub_979BB0
scene_anim_handle_t world_dynamics_system::play_scene_anim(const mash::string &filename, u16 flags, f32 offset) {
    filespec spec(filename);

    spec.name.to_upper();

    scene_anim_map_t::node* where  = scene_anim_map.find(spec.name);
    scene_anim_handle_t     handle = (scene_anim_handle_t)-1;

    if (where != scene_anim_map.end())
        handle = play_scene_anim(where->value.second, flags, offset);

    return handle;
}

// sub_977220
void world_dynamics_system::kill_scene_anim(scene_anim_handle_t handle, bool b_reverse, bool finish) {
    (void)b_reverse;

    sa_handle_obj_map_t::node* where = playing_scene_anims.find(handle);

    if (where == playing_scene_anims.end())
        return;

    scene_anim* anim = where->value.second;

    if (finish)
        anim->finish();

    anim->dead = true;

    // lets it die now rather than on the next update
    if (finish)
        update_scene_anims(0.0f);
}

// sub_975380
void world_dynamics_system::set_scene_anim_paused(scene_anim_handle_t handle, bool paused) {
    sa_handle_obj_map_t::node* where = playing_scene_anims.find(handle);

    if (where != playing_scene_anims.end())
        where->value.second->paused = paused;
}

// sub_9753B0
bool world_dynamics_system::is_scene_anim_finished(scene_anim_handle_t handle) {
    sa_handle_obj_map_t::node* where = playing_scene_anims.find(handle);

    if (where == playing_scene_anims.end())
        return true;

    scene_anim*               anim = where->value.second;
    nal::scene_anim_instance* inst = anim->scene_anim_inst;

    return inst && (anim->finished || inst->t >= 1.0f);
}

// sub_975410
f32 world_dynamics_system::get_scene_anim_time(scene_anim_handle_t handle) {
    sa_handle_obj_map_t::node* where = playing_scene_anims.find(handle);

    if (where == playing_scene_anims.end())
        return 0.0f;

    nal::scene_anim_instance* inst = where->value.second->scene_anim_inst;

    return inst->instances->anim->duration * inst->t;
}

// sub_975460
f32 world_dynamics_system::scene_anim_time_remaining(scene_anim_handle_t handle) {
    sa_handle_obj_map_t::node* where = playing_scene_anims.find(handle);

    if (where == playing_scene_anims.end())
        return 0.0f;

    nal::scene_anim_instance* inst   = where->value.second->scene_anim_inst;
    f32                       length = inst->instances->anim->duration;

    return length - inst->instances->anim->duration * inst->t;
}

// sub_977170
void world_dynamics_system::remove_scene_anim(const mash::string &filename) {
    filespec spec(filename);

    spec.name.to_upper();

    scene_anim_map_t::node* where = scene_anim_map.find(spec.name);

    if (where != scene_anim_map.end())
        scene_anim_map.erase(where);
}

// sub_976FB0
void world_dynamics_system::update_scene_anims(f32 t) {
    scene_anim_map_t::node* end = scene_anim_map.end();

    for (scene_anim_map_t::node* where = scene_anim_map.begin(); where != end;) {
        scene_anim* anim = where->value.second;

        // the anim can die below, so step past it first
        where = where->next();

        // a cut scene with sound keeps its scene anim on the sound's clock
        if (retail::sub_8FF160() &&
            references::cut_scene_player.read()->has_sound() &&
            !references::sound_disabled.read()) {

            f32 sound_time = references::cut_scene_player.read()->get_sound_time();

            nal::scene_anim_instance* inst = anim->scene_anim_inst;

            f32 elapsed = (f32)((f64)inst->instances->anim->duration * (f64)inst->t);
            f32 length  = inst->instances->anim->duration;
            f32 target  = std::max(0.0f, std::min(sound_time, length));

            anim->frame_advance((f32)((f64)target - (f64)elapsed));
        } else
            anim->frame_advance(t);

        if (anim->dead)
            scene_anim_dies(anim->the_handle);
    }
}

// inlined @ sub_976FB0
void world_dynamics_system::scene_anim_dies(scene_anim_handle_t handle) {
    sa_handle_obj_map_t::node* where = playing_scene_anims.find(handle);

    if (where == playing_scene_anims.end())
        return;

    scene_anim* dying = where->value.second;

    playing_scene_anims.erase(where);

    delete dying;

    time_mgr.global_time_dilation = references::saved_global_time_dilation.read();
}
