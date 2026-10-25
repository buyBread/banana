#include <cmath>

#include "retail.hh"
#include "treyarch/game/als/animation_logic_system.hh"
#include "treyarch/game/als/meta_anim_instance.hh"
#include "treyarch/game/als/meta_anim_swing.hh"
#include "treyarch/game/als/meta_linear_blend.hh"
#include "treyarch/game/als/state_machine.hh"
#include "treyarch/nal/anim.hh"
#include "treyarch/nal/blend.hh"
#include "treyarch/shared/math/usefulmath.hh"

using namespace treyarch;

// sub_5ADE30
void als::nal_als_instance_bridge::get_pose(f32 t, f32 t_prev, nal::pose* pose, nal::transport_fk* fk) {
    if (nal_inst)
        nal_inst->get_pose(t, t_prev, pose, fk);
    else if (als_inst)
        als_inst->get_pose(t, t_prev, pose, fk);
}

// sub_71F870
bool als::als_anim_instance::is_looping() {
    return false;
}

// sub_A944D0
nal::anim* als::als_anim_instance::get_anim() {
    return nullptr;
}

// sub_5B4A70
void als::als_anim_instance::on_blend_out() {}

// sub_5BEEF0
als::als_meta_linear_blend_instance::als_meta_linear_blend_instance(als_meta_linear_blend*  the_meta_anim,
                                                                    nal::transport_list*    the_skel,
                                                                    u32                     flags,
                                                                    nal::skeleton_map*      skel_map,
                                                                    animation_logic_system* als,
                                                                    state_machine*          layer) : the_als(als),
                                                                                                     the_layer(layer),
                                                                                                     meta_anim(the_meta_anim) {

    instances     = (nal::anim_instance**)memory::heap::allocate(meta_anim->key_anims.size * sizeof(nal::anim_instance*));
    follows_param = true;

    for (u32 index = 0; index < meta_anim->key_anims.size; ++index) {
        if (nal::anim* key_anim = meta_anim->key_anims[index]->the_anim_ptr) {
            instances[index]                = key_anim->create_instance(flags, skel_map, the_skel);
            instances[index]->force_looping = layer->is_looping();
        } else
            instances[index] = nullptr;
    }
}

// sub_5B16A0
als::als_meta_linear_blend_instance::~als_meta_linear_blend_instance() {
    for (u32 index = 0; index < meta_anim->key_anims.size; ++index) {
        if (instances[index])
            delete instances[index];

        instances[index] = nullptr;
    }

    if (instances)
        memory::heap::free(instances);
}

// sub_5B1570
void* als::als_meta_linear_blend_instance::operator new(std::size_t) noexcept {
    return references::linear_blend_instance_pool.get().allocate();
}

// inlined @ sub_5BEFD0
void als::als_meta_linear_blend_instance::operator delete(void* allocation) noexcept {
    references::linear_blend_instance_pool.get().release(allocation);
}

// sub_5B1750
f32 als::als_meta_linear_blend_instance::get_duration() {
    return meta_anim->get_anim_proxy()->duration;
}

// sub_5F4BD0
void als::als_meta_linear_blend_instance::get_pose(f32 t, f32 t_prev, nal::pose* pose, nal::transport_fk* fk) {
    param_id param   = get_blend_amount_param(the_layer->get_optional_pb_int(meta_linear_param_index_id, 0, nullptr));
    f32      desired = the_layer->get_float_param(the_als, param);

    if (!follows_param || !the_layer->has_ext_param_been_set(param))
        desired = meta_anim->current_blend;

    clamp_value(&desired, 0.0f, 1.0f);

    f32 blend_speed = the_layer->get_optional_pb_float(meta_linear_param_blend_speed_id, 1.0f, nullptr);
    clamp_value(&blend_speed, 0.0f, 1.0f);

    meta_anim->current_blend -= (meta_anim->current_blend - desired) * blend_speed;

    // between which two key anims, and how far
    f32 position = (f32)(meta_anim->key_anims.size - 1) * meta_anim->current_blend;
    f32 lower    = std::floor(position);
    f32 upper    = std::ceil(position);

    f32 blend_amount = position - lower;
    clamp_value(&blend_amount, 0.0f, 1.0f);

    nal::pose blended(pose->pose_layout, nullptr);
    blend_two_anims(t, t_prev, &blended, blend_amount, instances[(i32)lower], instances[(i32)upper], fk);

    pose->copy(blended);
}

// sub_5B1760
bool als::als_meta_linear_blend_instance::is_looping() {
    return instances[0]->force_looping || (instances[0]->anim->flags & nal::anim_flag_looping);
}

// sub_5B1780
nal::anim* als::als_meta_linear_blend_instance::get_anim() {
    return instances[0]->anim;
}

