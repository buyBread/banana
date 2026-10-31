#include <cmath>
#include <cstring>
#include <new>

#include "retail.hh"
#include "game/line_info.hh"
#include "game/math/references.hh"
#include "game/polytube.hh"
#include "game/wds/ai/ai_tentacle_engine.hh"
#include "game/wds/ai/ai_tentacle_info.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"

using namespace treyarch;

// sub_4855F0
ai_tentacle_info::~ai_tentacle_info() {
    destroy_tentacle(false);
}

// sub_4BBF80
void ai_tentacle_info::destruct_mashed_class() {
    destroy_tentacle(true);

    tween_positions.destruct_mashed_class();
    node_indices   .destruct_mashed_class();
    positions      .destruct_mashed_class();
    node_po_storage.destruct_mashed_class();

    engines         .clear();
    misc_render_pool.clear();

    widths_params.destruct_mashed_class();

    if (tentacle_info) {
        tentacle_info->destruct_mashed_class();
        tentacle_info = nullptr;
    }
}

// sub_4B6FD0
void ai_tentacle_info::initialize(mash::allocation_scope scope) { // onto all_tentacles either way
    my_entity       = arch_base_vhandle();
    tween_timer     = 0.0f;
    tween_duration  = -1.0f;
    tween_amount    = 0.0f;
    misc_engine_id  = 0;
    misc_render_ent = arch_base_vhandle();

    engines.clear();

    desired_code_blend = 0.0f;
    code_blend         = 0.0f;
    tentacle           = nullptr;
    code_blend_speed   = 1.0f;

    if (scope == mash::ALLOCATED) {
        misc_ent_resource.hash = string_hash();
        misc_ent_resource.type = 0;

        base_node_id = string_hash();
        end_node_id  = string_hash();
        base_node    = nullptr;
        end_node     = nullptr;

        void* memory = mm::allocate(sizeof(polytube_render_info));

        tentacle_info = memory ? (polytube_render_info*)retail::sub_639C30((u32*)memory) : nullptr; // polytube_render_info::polytube_render_info

        default_length = 1.0f;
        counter_window = 1.0f;
        cur_length     = 0.0f;
        tpm_delta      = 0.0f;
        flags          = dynamic | distance_lod | use_polytube | check_collisions;

        misc_render_pool.clear();

        misc_render_ent_dist          = -1.0f;
        max_misc_render_ents          = 0;
        default_misc_render_ent_color = color32(0xFF, 0xFF, 0xFF, 0xFF);
        length_percent                = 0.0f;
        mesh_column_index             = 4;
        mesh_row_index                = 4;
    } else {
        flags &= ~(dynamic | in_use);

        string_hash none;
        none.initialize(mash::ALLOCATED);

        entity* render_ent = nullptr;

        if (misc_ent_resource.hash != none) {
            mash::string directory("");
            string_hash  instance_name;

            retail::sub_64CD40(&instance_name.source_hash_code, (void*)"tentacle_misc_render"); // the next free instance name

            render_ent = (entity*)retail::sub_804460((u32*)&references::g_world_ptr.read()->ent_mgr,
                                                     (i32)misc_ent_resource.get_hash().source_hash_code,
                                                     &instance_name.source_hash_code,
                                                     (u64*)&references::unk_00fc4f60.get(),
                                                     (i32)&directory,
                                                     1,
                                                     0,
                                                     0); // built from its template
        }

        // the misc render entity is parked out of sight
        if (render_ent) {
            render_ent->set_active(false);
            render_ent->set_visible(true, false);

            vector3d parked(-999.0f, -999.0f, -999.0f);

            retail::sub_626CA0((u32*)render_ent, (u32*)&parked); // entity_base::set_rel_position
            retail::sub_612D70((u32*)render_ent, true);          // entity_base::remove_from_regions

            misc_render_ent = render_ent->my_handle;
        } else
            misc_render_ent = arch_base_vhandle();

        if (misc_render_ent.value) {
            misc_render_pool.clear();

            for (i32 i = 0; i < max_misc_render_ents; ++i) {
                void* memory = mm::allocate(sizeof(polytube_misc_render_object));

                auto object = memory ?
                    (polytube_misc_render_object*)retail::sub_639CF0((u32*)memory, (i32)misc_render_ent.value, 0, (u32*)&default_misc_render_ent_color) :
                    nullptr; // polytube_misc_render_object::polytube_misc_render_object

                misc_render_pool.push_back(object);
            }
        }
    }

    engine_lock_scope lock(&references::all_tentacles_lock.get());

    references::all_tentacles.get().push_back(this);
}

