#include <algorithm>
#include <cmath>
#include <new>

#include "retail.hh"
#include "game/als/meta_anim.hh"
#include "game/animation/animation_controller.hh"
#include "game/fixed_string.hh"
#include "game/frontend/igo/igo_3d_text.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/game_meter_manager.hh"
#include "game/hash/algo.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/base/tl_fixed_string.hh"
#include "treyarch/base/tl_system.hh"
#include "treyarch/jq/native_locks.hh"
#include "treyarch/mm/mm.hh"
#include "treyarch/nal/nal.hh"
#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/mesh/submission.hh"

using namespace treyarch;

// sub_6F83E0
igo_3d_widget::~igo_3d_widget() {
    member_texts.clear();
    member_widgets.clear();

    reset_anim_speeds();
    release_entity();

    if (parameters) {
        if (flags & flag_parameters_from_mash) {
            parameters->destruct_mashed_class();

            ((u8*)parameters)[-4] = 0;
        } else {
            retail::sub_6F1E70((u32*)parameters);
            mm::free(parameters);
        }

        flags      &= ~flag_parameters_from_mash;
        parameters  = nullptr;
    }
}

// slot 1
void igo_3d_widget::init() {}

// sub_699E00
void igo_3d_widget::update(f32 time_inc) {
    if (is_anim_playing() || moving)
        move_elapsed = (f32)((f64)move_elapsed + time_inc);

    if (!is_anim_playing() && !moving)
        return;

    if (move_elapsed == 0.0f)
        set_position(move_from.x, move_from.y, move_from.z);
    else if ((f64)move_elapsed >= move_duration) {
        moving = false;

        set_position(move_to.x, move_to.y, move_to.z);
    } else {
        f32 progress = move_curve_value((f32)((f64)move_elapsed / move_duration), move_curve);

        set_position((f32)(((f64)move_to.x - move_from.x) * progress + move_from.x),
                     (f32)(((f64)move_to.y - move_from.y) * progress + move_from.y),
                     (f32)(((f64)move_to.z - move_from.z) * progress + move_from.z));
    }
}

// sub_68AD00
f32 igo_3d_widget::move_curve_value(f32 t, i32 curve) {
    if (curve == 1) {
        f32 value = (f32)((f32)(9.0 * t) + 1.0);

        return (f32)std::log10((f64)value);
    }

    if (curve == 0) {
        f32 value = (f32)(9.0 * (1.0f - t)) + 1.0f;

        return 1.0f - (f32)std::log10((f64)value);
    }

    return t;
}

// sub_6911A0
void igo_3d_widget::render() {
    if (is_shown())
        render_entity();
}

// sub_698430
u8 igo_3d_widget::is_shown() {
    return flags & flag_shown;
}

// sub_6AE420
void igo_3d_widget::set_shown(bool shown) {
    if (shown)
        flags |= flag_shown;
    else
        flags &= ~flag_shown;
}

// sub_6B1C30
void igo_3d_widget::set_position(f32 x, f32 y, f32 z) {
    parameters->position.set_x(x);
    parameters->position.set_y(y);
    parameters->position.set_z(z);

    parameters->update_transform_matrix();
}

// sub_6B1C00
void igo_3d_widget::set_center_pos(f32 x, f32 y) {
    parameters->position.set_x(x);
    parameters->position.set_y(y);

    parameters->update_transform_matrix();
}

// sub_689890
void igo_3d_widget::get_position(f32* x, f32* y) {
    *x = parameters->position.get_x();
    *y = parameters->position.get_y();
}

// sub_6B1C70
void igo_3d_widget::set_position_z(f32 z) {
    parameters->position.set_z(z);

    parameters->update_transform_matrix();
}

// sub_6B1C90
void igo_3d_widget::set_rotation_angle(f32 angle) {
    parameters->rotation_angle = angle;

    parameters->update_transform_matrix();
}

// sub_6B1CB0
void igo_3d_widget::set_tilt_angle(f32 angle) {
    parameters->tilt_angle = angle;

    parameters->update_transform_matrix();
}

