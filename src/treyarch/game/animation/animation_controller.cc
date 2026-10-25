#include <cmath>
#include <cstring>
#include <limits>

#include "retail.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/animation/animation_controller_jq.hh"
#include "treyarch/game/animation/animation_pose_jq.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/nal/anim.hh"
#include "treyarch/nal/anim_instance.hh"
#include "treyarch/nal/transport_fk.hh"
#include "treyarch/nal/transport_list.hh"

using namespace treyarch;

// sub_5D2A20
void animation_controller::get_matrix_data_from_pose(bool unk) {
    retail::sub_5D2A20((i64**)this, unk);
}

// sub_5D4290
bool animation_controller::add_pose_jq_batch(scratch_arena* arena, bool immediate) {
    animation_controller_state* the_state = state;

    // nothing to pose isn't a failure
    if (!the_state || the_state->unk_128 || the_state->is_suspended)
        return true;

    nal::nal_jq_batch_input_t &input = the_state->pose_input;
    std::memset(&input, 0, sizeof(input));

    input.pose_count     = the_state->job.num_pose_records;
    input.combined_pose  = &the_state->combined_pose;
    input.alphas[0]      = 0.0f;
    input.skip_matrices  = the_state->job.skip_matrices;
    input.transport_list = the_state->skeleton;
    input.trajectory     = &the_state->trajectory;

    // slot 13
    ((void (__thiscall*)(entity*))the_state->unk_0e4->vtable[13])(the_state->unk_0e4);

    auto fk = (nal::transport_fk*)the_state->skeleton->get_transport(0);

    input.result_matrix_count = fk->bone_count;

    u32 needed = (arena->alignment + (input.result_matrix_count << 6) - 1) & ~(arena->alignment - 1);

    if (arena->cursor + needed >= arena->base + arena->size)
        return false;

    arena->cursor        += needed;
    input.result_matrices = (math::mat43*)(arena->cursor - needed);

    for (u32 index = 0; index < the_state->job.num_pose_records; ++index) {
        animation_controller_state::advance_job::pose_record &record = the_state->job.pose_records[index];
        animation_controller_state::layer_anim*               anim   = record.anim;

        input.t_prev[index] = record.prev_time;
        input.t[index]      = record.time;
        input.poses[index]  = &anim->pose;

        if (anim->nal_inst) {
            input.instances[index] = anim->nal_inst;
        } else {
            input.instances[index] = nullptr;

            anim->als_inst->get_pose(record.time, record.prev_time, &anim->pose, fk);
        }

        if (index)
            input.alphas[index] = anim->alpha;
    }

    if (immediate) {
        animation_pose_jq::start(false);
        animation_pose_jq::static_batch.get().payload[0] = &input;
        animation_pose_jq::add_batch(1);
        animation_pose_jq::finish(false);

        get_matrix_data_from_pose(false);
    } else {
        animation_pose_jq::static_batch.get().payload[0] = &input;
        animation_pose_jq::add_batch(1);
    }

    return true;
}

// sub_5D3950
void animation_controller::advance(animation_controller* controller) {
    if (!controller)
        return;

    animation_controller_state* the_state = controller->state;

    f32  delta_t = the_state->job.delta_t;
    bool jumped  = false;

    engine_lock_scope lock(&the_state->lock);

    if (the_state->unk_2f4 > 0.0f) {
        f32 base_time = the_state->layer_anims[0].size() ? the_state->layer_anims[0].back()->normalized_time : 0.0f;

        if (the_state->unk_2f8 / controller->get_anim_duration(0) <= base_time + 0.000001f) {
            if (delta_t > the_state->unk_2f4) {
                delta_t            -= the_state->unk_2f4;
                the_state->unk_2f4  = 0.0f;
            } else {
                the_state->unk_2f4 -= delta_t;
                delta_t             = 0.0f;
            }
        } else {
            controller->set_anim_normalized_time(0, the_state->unk_2f8 / controller->get_anim_duration(0));

            jumped  = true;
            delta_t = 0.0f;
        }
    }

    for (u32 layer = 0; layer < 15; ++layer) {
        animation_records &anims = the_state->layer_anims[layer];

        for (u32 index = 0; index < anims.size(); ++index) {
            animation_controller_state::layer_anim* anim = anims[index];

            bool older = anims.size() > index + 1;
            f32  prev  = anim->normalized_time;
            f32  time;

            if (!older && the_state->master && the_state->sync_to_master && layer == the_state->synced_layer) {
                if (the_state->should_follow_master()) {
                    time = anim->get_inverse_duration() * the_state->master->get_anim_time(layer);

                    animation_records &master_anims = the_state->master->state->layer_anims[layer];

                    f32 speed = master_anims.size() ? master_anims.back()->speed : 1.0f;

                    if (anims.size()) {
                        anims.back()->speed = speed;

                        if (speed < 0.0f)
                            anims.back()->speed = 0.0f;
                    }

                    f32 secondary_speed = master_anims.size() ? master_anims.back()->secondary_speed : 1.0f;

                    if (anims.size()) {
                        anims.back()->secondary_speed = secondary_speed;

                        if (secondary_speed < 0.0f)
                            anims.back()->secondary_speed = 0.0f;
                    }
                } else {
                    time = anim->get_inverse_duration() * (anim->speed * delta_t * anim->secondary_speed) + prev;
                }
            } else {
                time = anim->speed * delta_t * anim->secondary_speed * (1.0f / anim->get_duration()) + prev;
            }

            // a jump leaves the base layer where it put it
            if (!jumped || layer || time != prev)
                anim->set_normalized_time(time);

            // under a newer anim that's finished blending in
            animation_controller_state::layer_anim* newer = older ? anims[index + 1] : nullptr;

            if (newer && std::fabs(newer->alpha_start - newer->alpha_end) > 0.0001f && newer->normalized_time > newer->blend_end) {
                delete anim;
                anims.erase(index);

                --index;
            } else {
                animation_controller_state::advance_job::pose_record &record = the_state->job.pose_records[the_state->job.num_pose_records];

                record.anim      = anim;
                record.prev_time = prev;
                record.time      = time;
                record.newest    = index == anims.size() - 1;

                ++the_state->job.num_pose_records;
            }
        }
    }
}