// sub_4C1D80
void ai_tentacle_info::create_tentacle(polytube* owner) { // without an owner the tentacle builds a polytube of its own
    string_hash name;

    if (tentacle)
        return;

    if (!(flags & use_polytube) && !owner)
        return;

    if (owner) {
        tentacle = owner;
        flags   |= use_polytube | external_polytube;

        init_num_positions(owner->get_num_control_pts() - 2, nullptr, nullptr);
        init_full_code_control();

        return;
    }

    flags &= ~external_polytube;

    void* memory = mm::allocate(sizeof(polytube));

    tentacle = memory ?
        (polytube*)retail::sub_667720((i32)memory, (i32*)retail::sub_64CD40(&name.source_hash_code, (void*)"ai_tentacle"), 0) :
        nullptr; // polytube::polytube

    retail::sub_804010((u32**)&references::g_world_ptr.read()->ent_mgr, (i32*)tentacle); // the world's entities take it

    tentacle->the_spline.set_force_start(true);

    retail::sub_653DA0((u32*)tentacle, (i32*)get_tentacle_parent());          // entity_base::set_parent
    retail::sub_626BE0((i32)tentacle, (u64*)&references::unk_00fc4f60.get()); // entity_base::set_rel_po

    i32 points = (i32)positions.size + 2;

    retail::sub_48AC10((u32*)tentacle, (u32)points); // polytube::reserve_control_pts

    for (; points > 0; --points)
        tentacle->add_control_pt(references::unk_00fc4a10.get());

    tentacle->set_visible(true, false);
    tentacle->set_external_tentacle_info(this);

    apply_render_info();

    const i32 widths = (i32)widths_params.size;

    tentacle->distance_lod = (flags & distance_lod) != 0;

    if (widths > 0) {
        retail::sub_64B670((u32*)tentacle, (i32)mesh_column_index, (i32)mesh_row_index); // the polytube's mesh grid
        retail::sub_65B430((i32)tentacle, widths);                                       // the polytube's widths count

        for (i32 i = 0; i < widths; ++i)
            retail::sub_62ED00((i32)tentacle, i, (i32)widths_params.array[i]); // the polytube's widths started from the params
    }
}

// sub_4B7330
void ai_tentacle_info::destroy_tentacle(bool keep_render_info) {
    if (!keep_render_info && tentacle_info) {
        tentacle_info->texture_id.finalize(mash::ALLOCATED);

        mm::free(tentacle_info);
        tentacle_info = nullptr;
    }

    kill_engine(misc_engine_id);

    // each engine is looked up by its own id
    while (engines.size) {
        if (engines.first_element)
            kill_engine(engines.first_element->id);
    }

    if (tentacle) {
        while (tentacle->misc_renders.size) {
            polytube_misc_render_object* object = tentacle->misc_renders.first_element;

            retail::sub_62A650((u32*)tentacle, (u32*)object); // polytube::remove_misc_render

            if (object)
                mm::free(object);
        }

        if (!(flags & external_polytube))
            retail::sub_7FBE10((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)tentacle); // destroyed now

        tentacle = nullptr;
    }

    while (misc_render_pool.size) {
        polytube_misc_render_object* object = misc_render_pool.first_element;

        if (object) {
            misc_render_pool.remove(object);

            mm::free(object);
        }
    }

    if (misc_render_ent.value) {
        if (arch_base* render_ent = misc_render_ent.resolve())
            retail::sub_7FBE10((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)render_ent); // destroyed now

        misc_render_ent = arch_base_vhandle();
    }

    engine_lock_scope lock(&references::all_tentacles_lock.get());

    references::all_tentacles.get().remove(this);
}