// sub_6B1CD0
void igo_3d_widget::set_spin_angle(f32 angle) {
    parameters->spin_angle = angle;

    parameters->update_transform_matrix();
}

// sub_68FAD0
void igo_3d_widget::set_scale(f32 scale) {
    parameters->scale = scale;
}

// sub_6898B0
void igo_3d_widget::set_scale_offset(f32 x, f32 y, f32 z) {
    scale_offset.x = x;
    scale_offset.y = z;
    scale_offset.z = y;
}

// sub_6898E0
void igo_3d_widget::set_alpha(f32 value) {
    alpha = value;
}

// sub_6B1CF0
void igo_3d_widget::set_widescreen_alignment(igo_3d_widget_parameters::e_wide_align_flags alignment) {
    parameters->widescreen_align = alignment;

    parameters->update_transform_matrix();
}

// sub_6898F0
void igo_3d_widget::set_suppression_mask(u32 mask) {
    parameters->suppression_mask = mask;
}

// sub_689900
void igo_3d_widget::set_color(f32 r, f32 g, f32 b) {
    color.x = std::clamp(r, 0.0f, 1.0f);
    color.y = std::clamp(g, 0.0f, 1.0f);
    color.z = std::clamp(b, 0.0f, 1.0f);
}

// sub_689980
void igo_3d_widget::set_uv_offset(f32 u, f32 v) {
    uv_offset.x = u;
    uv_offset.y = v;
}

// slot 20
void igo_3d_widget::unk_020(i32 value) {
    unk_044 = value;
}

// sub_4F8A30
void igo_3d_widget::unk_021(i32 value) {
    unk_048 = value;
}

// sub_687260
f32 igo_3d_widget::get_position_x() {
    return parameters->position.get_x();
}

// sub_687270
f32 igo_3d_widget::get_position_y() {
    return parameters->position.get_y();
}

// sub_687280
f32 igo_3d_widget::get_position_z() {
    return parameters->position.get_z();
}

// sub_6F8580
f32 igo_3d_widget::get_rotation_angle() {
    return parameters->rotation_angle;
}

// sub_6F8590
f32 igo_3d_widget::get_tilt_angle() {
    return parameters->tilt_angle;
}

// sub_6F85A0
f32 igo_3d_widget::get_spin_angle() {
    return parameters->spin_angle;
}

// sub_687290
f32 igo_3d_widget::get_scale() {
    return parameters->scale;
}

// sub_90D180
f32 igo_3d_widget::get_alpha() {
    return alpha;
}

// sub_6F85B0
bool igo_3d_widget::anims_enabled() {
    return parameters->anims_enabled != 0;
}

// sub_6F85C0
igo_3d_widget_parameters::e_wide_align_flags igo_3d_widget::get_widescreen_alignment() {
    return parameters->widescreen_align;
}

// sub_6F85D0
u32 igo_3d_widget::suppression_mask() {
    return parameters->suppression_mask;
}

// sub_699B40
void igo_3d_widget::play_anim_50() {
    if (!parameters->anims_enabled || !parameters->anim_50.length())
        return;

    kill_layer_0_anims();
    reset_entity_po();

    tl_fixed_string name(parameters->anim_50.c_str());

    play_anim(name);
}

// sub_699BC0
void igo_3d_widget::play_anim_5c() {
    if (!parameters->anims_enabled || !parameters->anim_5c.length())
        return;

    kill_layer_0_anims();
    reset_entity_po();

    tl_fixed_string name(parameters->anim_5c.c_str());

    play_anim(name);
}

// sub_6F6750
void igo_3d_widget::set_scale_factor(f32 factor) {
    scale_factor = factor;
}

// sub_6F85E0
f32 igo_3d_widget::get_scale_factor() {
    return scale_factor;
}

// sub_6F85F0
void igo_3d_widget::set_alpha_factor(f32 factor) {
    alpha_factor = factor;
}

// sub_6F6760
f32 igo_3d_widget::get_alpha_factor() {
    return alpha_factor;
}

