#include <algorithm>

#include "retail.hh"
#include "treyarch/game/animation/scene_anim.hh"
#include "treyarch/game/audio/references.hh"
#include "treyarch/game/cutscene/cut_scene_player.hh"
#include "treyarch/game/wds/references.hh"
#include "treyarch/game/wds/world_dynamics_system.hh"
#include "treyarch/shared/filespec.hh"

using namespace treyarch;

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
            retail::sub_8056E0((u32*)references::cut_scene_player.read()) &&
            !references::sound_disabled.read()) {

            f32 sound_time = (f32)retail::sub_805680((i32)references::cut_scene_player.read());

            nal::scene_anim_instance* inst = anim->scene_anim_inst;

            f32 elapsed = (f32)((f64)inst->instances->anim->duration * (f64)inst->t);
            f32 length  = inst->instances->anim->duration;
            f32 target  = std::max(0.0f, std::min(sound_time, length));

            retail::sub_5E00B0((u32*)anim, (f32)((f64)target - (f64)elapsed)); // scene_anim::frame_advance
        } else
            retail::sub_5E00B0((u32*)anim, t); // scene_anim::frame_advance

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