// sub_4B3690
void ai_tentacle_info::init_num_positions(i32 num_pos, entity* _base_node, entity* _end_node) { // nodes that are the owner's members are remembered by id
    base_node = _base_node;

    if (base_node &&
        (base_node->flags & entity_flag_member) &&
        (entity*)retail::sub_601910((u32*)base_node) == get_my_entity()) { // entity_base::get_attach_root

        flags        |= base_node_is_member;
        base_node_id  = base_node->id;
    } else
        base_node_id = string_hash();

    end_node = _end_node;

    if (end_node &&
        (end_node->flags & entity_flag_member) &&
        (entity*)retail::sub_601910((u32*)end_node) == get_my_entity()) { // entity_base::get_attach_root

        flags       |= end_node_is_member;
        end_node_id  = end_node->id;
    } else
        end_node_id = string_hash();

    while (positions.size < (u32)num_pos) {
        positions      .push_back(references::unk_00fc4a10.get());
        tween_positions.push_back(references::unk_00fc4a10.get());
    }
}

// sub_491BA0
void ai_tentacle_info::apply_render_info() {
    if (!tentacle)
        return;

    if (tentacle_info->texture_id.length())
        retail::sub_615760((u32*)tentacle, (char*)tentacle_info->texture_id.c_str()); // set_material

    tentacle->set_render_color(tentacle_info->render_color);
    tentacle->set_tube_radius(tentacle_info->radius);

    tentacle->tiles_per_meter = tentacle_info->tiles_per_meter;

    tentacle->set_num_sides(tentacle_info->num_sides);

    tentacle->pseudo_3d_offset = tentacle_info->pseudo_3d_offset;

    retail::sub_96F1B0((i32)&tentacle->the_spline, tentacle_info->num_subdivisions, tentacle->the_spline.spline_type); // spline::build
}

// sub_4914E0
entity* ai_tentacle_info::get_my_entity() const { // an external polytube stands in for a missing owner
    if (my_entity.resolve())
        return (entity*)my_entity.resolve();

    if (!tentacle || !(flags & external_polytube))
        return nullptr;

    return tentacle;
}

// sub_491520
// inlined @ sub_4B77A0
entity* ai_tentacle_info::get_tentacle_parent() const {
    entity* parent = get_my_entity();

    if ((flags & external_polytube) && tentacle)
        return tentacle->my_parent;

    if (base_node) {
        parent = base_node->my_parent;

        if (!parent)
            return base_node;
    }

    return parent;
}

// sub_4915E0
// inlined @ sub_4B4510
// inlined @ sub_4B77A0
vector3d ai_tentacle_info::get_base_position() const {
    return base_node ? base_node->my_abs_po->get_position() : base_pos;
}

// sub_491620
void ai_tentacle_info::set_base_position(const vector3d &pos) {
    base_pos = pos;
}

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
    const vector3d base = get_base_position();

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

// sub_491740
void ai_tentacle_info::set_end_po(po &the_po) {
    matrix4x4 &m = the_po.get_matrix();

    m.x.w = 0.0f;
    m.y.w = 0.0f;
    m.z.w = 0.0f;
    m.w.w = 1.0f;

    end_quat = quaternion(m);
    end_pos  = the_po.get_position();
}

// sub_491800
po ai_tentacle_info::get_end_po() const {
    po result = references::unk_00fc4f60.get();

    result.get_matrix().x.w = 0.0f;
    result.get_matrix().y.w = 0.0f;
    result.get_matrix().z.w = 0.0f;
    result.get_matrix().w.w = 1.0f;

    end_quat.to_matrix(&result.get_matrix());

    result.get_matrix().w = vector4d(end_pos.x, end_pos.y, end_pos.z, 1.0f);

    return result;
}