// sub_68AC20
void igo_3d_widget::play_sound(string_hash sound) {
    if (is_shown())
        retail::sub_900D60(sound.source_hash_code, 0x10000000, 0); // post the sound event on the global game object
}

// sub_68AC60
void igo_3d_widget::start_timed_move(f32 duration, f32 from_x, f32 from_y, f32 from_z, f32 to_x, f32 to_y, f32 to_z, i32 curve) {
    if (is_anim_playing() || 0.0f > duration)
        return;

    move_elapsed  = 0.0f;
    move_duration = duration;
    move_from.x   = from_x;
    move_from.y   = from_y;
    move_from.z   = from_z;
    move_to.x     = to_x;
    move_to.y     = to_y;
    move_to.z     = to_z;
    moving        = true;
    move_curve    = curve;
}

// slot 41
void igo_3d_widget::render_entity() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_6D9550)(this);
}

// sub_6B1DB0
void igo_3d_widget::spawn_entity() {
    if (entity_spawned)
        return;

    entity_spawned = true;

    if (entity_vh.value)
        return;

    if (parameters->entity_name.length()) {
        string_hash name;
        name.initialize(mash::ALLOCATED, parameters->entity_name.c_str());

        init_entity(name);

        if (parameters->anims_enabled) {
            init_anim_controller();

            if (parameters->spawn_anim.length()) {
                tl_fixed_string anim(parameters->spawn_anim.c_str());

                play_anim(anim);
            }
        }
    }

    create_child_texts();
    create_child_widgets();
}

// sub_689A40
void igo_3d_widget::render_mesh(ngl::mesh*              mesh,
                                const matrix4x4        &trans_matrix,
                                ngl::mesh_params*       mesh_params,
                                ngl::shader_param_set*  shader_params) {

    alignas(16) matrix4x4 local = trans_matrix;

    retail::sub_9DC110((i32)mesh, (f32*)&local, (u32*)mesh_params, (u32*)shader_params, 0);
}

// sub_689AD0
void igo_3d_widget::render_congomerate_mesh(ngl::mesh*              mesh,
                                            i32,
                                            const matrix4x4        &trans_matrix,
                                            const matrix4x4        &rel_pos_matrix,
                                            ngl::mesh_params*       mesh_params,
                                            ngl::shader_param_set*  shader_params) {

    alignas(16) matrix4x4 local = math::matrix4x4_mul(rel_pos_matrix, trans_matrix);

    retail::sub_9DC110((i32)mesh, (f32*)&local, (u32*)mesh_params, (u32*)shader_params, 0);
}

// sub_68A130
void igo_3d_widget::render_mesh_from_parameters(ngl::mesh*              mesh,
                                                i32,
                                                ngl::mesh_params*       mesh_params,
                                                ngl::shader_param_set*  shader_params) {

    alignas(16) matrix4x4 local = *parameters->matrix;

    retail::sub_9DC110((i32)mesh, (f32*)&local, (u32*)mesh_params, (u32*)shader_params, 0);
}

// sub_68A1C0
void igo_3d_widget::render_text(igo_3d_text*            text,
                                const matrix4x4        &trans_matrix,
                                const matrix4x4        &rel_pos_matrix,
                                ngl::mesh_params*,
                                ngl::shader_param_set*) {

    if (text)
        text->render(trans_matrix, rel_pos_matrix);
}

// sub_68A1E0
void igo_3d_widget::render_child_widget(igo_3d_widget*          child,
                                        const matrix4x4        &trans_matrix,
                                        const matrix4x4        &rel_pos_matrix,
                                        ngl::mesh_params*,
                                        ngl::shader_param_set*) {

    if (!child)
        return;

    *child->parameters->matrix = math::matrix4x4_mul(rel_pos_matrix, trans_matrix);

    child->render();
}

