#include <cmath>
#include <cstring>

#include "retail.hh"
#include "game/wds/ai/ai_tentacle_engine.hh"
#include "game/wds/ai/ai_tentacle_info.hh"
#include "game/wds/entity/entity.hh"

using namespace treyarch;

// sub_491C90
ai_tentacle_info* ai_tentacle_info::find_tentacle(const string_hash &search_id, ai_tentacle_info** array, i32 size) {
    for (i32 i = 0; i < size; ++i) {
        if (array[i]->id == search_id)
            return array[i];
    }

    return nullptr;
}

// sub_4915D0
i32 ai_tentacle_info::get_num_positions() const {
    return (i32)positions.size;
}

// sub_4A01B0
const vector3d &ai_tentacle_info::get_position(i32 index) const {
    return positions.array[index];
}

// sub_4A01D0
void ai_tentacle_info::set_position(i32 index, const vector3d &pos) {
    positions.array[index] = pos;
}

// sub_491710
void ai_tentacle_info::set_end_position(const vector3d &pos) {
    end_pos = pos;
}

// sub_4917F0
const vector3d &ai_tentacle_info::get_end_position() const {
    return end_pos;
}

// sub_4B4510
void ai_tentacle_info::create_line(const vector3d &end, const vector3d* end_face) { // the positions spaced evenly from the base towards the end
    const vector3d base = base_node ? base_node->my_abs_po->get_position() : base_pos;

    vector3d  dir    = end - base;
    const f32 length = dir.length();

    // too short to measure keeps the raw direction
    if (length > 0.0001f)
        dir *= 1.0f / length;

    const f32 step = length / (f32)((i32)positions.size + 1);

    for (i32 i = 0; i < (i32)positions.size; ++i) {
        const f32 along = (f32)(i + 1) * step;

        positions.array[i] = vector3d(base.x + dir.x * along,
                                      base.y + dir.y * along,
                                      base.z + dir.z * along);
    }

    end_pos = end;

    retail::sub_4A0200((i32)this, (f32*)end_face); // set_end_facing
}

// sub_4B4B50
u32 ai_tentacle_info::push_engine(ai_tentacle_engine* engine) {
    engines.push_front(engine);

    return engine->id;
}

// sub_4B4B90
ai_tentacle_engine* ai_tentacle_info::get_engine(u32 engine_id) {
    for (ai_tentacle_engine* engine = engines.first_element; engine; engine = engine->simple_list_vars.next_element) {
        if (engine->id == engine_id)
            return engine;
    }

    return nullptr;
}

// sub_4B4BC0
bool ai_tentacle_info::kill_engine(u32 engine_id) {
    ai_tentacle_engine* engine = get_engine(engine_id);

    if (!engine)
        return false;

    engines.remove(engine);
    engine->destroy();

    return true;
}

// sub_4B9CB0
void ai_tentacle_info::init_code_tween(f32 time) { // the tween starts from where the positions are now
    if (!under_code_control())
        return;

    if (0.01f > time) {
        tween_duration = -1.0f;
        tween_amount   = 1.0f;
    } else {
        retail::sub_4B3910((i32)this, true); // init_positions

        std::memcpy(tween_positions.array, positions.array, tween_positions.size * sizeof(vector3d));

        tween_end_pos  = end_pos;
        tween_end_quat = end_quat;
        tween_amount   = 0.0f;
    }

    tween_timer    = 0.0f;
    tween_duration = time;
}

// sub_4BC320
void ai_tentacle_info::set_code_blend(f32 blend, f32 time) { // eases towards the blend over the time
    desired_code_blend = blend;

    if (blend > 1.0f)
        desired_code_blend = 1.0f;
    else if (0.0f > blend)
        desired_code_blend = 0.0f;

    if (0.01f > time) {
        code_blend_speed = 0.0f;
        code_blend       = desired_code_blend;
    } else
        code_blend_speed = std::fabs(desired_code_blend - code_blend) / time;

    if (desired_code_blend > 0.0001f && under_code_control())
        init_code_tween(time);
}

// sub_4BC3E0
bool ai_tentacle_info::is_dangling() {
    ai_tentacle_engine* engine = get_engine(misc_engine_id);

    return engine && engine->get_type() == tentacle_engine_dangle;
}