// sub_5DFE20
void animation_controller::play(const als::nal_als_anim_bridge     &anim,
                                      f32                           start_time,
                                      f32                           blend_time,
                                      u32                           flags,
                                      bool,
                                      u32                           layer,
                                      als::animation_logic_system*  the_als,
                                      als::state_machine*           the_sm,
                                      bool                          looping) {

    als::nal_als_instance_bridge instance;

    if (anim.nal_anim) {
        nal::anim_instance* nal_inst = anim.nal_anim->create_instance(flags, state->skeleton_map, nullptr);
        nal_inst->force_looping = looping;

        instance.nal_inst = nal_inst;
        instance.als_inst = nullptr;
    } else
        instance = anim.als_anim->create_anim_inst(state->skeleton, flags, state->skeleton_map, (u32)anim.nal_anim, anim.als_anim, the_als, the_sm);

    animation_records &anims = state->layer_anims[layer];

    auto added = new animation_controller_state::layer_anim(state, instance, state->combined_pose.pose_layout);

    added->normalized_time = start_time / added->get_duration();

    // a looping anim starting before its start wraps round
    if (added->normalized_time < 0.0f) {
        bool loops = false;

        if (instance.nal_inst)
            loops = instance.nal_inst->force_looping || (instance.nal_inst->anim->flags & nal::anim_flag_looping);
        else if (instance.als_inst)
            loops = instance.als_inst->is_looping();

        if (loops)
            added->normalized_time += 1.0f;
    }

    // the newest fades out as the new one fades in
    if (anims.size()) {
        animation_controller_state::layer_anim* newest = anims.back();

        f32 fade_out = newest->get_inverse_duration() * blend_time;

        newest->blend_start = newest->normalized_time;
        newest->blend_end   = newest->normalized_time + fade_out;
        newest->alpha_start = 1.0f;
        newest->alpha_end   = 0.0f;

        if (newest->als_inst)
            newest->als_inst->on_blend_out();

        f32 fade_in = added->get_inverse_duration() * blend_time;

        added->blend_start = added->normalized_time;
        added->blend_end   = added->normalized_time + fade_in;
        added->alpha_start = 0.0f;
        added->alpha_end   = 1.0f;
    }

    if (anims.size() == 2) {
        delete anims[0];
        anims.erase(0);
    }

    anims.push_back(added);
}

// sub_5BA3B0
void animation_controller::kill_layer_anims(u32 layer) {
    animation_records &anims = state->layer_anims[layer];

    for (u32 index = 0; index < anims.size(); ++index)
        delete anims[index];

    anims.clear();
}

// sub_5D44E0
void animation_controller::reset_speeds() {
    for (animation_records &anims : state->layer_anims) {
        if (anims.size()) {
            anims.back()->speed = 1.0f;

            if (anims.size())
                anims.back()->secondary_speed = 1.0f;
        }
    }
}

// sub_5BA290
f32 animation_controller::get_anim_duration(u32 layer) {
    if (!state || !state->layer_anims[layer].size())
        return 1.0f;

    return state->layer_anims[layer].back()->get_duration();
}

// sub_5BA510
f32 animation_controller::get_anim_time(u32 layer) {
    if (!state->layer_anims[layer].size())
        return 0.0f;

    animation_controller_state::layer_anim* anim = state->layer_anims[layer].back();

    return anim->get_duration() * anim->normalized_time;
}