// sub_491900
vector3d ai_tentacle_info::get_abs_end_position() const { // the end node's position eased towards the end by the code blend
    if (!end_node)
        return end_pos;

    vector3d position = end_pos;

    if (is_tweening())
        position = compute_blend_pos(tween_end_pos, position, tween_amount);

    return compute_blend_pos(end_node->my_abs_po->get_position(), position, code_blend);
}

// sub_4919C0
po ai_tentacle_info::get_abs_end_po() const { // the end node's fourth column is rewritten
    if (!end_node)
        return get_end_po();

    const matrix4x4 &identity = references::unk_00fc4f60.get().get_matrix();

    po result;

    result.get_matrix().x = identity.x;
    result.get_matrix().y = identity.y;
    result.get_matrix().z = identity.z;

    quaternion rotation = end_quat;

    if (is_tweening())
        rotation = math::slerp(tween_end_quat, rotation, tween_amount);

    matrix4x4 &node_m = end_node->my_abs_po->get_matrix();

    node_m.x.w = 0.0f;
    node_m.y.w = 0.0f;
    node_m.z.w = 0.0f;
    node_m.w.w = 1.0f;

    rotation = math::slerp(quaternion(node_m), rotation, code_blend);

    result.get_matrix().x.w = 0.0f;
    result.get_matrix().y.w = 0.0f;
    result.get_matrix().z.w = 0.0f;

    rotation.to_matrix(&result.get_matrix());

    const vector3d position = get_abs_end_position();

    result.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

    return result;
}

// inlined @ sub_4BE710
// inlined @ sub_4C1D80
void ai_tentacle_info::init_full_code_control() {
    retail::sub_4B3910((i32)this, true); // init_positions

    set_code_blend(1.0f, 0.0f);
}

// inlined @ sub_4BE710
ai_tentacle_dangle* ai_tentacle_info::get_dangle_engine() {
    ai_tentacle_engine* engine = get_engine(misc_engine_id);

    return engine && engine->get_type() == tentacle_engine_dangle ? (ai_tentacle_dangle*)engine : nullptr;
}

// sub_4BE710
void ai_tentacle_info::start_dangle(f32 desired_length, const vector3d &initial_vel) { // a running dangle only takes the new length
    // a dangle engine that's gone faults here
    if (is_dangling()) {
        get_dangle_engine()->set_length(desired_length);

        return;
    }

    flags |= in_use;

    if (desired_length <= 0.0f)
        desired_length = default_length + 1.0f;

    init_full_code_control();

    void* memory = mm::allocate(sizeof(ai_tentacle_dangle));

    auto* engine = memory ? new (memory) ai_tentacle_dangle(this) : nullptr;

    retail::sub_4A07F0((i32)engine, desired_length, (i32)&initial_vel, true, true); // ai_tentacle_dangle::setup

    misc_engine_id = push_engine(engine);
}

// sub_4BE890
void ai_tentacle_info::stop_dangle(f32 time) { // the code blend eases out over the time
    if (!is_dangling())
        return;

    flags &= ~in_use;

    kill_engine(misc_engine_id);
    set_code_blend(0.0f, time);

    misc_engine_id = 0;
}

// sub_4B75B0
void ai_tentacle_info::frame_advance(f32 delta_t) { // only the first engine runs
    if (tentacle && ((tentacle->flags & entity_flag_in_limbo) || !tentacle->get_primary_region()))
        return;

    if (base_node)
        base_pos = base_node->my_abs_po->get_position();
    else if (tentacle && (flags & external_polytube) && !(flags & external_base_position))
        base_pos = tentacle->my_abs_po->get_position();

    if (code_blend_speed > 0.0f) {
        if (desired_code_blend > code_blend) {
            code_blend = (f32)((f64)code_blend_speed * delta_t + code_blend);

            if (code_blend > desired_code_blend)
                code_blend = desired_code_blend;
        } else if (code_blend > desired_code_blend) {
            code_blend = (f32)((f64)code_blend - (f64)code_blend_speed * delta_t);

            if (desired_code_blend > code_blend)
                code_blend = desired_code_blend;
        }
    }

    if (tween_duration >= tween_timer) {
        tween_timer += delta_t;

        if (tween_timer < tween_duration)
            tween_amount = tween_timer / tween_duration;
        else {
            tween_duration = -1.0f;
            tween_timer    = 0.0f;
            tween_amount   = 1.0f;
        }
    }

    if (engines.size && engines.first_element) {
        ai_tentacle_engine* engine = engines.first_element;

        if (engine->frame_advance(delta_t, false))
            kill_engine(engine->id);
    }

    if (tentacle && !(flags & external_polytube))
        tentacle->set_visible(get_my_entity()->is_visible(), false);
}

