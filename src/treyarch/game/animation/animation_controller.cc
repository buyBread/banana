#include "retail.hh"
#include "treyarch/game/animation/animation_controller.hh"
#include "treyarch/game/animation/animation_controller_jq.hh"

using namespace treyarch;

// sub_5D2A20
void animation_controller::get_matrix_data_from_pose(bool unk) {
    retail::sub_5D2A20((i64**)this, unk);
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