// sub_6B1E40
void igo_3d_widget::render_value_mesh(ngl::mesh*              mesh,
                                      i32                     index,
                                      const matrix4x4        &trans_matrix,
                                      const matrix4x4        &rel_pos_matrix,
                                      ngl::mesh_params*       mesh_params,
                                      ngl::shader_param_set*  shader_params) {

    if (!parameters->value_source || !parameters->value_display) {
        render_congomerate_mesh(mesh, index, trans_matrix, rel_pos_matrix, mesh_params, shader_params);

        return;
    }

    f32 min   = parameters->value_min;
    f32 max   = parameters->value_max;
    f32 value = 0.0f;

    switch (parameters->value_source) {
        case 1:
            if (entity* hero = references::g_world_ptr.read()->hero_ptr)
                value = (f32)(retail::sub_5BAB80((u32*)hero->damage_ifc()) * (max - min)) + min; // damage_interface hit point fraction

            break;

        case 2:
            value = (f32)(retail::sub_933AB0((i32*)references::game_meter_manager.read(), 0) * (max - min)) + min; // the meter's fraction

            break;

        case 3:
            value = (f32)(retail::sub_933AB0((i32*)references::game_meter_manager.read(), 1) * (max - min)) + min;

            break;
    }

    f32 offset_x;
    f32 offset_y;

    switch (parameters->value_display) {
        case 2:
            offset_x = value;
            offset_y = 0.0f;

            break;

        case 3:
            offset_x = 0.0f;
            offset_y = value;

            break;

        case 10:
            offset_x = -0.0f - value;
            offset_y = 0.0f;

            break;

        case 4:
            unk_021((i32)value);
            render_congomerate_mesh(mesh, index, trans_matrix, rel_pos_matrix, mesh_params, shader_params);

            return;

        case 5:
            unk_020((i32)value);
            render_congomerate_mesh(mesh, index, trans_matrix, rel_pos_matrix, mesh_params, shader_params);

            return;

        default:
            return;
    }

    ngl::shader_param_set offset_params;

    retail::sub_620EA0((u32*)&offset_params); // shader_param_set::shader_param_set

    if (shader_params)
        retail::sub_686F60((void**)&offset_params, (const void**)shader_params); // shader_param_set::copy

    jq::references::unk_011106e0.get().acquire();

    void* memory = ngl::list_alloc(0x40, 0x10);
    auto* offset = memory ? (matrix4x4*)retail::sub_686F90((u64*)memory, offset_x, offset_y, 1.0f) : nullptr; // a translation matrix

    retail::sub_6B0F20((u64**)&offset_params, (i32*)&offset); // shader_param_set::set

    jq::references::unk_011106e0.get().release();

    render_congomerate_mesh(mesh, index, trans_matrix, rel_pos_matrix, mesh_params, &offset_params);
}

// sub_6997E0
void igo_3d_widget::render_value_text(igo_3d_text*            text,
                                      const matrix4x4        &trans_matrix,
                                      const matrix4x4        &rel_pos_matrix,
                                      ngl::mesh_params*       mesh_params,
                                      ngl::shader_param_set*  shader_params) {

    if (!text)
        return;

    if (!text->current_parameter_set()->value_source ||
        !text->current_parameter_set()->value_display) {

        render_text(text, trans_matrix, rel_pos_matrix, mesh_params, shader_params);

        return;
    }

    f32 min   = text->current_parameter_set()->value_min;
    f32 max   = text->current_parameter_set()->value_max;
    f32 value = 0.0f;

    if (text->current_parameter_set()->value_source == 1) {
        if (entity* hero = references::g_world_ptr.read()->hero_ptr)
            value = (f32)(retail::sub_5BAB80((u32*)hero->damage_ifc()) * (max - min)) + min; // damage_interface hit point fraction
    }

    if (text->current_parameter_set()->value_display != 1)
        return;

    alignas(mash::string) u8 storage[sizeof(mash::string)];

    auto* formatted = new (storage) mash::string(mash::string::fmt, "%d", (i32)value);

    text->set_text_no_localize(formatted->c_str());

    render_text(text, trans_matrix, rel_pos_matrix, mesh_params, shader_params);

    formatted->finalize(mash::ALLOCATED);
}