// sub_5BA1A0
void animation_controller::set_anim_normalized_time(u32 layer, f32 time) {
    if (state && !state->master && state->layer_anims[layer].size())
        state->layer_anims[layer].back()->set_normalized_time(time);
}

// sub_5BA030
bool animation_controller_state::should_follow_master() const {
    return ((bool (__thiscall*)(const animation_controller_state*))retail::sub_5BA030)(this);
}

// sub_5D2950
animation_controller_state::layer_anim::layer_anim(animation_controller_state*         the_owner,
                                                   const als::nal_als_instance_bridge &instance,
                                                   nal::pose_layout*                   layout) : nal_inst(nullptr),
                                                                                                 als_inst(nullptr),
                                                                                                 normalized_time(0.0f),
                                                                                                 prev_normalized_time(0.0f),
                                                                                                 pose(layout, nullptr),
                                                                                                 alpha(1.0f),
                                                                                                 blend_start(0.0f),
                                                                                                 blend_end(1.0f),
                                                                                                 alpha_start(1.0f),
                                                                                                 alpha_end(1.0f),
                                                                                                 speed(1.0f),
                                                                                                 secondary_speed(1.0f),
                                                                                                 unk_06c(true),
                                                                                                 owner(the_owner) {

    nal_inst = instance.nal_inst;
    als_inst = instance.als_inst;
}

// sub_5B9EF0
animation_controller_state::layer_anim::~layer_anim() {
    if (nal_inst)
        delete nal_inst;
    else if (als_inst)
        delete als_inst;
}

// sub_5DFBD0
void* animation_controller_state::layer_anim::operator new(std::size_t) noexcept {
    engine_lock_scope lock(&references::pool_lock.get());

    layer_anim* result = references::layer_anim_free_list.read();

    if (result || (result = (layer_anim*)retail::sub_5CD170()) != nullptr) {
        references::layer_anim_free_list.write(*(layer_anim**)result);
    } else {
        retail::sub_9CC940((char*)"Out of pool memory limit");

        result = nullptr;
    }

    return result;
}

// sub_5B9FF0
// inlined @ sub_5D3950
void animation_controller_state::layer_anim::operator delete(void* allocation) noexcept {
    engine_lock_scope lock(&references::pool_lock.get());

    *(layer_anim**)allocation = references::layer_anim_free_list.read();

    references::layer_anim_free_list.write((layer_anim*)allocation);
}

// sub_5B77E0
f32 animation_controller_state::layer_anim::get_inverse_duration() const {
    if (nal_inst)
        return 1.0f / nal_inst->anim->duration;

    if (als_inst)
        return 1.0f / als_inst->get_duration();

    // retail divides by zero here
    return std::numeric_limits<f32>::infinity();
}

// inlined @ sub_5BA1A0
// inlined @ sub_5D3950
void animation_controller_state::layer_anim::set_normalized_time(f32 time) {
    prev_normalized_time = normalized_time;
    normalized_time      = time;

    if (time <= blend_start || blend_end <= time) {
        if (time > blend_end)
            alpha = alpha_end;
    } else {
        f32 t = (time - blend_start) / (blend_end - blend_start);

        alpha = (1.0f - t) * alpha_start + alpha_end * t;
    }
}

// sub_5B9F60
void animation_controller_state::layer_anim::raise_animation_signals(f32 time, f32 prev_time, entity* the_entity) {
    ((void (__thiscall*)(layer_anim*, f32, f32, entity*))retail::sub_5B9F60)(this, time, prev_time, the_entity);
}

// sub_5D3F30
void treyarch::animation_controller_jq_callback(jq::batch* batch) {
    animation_controller::advance(((animation_controller_state::advance_job*)batch->payload[0])->controller);
}

// sub_5D3FB0
void treyarch::multi_animation_controller_jq_callback(jq::batch* batch) {
    auto controllers = (animation_controller**)batch->payload[0];

    for (i32 index = 0; index < batch->count; ++index) {
        animation_controller* controller = controllers[index];

        // the flush unhooks the ones that sit the frame out
        if (controller->state->job.controller == controller)
            animation_controller::advance(controller);
    }
}

// inlined @ sub_5D4050
// inlined @ sub_5D40F0
void treyarch::raise_newest_animation_signals(animation_records* records) {
    if (!records || !records->size())
        return;

    if (animation_controller_state::layer_anim* newest = records->back())
        newest->raise_animation_signals(newest->normalized_time, newest->prev_normalized_time, newest->owner->unk_0e4);
}

// sub_5D4050
void treyarch::animation_signal_jq_callback(jq::batch* batch) {
    raise_newest_animation_signals((animation_records*)batch->payload[0]);
}

// sub_5D40F0
void treyarch::multi_animation_signal_jq_callback(jq::batch* batch) {
    auto records = (animation_records**)batch->payload[0];

    for (i32 index = 0; index < batch->count; ++index)
        raise_newest_animation_signals(records[index]);
}
