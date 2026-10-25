#include "retail.hh"
#include "treyarch/game/als/meta_anim_instance.hh"
#include "treyarch/game/als/meta_anim_swing.hh"
#include "treyarch/game/als/meta_linear_blend.hh"
#include "treyarch/game/als/state_machine.hh"
#include "treyarch/nal/anim.hh"

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

    instances    = (nal::anim_instance**)memory::heap::allocate(meta_anim->key_anims.size * sizeof(nal::anim_instance*));
    first_sample = true;

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
    using get_pose_t = void (__thiscall*)(als_meta_linear_blend_instance*, f32, f32, nal::pose*, nal::transport_fk*);

    ((get_pose_t)retail::sub_5F4BD0)(this, t, t_prev, pose, fk);
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
    first_sample = false;
}

// sub_5EFB50
void als::als_meta_linear_blend_instance::blend_two_anims(f32                 t,
                                                          f32                 t_prev,
                                                          nal::pose*          pose,
                                                          f32                 weight,
                                                          nal::anim_instance* first,
                                                          nal::anim_instance* second,
                                                          nal::transport_fk*  fk) {

    using blend_two_anims_t = void (__thiscall*)(als_meta_linear_blend_instance*,
                                                 f32,
                                                 f32,
                                                 nal::pose*,
                                                 f32,
                                                 nal::anim_instance*,
                                                 nal::anim_instance*,
                                                 nal::transport_fk*);

    ((blend_two_anims_t)retail::sub_5EFB50)(this, t, t_prev, pose, weight, first, second, fk);
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
