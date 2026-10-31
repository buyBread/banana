#include "retail.hh"
#include "game/animation/animation_controller.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"

using namespace treyarch;

// sub_699C40
void igo_3d_widget::fade_in(f32 duration, f32 from_alpha, f32 to_alpha, f32 delay) {
    fade_in_duration   = duration;
    fade_in_remaining  = (f32)((f64)duration + delay);
    fade_out_remaining = 0.0f;
    fading_in          = true;
    fading_out         = false;
    fade_in_alpha_span = (f32)((f64)to_alpha - from_alpha);
    fade_in_alpha_base = from_alpha;
    faded_in           = true;

    retail::sub_68A920(this, 0.0f); // fade step, applies the start alpha
}

// sub_699CD0
void igo_3d_widget::fade_out(f32 duration, f32 from_alpha, f32 to_alpha, f32 delay) {
    fade_out_duration   = duration;
    fade_out_remaining  = (f32)((f64)duration + delay);
    fade_in_remaining   = 0.0f;
    fading_out          = true;
    fading_in           = false;
    fade_out_alpha_span = (f32)((f64)from_alpha - to_alpha);
    fade_out_alpha_base = to_alpha;
    faded_in            = true;

    retail::sub_68A920(this, 0.0f); // fade step, applies the start alpha
}

// sub_68A860
void igo_3d_widget::kill_layer_0_anims() {
    arch_base* ent = entity_vh.resolve();

    if (!ent || !ent->is_an_actor())
        return;

    if (animation_controller* anim_ctrl = ((actor*)ent)->anim_ctrl)
        anim_ctrl->kill_layer_anims(0);
}

// sub_68A890
bool igo_3d_widget::is_anim_playing() {
    arch_base* ent = entity_vh.resolve();

    if (!ent || !ent->is_an_actor())
        return false;

    animation_controller* anim_ctrl = ((actor*)ent)->anim_ctrl;

    if (!anim_ctrl || !anim_ctrl->has_anim_instance(0))
        return false;

    f32 time = anim_ctrl->get_anim_time(0);

    return anim_ctrl->get_anim_duration(0) > time;
}

// sub_6899A0
i32 igo_3d_widget::get_mesh_index(const string_hash &nodename) {
    arch_base* ent = entity_vh.resolve();

    if (!ent || !ent->is_a_conglomerate())
        return invalid_conglom_member;

    return ((conglomerate*)ent)->get_member_index(nodename);
}

// sub_6899E0
i32 igo_3d_widget::find_child(mash::string* name) {
    name->finalize(mash::ALLOCATED);

    return -2;
}