// sub_70B160
void igo_3d_widget::create_child_texts() {
    for (igo_3d_text_child* child : parameters->text_children) {
        igo_3d_text* text = nullptr;

        void* memory = mm::allocate(sizeof(igo_3d_text));

        if (memory) {
            string_hash record;
            record.initialize(mash::ALLOCATED, child->record_name.c_str());

            text = (igo_3d_text*)retail::sub_701610(memory, record.source_hash_code); // igo_3d_text::igo_3d_text
        }

        if (!text)
            continue;

        if (child->member_name.length()) {
            string_hash member;
            member.initialize(mash::ALLOCATED, child->member_name.c_str());

            attach_text(text, member);
        } else
            unattached_texts.push_back(text);
    }
}

// sub_700FC0
void igo_3d_widget::create_child_widgets() {
    for (igo_3d_widget_child* child : parameters->widget_children) {
        igo_3d_widget* widget = nullptr;

        void* memory = mm::allocate(sizeof(igo_3d_widget));

        if (memory) {
            string_hash record;
            record.initialize(mash::ALLOCATED, child->record_name.c_str());

            widget = (igo_3d_widget*)retail::sub_700D00((i32)memory, (i32)record.source_hash_code); // igo_3d_widget::igo_3d_widget
        }

        if (!widget)
            continue;

        if (child->member_name.length()) {
            string_hash member;
            member.initialize(mash::ALLOCATED, child->member_name.c_str());

            attach_widget(widget, member);
        } else
            unattached_widgets.push_back(widget);

        widget->init();
        widget->spawn_entity();
    }
}

// slot 52
void igo_3d_widget::unk_052() {}

// slot 53
void igo_3d_widget::unk_053() {}

// sub_699C40
// inlined @ sub_6DE490
void igo_3d_widget::fade_in(f32 duration, f32 from_alpha, f32 to_alpha, f32 delay) {
    fade_in_duration   = duration;
    fade_in_remaining  = (f32)((f64)duration + delay);
    fade_out_remaining = 0.0f;
    fading_in          = true;
    fading_out         = false;
    fade_in_alpha_span = (f32)((f64)to_alpha - from_alpha);
    fade_in_alpha_base = from_alpha;
    faded_in           = true;

    update_fade(0.0f);
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

    update_fade(0.0f);
}

// sub_68A920
void igo_3d_widget::update_fade(f32 dt) {
    if (fading_in) {
        fade_in_remaining = (f32)((f64)fade_in_remaining - dt);

        if (fade_in_remaining <= 0.0f) {
            fade_in_remaining = 0.0f;
            fading_in         = false;
            faded_in          = true;

            set_alpha((f32)((f64)fade_in_alpha_base + fade_in_alpha_span));
        }

        if (!((f64)fade_in_remaining > fade_in_duration) && fade_in_duration != 0.0f)
            set_alpha((f32)(((f64)fade_in_duration - fade_in_remaining) / fade_in_duration * fade_in_alpha_span + fade_in_alpha_base));
    }

    if (fading_out) {
        fade_out_remaining = (f32)((f64)fade_out_remaining - dt);

        if (fade_out_remaining <= 0.0f) {
            fade_out_remaining = 0.0f;
            fading_out         = false;
            faded_in           = false;

            set_alpha(fade_out_alpha_base);
        }

        if (!((f64)fade_out_remaining > fade_out_duration) && fade_out_duration != 0.0f)
            set_alpha((f32)((f64)fade_out_remaining / fade_out_duration * fade_out_alpha_span + fade_out_alpha_base));
    }
}

// sub_6B1D10
void igo_3d_widget::init_entity(string_hash name) {
    if (entity_vh.value)
        release_entity();

    auto ent = (entity_base*)retail::sub_804740((u32*)&references::g_world_ptr.read()->ent_mgr,
                                                (i32)name.source_hash_code,
                                                (i32*)1); // wds_entity_manager::spawn_entity_autoname

    if (!ent)
        return;

    entity_vh = ent->my_handle;

    ent->set_visible(false, false);
    ent->set_collisions_active(false, false);

    *(po*)&trans_matrix = *ent->my_abs_po;
}