// sub_5B1740
void als::als_meta_linear_blend_instance::on_blend_out() {
    follows_param = false;
}

// sub_5EFB50
void als::als_meta_linear_blend_instance::blend_two_anims(f32                 t,
                                                          f32                 t_prev,
                                                          nal::pose*          pose,
                                                          f32                 weight,
                                                          nal::anim_instance* first,
                                                          nal::anim_instance* second,
                                                          nal::transport_fk*  fk) {

    // each instance is primed just before t_prev, so its trajectory delta starts there
    if (first == second) {
        first->get_pose(t_prev, t_prev - 0.0001f, pose, fk);
        first->get_pose(t,      t_prev,           pose, fk);

        return;
    }

    nal::pose first_pose (pose->pose_layout, nullptr);
    nal::pose second_pose(pose->pose_layout, nullptr);

    f32 primed = t_prev - 0.0001f;

    first->get_pose (t_prev, primed, &first_pose,  fk);
    first->get_pose (t,      t_prev, &first_pose,  fk);
    second->get_pose(t_prev, primed, &second_pose, fk);
    second->get_pose(t,      t_prev, &second_pose, fk);

    nal::blend(pose, weight, &first_pose, &second_pose);

    // the trajectory goes by the param itself, not the eased blend
    param_id param      = get_blend_amount_param(the_layer->get_optional_pb_int(meta_linear_param_index_id, 0, nullptr));
    f32      trajectory = the_layer->get_float_param(the_als, param);

    if (the_layer->has_ext_param_been_set(param))
        clamp_value(&trajectory, 0.0f, 1.0f);
    else
        trajectory = 0.5f;

    trajectory *= (f32)(meta_anim->key_anims.size - 1);
    trajectory -= std::floor(trajectory);

    clamp_value(&trajectory, 0.0f, 1.0f);

    nal::blend_trajectory(pose, trajectory, &first_pose, &second_pose);
}

// sub_5B1650
// inlined @ sub_5F4BD0
als::param_id als::als_meta_linear_blend_instance::get_blend_amount_param(i32 index) {
    switch (index) {
        case 1:
            return 107;

        case 2:
            return 108;

        case 3:
            return 109;

        case 4:
            return 110;

        default:
            return 106;
    }
}

// sub_5BF050
als::als_meta_anim_swing_instance::als_meta_anim_swing_instance(als_meta_anim_swing*    the_meta_anim,
                                                                nal::transport_list*    the_skel,
                                                                u32                     flags,
                                                                nal::skeleton_map*      skel_map,
                                                                animation_logic_system* als,
                                                                state_machine*          layer) : keys{},
                                                                                                 the_als(als),
                                                                                                 the_layer(layer),
                                                                                                 meta_anim(the_meta_anim),
                                                                                                 unk_50(1.0f),
                                                                                                 unk_54(1.0f),
                                                                                                 unk_58(0.0f) {

    for (u32 index = 0; index < 8; ++index) {
        if (nal::anim* key_anim = meta_anim->keys[index].the_anim_ptr) {
            nal::anim_instance* nal_inst = key_anim->create_instance(flags, skel_map, the_skel);
            nal_inst->force_looping = layer->is_looping();

            keys[index].nal_inst = nal_inst;
        } else
            keys[index].nal_inst = nullptr;

        keys[index].als_inst = nullptr;
    }
}

// sub_5B1870
als::als_meta_anim_swing_instance::~als_meta_anim_swing_instance() {
    for (nal_als_instance_bridge &key : keys) {
        if (key.nal_inst || key.als_inst) {
            if (key.nal_inst)
                delete key.nal_inst;
            else if (key.als_inst)
                delete key.als_inst;

            key.nal_inst = nullptr;
            key.als_inst = nullptr;
        }
    }
}

// sub_5B1790
void* als::als_meta_anim_swing_instance::operator new(std::size_t) noexcept {
    return references::swing_instance_pool.get().allocate();
}

// inlined @ sub_5BF140
void als::als_meta_anim_swing_instance::operator delete(void* allocation) noexcept {
    references::swing_instance_pool.get().release(allocation);
}

// sub_720040
f32 als::als_meta_anim_swing_instance::get_duration() {
    return 1.0f;
}

// sub_5EFE70
void als::als_meta_anim_swing_instance::get_pose(f32 t, f32 t_prev, nal::pose* pose, nal::transport_fk* fk) {
    using get_pose_t = void (__thiscall*)(als_meta_anim_swing_instance*, f32, f32, nal::pose*, nal::transport_fk*);

    ((get_pose_t)retail::sub_5EFE70)(this, t, t_prev, pose, fk);
}