// sub_4B77A0
void ai_tentacle_info::update_spline(bool ignore_limbo) { // the polytube's control points follow the positions, in its parent's frame
    // the rotation turned back and the translation undone
    auto inverse_of = [](const po &frame) {
        const matrix4x4 &m = frame.get_matrix();

        matrix4x4 inverse;

        inverse.x.x = m.x.x;
        inverse.x.y = m.y.x;
        inverse.x.z = m.z.x;
        inverse.y.x = m.x.y;
        inverse.y.y = m.y.y;
        inverse.y.z = m.z.y;
        inverse.z.x = m.x.z;
        inverse.z.y = m.y.z;
        inverse.z.z = m.z.z;
        inverse.w.x = -0.0f - (f32)((f64)m.w.x * m.x.x + (f64)m.w.y * m.x.y + (f64)m.w.z * m.x.z);
        inverse.w.y = -0.0f - (f32)((f64)m.w.x * m.y.x + (f64)m.w.y * m.y.y + (f64)m.w.z * m.y.z);
        inverse.w.z = -0.0f - (f32)((f64)m.w.x * m.z.x + (f64)m.w.y * m.z.y + (f64)m.w.z * m.z.z);

        po result;
        retail::sub_44DA50((u64*)&result, (i32)&inverse); // po from the matrix's 3x4

        return result;
    };

    // a hidden polytube only keeps its misc render objects up
    const bool hidden = tentacle &&
                        !ignore_limbo &&
                        (!tentacle->is_visible() || (tentacle->flags & entity_flag_in_limbo) || !tentacle->get_primary_region());

    if (!hidden) {
        const bool need_po      = (flags & need_po_vector) && node_indices.size;
        const bool update_po    = (flags & update_node_po) && node_indices.size;
        const bool is_pseudo_3d = (flags & pseudo_3d)      && node_indices.size;

        if (base_node)
            base_pos = base_node->my_abs_po->get_position();
        else if (tentacle && (flags & external_polytube) && !(flags & external_base_position))
            base_pos = tentacle->my_abs_po->get_position();

        if (!under_code_control() && !(flags & check_collisions)) {
            if (tentacle) {
                tentacle->pseudo_3d          = is_pseudo_3d;
                tentacle->pseudo_3d_po_array = nullptr;
            }
        } else {
            line_info node_lif;
            bool      last_collided = false;
            vector3d  last_delta;
            vector3d  last_normal;

            // each point is run against the world from the last corrected one
            auto correct = [&]() {
                vector3d corrected;
                retail::sub_49F550((i32)this, (f32*)&corrected, (u8*)&node_lif, &last_collided, (f32*)&last_delta, (f32*)&last_normal, 0); // correct_tentacle_pos

                return corrected;
            };

            node_lif.start = get_base_position();

            // while it's set the members and the end node are left where they are
            const bool hold_nodes = retail::sub_967DF0();

            u32 po_index = 0;

            if (!node_indices.size) {
                if ((i32)positions.size > 0) {
                    const bool reverse = (flags & reverse_spline) != 0;
                    const i32  step    = reverse ? -1 : 1;

                    i32 index = reverse && tentacle ? tentacle->get_num_control_pts() - 1 : 0;

                    index += step;

                    for (vector3d* position = positions.begin(); position != positions.end(); ++position) {
                        node_lif.end   = *position;
                        node_lif.start = correct();

                        if (tentacle)
                            tentacle->the_spline.set_control_pt(index, node_lif.start);

                        index += step;
                    }
                }
            } else {
                entity* owner = get_my_entity();

                if (need_po) {
                    retail::sub_583280((u64*)node_po_storage.array, (u64*)base_node->my_abs_po); // po copy

                    po_index = 1;
                }

                // every member is looked up once more for a bounding box nothing reads
                if (!hold_nodes) {
                    for (e_conglom_member_index* member = node_indices.begin(); member != node_indices.end(); ++member) {
                        vector3d member_pos = owner->my_abs_po->get_position();

                        retail::sub_62FA60((i32)owner, *member, (f32*)&member_pos, 0); // conglomerate::get_member_abs_position
                    }
                }

                const vector3d* position       = positions.array;
                const vector3d* tween_position = tween_positions.array;

                for (e_conglom_member_index* member = node_indices.begin(); member != node_indices.end(); ++member, ++position, ++tween_position) {
                    if (!hold_nodes) {
                        vector3d target = *position;

                        if (is_tweening())
                            target = compute_blend_pos(*tween_position, target, tween_amount);

                        vector3d member_pos = owner->my_abs_po->get_position();

                        retail::sub_62FA60((i32)owner, *member, (f32*)&member_pos, 0); // conglomerate::get_member_abs_position

                        node_lif.end   = compute_blend_pos(member_pos, target, code_blend);
                        node_lif.start = correct();

                        retail::sub_640EA0((u32*)owner, *member, (f32*)&node_lif.start); // conglomerate::set_member_abs_position
                    }

                    if (need_po) {
                        po member_po = *owner->my_abs_po;

                        retail::sub_62FC10((i32)owner, *member, (u64*)&member_po, 0); // conglomerate::get_member_abs_po

                        node_po_storage.array[po_index++] = member_po;
                    }
                }
            }

            po end_po = get_end_po();

            if (!hold_nodes || !end_node) {
                end_po = get_abs_end_po();

                node_lif.end = end_po.get_position();

                const vector3d &zero = references::unk_00fc4a10.get();

                end_po.get_matrix().w = vector4d(zero.x, zero.y, zero.z, 1.0f);

                retail::sub_9595B0((i32)&end_po); // po: orthonormalize

                const vector3d corrected = correct();

                node_lif.start = corrected;

                end_po.get_matrix().w = vector4d(corrected.x, corrected.y, corrected.z, 1.0f);

                if (!end_node)
                    end_pos = corrected;
                else if (end_node->flags & entity_flag_bone_connector) {
                    auto root = (conglomerate*)retail::sub_601910((u32*)end_node); // entity_base::get_attach_root

                    retail::sub_6410F0((i32)root, root->get_member_index(end_node->id), (f32*)&end_po, 0); // conglomerate::set_member_abs_po
                } else
                    retail::sub_627180((i32)end_node, (f32*)&end_po); // entity_base::set_abs_po
            }

            if (need_po) {
                // a bone connector is placed through its conglomerate
                auto place = [](entity* node, po* node_po) {
                    if (node->flags & entity_flag_bone_connector) {
                        auto root = (u32*)retail::sub_601910((u32*)node); // entity_base::get_attach_root

                        retail::sub_48E860(root, &node->id.source_hash_code, (f32*)node_po, 0); // conglomerate::set_member_abs_po
                    } else
                        retail::sub_627180((i32)node, (f32*)node_po); // entity_base::set_abs_po
                };

                const po &end_source = end_node ? *end_node->my_abs_po : end_po;

                retail::sub_583280((u64*)&node_po_storage.array[po_index], (u64*)&end_source); // po copy

                entity* owner = get_my_entity();

                // each po's z turned at the next one
                for (u32 i = 0; i < node_po_storage.size; ++i) {
                    po* node_po = &node_po_storage.array[i];

                    if (!i) {
                        retail::sub_49FC40((i32)node_po, (i32)base_node->my_abs_po, (f32*)&node_po[1].get_position()); // po: z at the point

                        if (update_po && !hold_nodes && base_node)
                            place(base_node, node_po);
                    } else if (i - 1 < node_indices.size) {
                        retail::sub_49FC40((i32)node_po, (i32)&node_po[-1], (f32*)&node_po[1].get_position()); // po: z at the point

                        if (update_po && !hold_nodes)
                            retail::sub_6410F0((i32)owner, node_indices.array[i - 1], (f32*)node_po, 0); // conglomerate::set_member_abs_po
                    } else if (i == node_po_storage.size - 1) {
                        const vector3d &at    = node_po->get_position();
                        const vector3d &reach = node_po[-1].get_z_facing();

                        vector3d aim(at.x + reach.x, at.y + reach.y, at.z + reach.z);

                        retail::sub_49FC40((i32)node_po, (i32)&node_po[-1], (f32*)&aim); // po: z at the point

                        if (update_po && !hold_nodes && end_node)
                            place(end_node, &end_po);
                    }
                }

                // into the polytube's frame
                for (u32 i = 0; i < node_po_storage.size; ++i) {
                    po to_tube = inverse_of(*tentacle->my_abs_po);

                    retail::sub_44E230((u64*)&node_po_storage.array[i], (f32*)&node_po_storage.array[i], (f32*)&to_tube); // po product
                }
            }

            if (tentacle) {
                tentacle->pseudo_3d          = is_pseudo_3d;
                tentacle->pseudo_3d_po_array = need_po ? node_po_storage.array : nullptr;
            }

            retail::sub_798F10((u8*)&node_lif); // line_info::~line_info
        }

        entity* parent = get_tentacle_parent();

        if (tentacle && !(flags & external_polytube) && parent != tentacle) {
            retail::sub_626BE0((i32)tentacle, (u64*)&references::unk_00fc4f60.get()); // entity_base::set_rel_po
            retail::sub_653DA0((u32*)tentacle, (i32*)parent);                          // entity_base::set_parent
        }

        po to_local = references::unk_00fc4f60.get();

        if (tentacle && (flags & external_polytube))
            to_local = inverse_of(*tentacle->my_abs_po);
        else if (parent)
            to_local = inverse_of(*parent->my_abs_po);
        else if (tentacle)
            to_local = inverse_of(*tentacle->my_abs_po);

        const bool reverse = (flags & reverse_spline) != 0;

        cur_length = 0.0f;

        i32       index = reverse && tentacle ? tentacle->get_num_control_pts() - 1 : 0;
        const i32 step  = reverse ? -1 : 1;

        vector3d prev = to_local.slow_xform(get_base_position());

        if (tentacle)
            tentacle->the_spline.set_control_pt(index, prev);

        index += step;

        if (!node_indices.size) {
            for (i32 i = 0; i < (i32)positions.size; ++i) {
                vector3d point;

                // a corrected point is read back from the spline unchecked
                if (tentacle && (under_code_control() || (flags & check_collisions)))
                    point = to_local.slow_xform(tentacle->the_spline.control_pts.begin()[index]);
                else
                    point = to_local.slow_xform(positions.array[i]);

                if (tentacle)
                    tentacle->the_spline.set_control_pt(index, point);

                index += step;

                cur_length += (point - prev).length();

                prev = point;
            }
        } else {
            for (e_conglom_member_index* member = node_indices.begin(); member != node_indices.end(); ++member) {
                entity* owner = get_my_entity();

                vector3d member_pos = owner->my_abs_po->get_position();

                retail::sub_62FA60((i32)owner, *member, (f32*)&member_pos, 0); // conglomerate::get_member_abs_position

                const vector3d point = to_local.slow_xform(member_pos);

                if (tentacle)
                    tentacle->the_spline.set_control_pt(index, point);

                index += step;

                cur_length += (point - prev).length();

                prev = point;
            }
        }

        const vector3d end = to_local.slow_xform(end_node ? end_node->my_abs_po->get_position() : end_pos);

        if (tentacle)
            tentacle->the_spline.set_control_pt(index, end);

        cur_length += (end - prev).length();

        if (tentacle) {
            // the texture stretches with the tentacle
            if (tpm_delta > 0.0001f) {
                f32       tiles  = tentacle_info->tiles_per_meter;
                const f32 excess = cur_length - default_length;

                if (std::fabs(excess) > 0.0001f) {
                    f32 share = std::fabs(excess / tpm_delta);

                    if (share > 1.0f)
                        share = 1.0f;

                    const f32 bound = excess > 0.0f ? tentacle_info->min_tiles_per_meter : tentacle_info->max_tiles_per_meter;

                    tiles = (f32)(((f64)bound - tiles) * share + tiles);
                }

                tentacle->tiles_per_meter = tiles;
            }

            if (tentacle->the_spline.need_rebuild)
                retail::sub_96F000((i32)&tentacle->the_spline); // spline::rebuild_helper

            retail::sub_796C70((i32)tentacle->my_handle.value); // add_moved
        }
    }

    // one misc render object every misc_render_ent_dist, at most max_misc_render_ents
    if (tentacle && std::fabs(misc_render_ent_dist) > 0.0001f && max_misc_render_ents > 0) {
        const f32 length = (f32)retail::sub_97A1F0((u32*)&tentacle->the_spline, -1.0f); // spline::curve_length

        if (length > 0.0001f && 10000.0f > length) {
            bool clamped = false;

            const f64 per = misc_render_ent_dist > 0.0f ?
                (f64)length / misc_render_ent_dist :
                (f64)(f32)(max_misc_render_ents + 1);

            i32 count = (i32)per;

            if (count < 0)
                count = max_misc_render_ents + 1;

            if (count > max_misc_render_ents) {
                count   = max_misc_render_ents;
                clamped = true;
            }

            while (tentacle->misc_renders.size < count) {
                polytube_misc_render_object* object = misc_render_pool.first_element;

                misc_render_pool.remove(object);

                object->render_color = default_misc_render_ent_color;
                object->visible      = true;

                retail::sub_63A070((u32*)tentacle, (u32*)object); // polytube::add_misc_render
            }

            while (tentacle->misc_renders.size > count) {
                polytube_misc_render_object* object = tentacle->misc_renders.first_element;

                retail::sub_62A650((u32*)tentacle, (u32*)object); // polytube::remove_misc_render

                object->visible = false;

                misc_render_pool.push_back(object);
            }

            const f32 spacing = clamped ? 1.0f / (f32)max_misc_render_ents : misc_render_ent_dist / length;

            f32 at = spacing;

            for (polytube_misc_render_object* object = tentacle->misc_renders.first_element; object; object = object->simple_list_vars.next_element) {
                object->percent_pos = at;

                at += spacing;
            }
        }
    }
}

// sub_4BC100
void ai_tentacle_info::frame_advance_all_tentacles(f32 delta_t) { // each on its owner's time
    engine_lock_scope lock(&references::all_tentacles_lock.get());

    for (ai_tentacle_info* info = references::all_tentacles.get().first_element; info; info = info->simple_list_vars.next_element) {
        if (info->flags & manual_frame_advance)
            continue;

        entity* owner = info->get_my_entity();

        if (!owner) {
            info->frame_advance(delta_t);

            continue;
        }

        const f32 dilation = owner->has_ifc(entity_ifc_time) ?
            owner->time_ifc()->calc_time_dilation() :
            references::g_world_ptr.read()->time_mgr.get_time_dilation_factor();

        info->frame_advance(dilation * delta_t);
    }
}

// sub_4BC290
void ai_tentacle_info::update_spline_on_all_tentacles() {
    engine_lock_scope lock(&references::all_tentacles_lock.get());

    for (ai_tentacle_info* info = references::all_tentacles.get().first_element; info; info = info->simple_list_vars.next_element) {
        if (!(info->flags & manual_update_spline))
            info->update_spline(false);
    }
}
