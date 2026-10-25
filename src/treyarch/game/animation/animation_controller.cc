#include <cstring>

#include "retail.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/animation/animation_controller_jq.hh"
#include "treyarch/game/animation/animation_pose_jq.hh"
#include "treyarch/game/wds/entity/entity.hh"
#include "treyarch/nal/transport_fk.hh"
#include "treyarch/nal/transport_list.hh"

using namespace treyarch;

// sub_5D2A20
void animation_controller::get_matrix_data_from_pose(bool unk) {
    retail::sub_5D2A20((i64**)this, unk);
}

// sub_5D4290
bool animation_controller::add_pose_jq_batch(scratch_arena* arena, bool immediate) {
    using get_pose_t = void (__thiscall*)(void*, f32, f32, nal::pose*, nal::transport_fk*);

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
        animation_controller_state::unk_02c_entry*            entry  = record.entry;

        input.t_prev[index] = record.prev_time;
        input.t[index]      = record.time;
        input.poses[index]  = &entry->pose;

        if (entry->nal_inst) {
            input.instances[index] = entry->nal_inst;
        } else {
            input.instances[index] = nullptr;

            // slot 2
            ((get_pose_t)(*(void***)entry->als_inst)[2])(entry->als_inst, record.time, record.prev_time, &entry->pose, fk);
        }

        if (index)
            input.alphas[index] = entry->alpha;
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
    ((void (__cdecl*)(animation_controller*))retail::sub_5D3950)(controller);
}

// sub_5B9F60
void animation_controller_state::unk_02c_entry::raise_animation_signals(f32 time, f32 prev_time, entity* the_entity) {
    ((void (__thiscall*)(unk_02c_entry*, f32, f32, entity*))retail::sub_5B9F60)(this, time, prev_time, the_entity);
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

    if (animation_controller_state::unk_02c_entry* newest = records->back())
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