// sub_699980
void igo_3d_widget::init_anim_controller() {
    arch_base* ent = entity_vh.resolve();

    if (!ent || !ent->is_an_actor())
        return;

    if (!((actor*)ent)->anim_ctrl)
        ((actor*)ent)->allocate_anim_controller();

    reset_anim_speeds();
    kill_layer_0_anims();
}

// sub_699A10
void igo_3d_widget::play_anim(const tl_fixed_string &name) {
    if (!entity_spawned) {
        spawn_entity();

        entity_spawned = true;
    }

    arch_base* ent = entity_vh.resolve();

    if (!ent || !ent->is_an_actor())
        return;

    animation_controller* anim_ctrl = ((actor*)ent)->anim_ctrl;

    if (!anim_ctrl)
        return;

    fixed_string anim_name;

    anim_name.hash = string_hash(hash::djb2(name.text));
    anim_name.set_text(name.text);

    nal::anim* anim = nal::get_anim(&anim_name);

    if (anim_name.text)
        mm::free(anim_name.text);

    if (!anim) {
        tl_fatal("nalGetAnim returned NULL in IGO3dWidget::PlayAnim. Anim name = %s", name.text);

        return;
    }

    als::nal_als_anim_bridge bridge = { anim, nullptr };

    retail::sub_5E8870((i32*)anim_ctrl, (u32*)&bridge, 0.0f, 32.0f, 1, 0, 0, 0, 0); // animation_controller::play on layer 0
}

// sub_68A860
// inlined @ sub_699980
// inlined @ sub_699B40
// inlined @ sub_699BC0
void igo_3d_widget::kill_layer_0_anims() {
    arch_base* ent = entity_vh.resolve();

    if (!ent || !ent->is_an_actor())
        return;

    if (animation_controller* anim_ctrl = ((actor*)ent)->anim_ctrl)
        anim_ctrl->kill_layer_anims(0);
}

// inlined @ sub_699980
// inlined @ sub_6D7E70
// inlined @ sub_6D7F90
// inlined @ sub_6D8070
// inlined @ sub_6D8190
// inlined @ sub_6D8270
// inlined @ sub_6D8380
// inlined @ sub_6F83E0
// inlined @ sub_6F9A90
void igo_3d_widget::reset_anim_speeds() {
    arch_base* ent = entity_vh.resolve();

    if (!ent || !ent->is_an_actor())
        return;

    if (animation_controller* anim_ctrl = ((actor*)ent)->anim_ctrl)
        anim_ctrl->reset_speeds();
}

// inlined @ sub_6B1D10
// inlined @ sub_6F83E0
// inlined @ sub_6F9470
// inlined @ sub_6F90E0
// inlined @ sub_6F9A90
// inlined @ sub_6FBD30
// inlined @ sub_6FC080
void igo_3d_widget::release_entity() {
    if (entity_vh.value) {
        if (arch_base* ent = entity_vh.resolve())
            retail::sub_7FBF20((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)ent); // wds_entity_manager::destroy_entity
    }

    entity_vh      = arch_base_vhandle();
    entity_spawned = false;
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

// sub_68A900
// inlined @ sub_699B40
// inlined @ sub_699BC0
void igo_3d_widget::reset_entity_po() {
    if (auto ent = (entity_base*)entity_vh.resolve())
        ent->set_rel_po(*(const po*)&trans_matrix);
}

// sub_6899A0
// inlined @ sub_6B2700
// inlined @ sub_6F8600
// inlined @ sub_6F8670
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

// sub_6F8600
void igo_3d_widget::attach_text(igo_3d_text* child, const string_hash &member) {
    i32 index = get_mesh_index(member);

    if (child && index != invalid_conglom_member)
        member_texts[index] = child;
}

// sub_6F8670
void igo_3d_widget::attach_widget(igo_3d_widget* child, const string_hash &member) {
    i32 index = get_mesh_index(member);

    if (index != invalid_conglom_member)
        member_widgets[index] = child;
}
