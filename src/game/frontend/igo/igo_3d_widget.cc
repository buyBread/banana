#include "retail.hh"
#include "game/animation/animation_controller.hh"
#include "game/frontend/igo/igo_3d_widget.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/mm/mm.hh"

using namespace treyarch;

// sub_6F83E0
igo_3d_widget::~igo_3d_widget() {
    member_texts.clear();
    member_widgets.clear();

    reset_anim_speeds();
    release_entity();

    if (parameters) {
        if (flags & 2) {
            parameters->destruct_mashed_class();

            ((u8*)parameters)[-4] = 0;
        } else {
            retail::sub_6F1E70((u32*)parameters);
            mm::free(parameters);
        }

        flags      &= ~2;
        parameters  = nullptr;
    }
}

// slot 1
void igo_3d_widget::init() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_5B4A70)(this);
}

// slot 2
void igo_3d_widget::update(f32 time_inc) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_699E00)(this, time_inc);
}

// slot 3
void igo_3d_widget::render() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_6911A0)(this);
}

// slot 4
u8 igo_3d_widget::is_shown() {
    return ((u8 (__thiscall*)(igo_3d_widget*))retail::sub_698430)(this);
}

// slot 5
void igo_3d_widget::set_shown(bool shown) {
    ((void (__thiscall*)(igo_3d_widget*, bool))retail::sub_6AE420)(this, shown);
}

// slot 6
void igo_3d_widget::set_position(f32 x, f32 y, f32 z) {
    ((void (__thiscall*)(igo_3d_widget*, f32, f32, f32))retail::sub_6B1C30)(this, x, y, z);
}

// slot 7
void igo_3d_widget::set_center_pos(f32 x, f32 y) {
    ((void (__thiscall*)(igo_3d_widget*, f32, f32))retail::sub_6B1C00)(this, x, y);
}

// slot 8
void igo_3d_widget::get_position(f32* x, f32* y) {
    ((void (__thiscall*)(igo_3d_widget*, f32*, f32*))retail::sub_689890)(this, x, y);
}

// slot 9
void igo_3d_widget::set_position_z(f32 z) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_6B1C70)(this, z);
}

// slot 10
void igo_3d_widget::set_rotation_angle(f32 angle) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_6B1C90)(this, angle);
}

// slot 11
void igo_3d_widget::set_tilt_angle(f32 angle) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_6B1CB0)(this, angle);
}

// slot 12
void igo_3d_widget::set_spin_angle(f32 angle) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_6B1CD0)(this, angle);
}

// slot 13
void igo_3d_widget::set_scale(f32 scale) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_68FAD0)(this, scale);
}

// slot 14
void igo_3d_widget::set_scale_offset(f32 x, f32 y, f32 z) {
    ((void (__thiscall*)(igo_3d_widget*, f32, f32, f32))retail::sub_6898B0)(this, x, y, z);
}

// slot 15
void igo_3d_widget::set_alpha(f32 value) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_6898E0)(this, value);
}

// slot 16
void igo_3d_widget::set_widescreen_alignment(igo_3d_widget_parameters::e_wide_align_flags alignment) {
    ((void (__thiscall*)(igo_3d_widget*, igo_3d_widget_parameters::e_wide_align_flags))retail::sub_6B1CF0)(this, alignment);
}

// slot 17
void igo_3d_widget::set_suppression_mask(u32 mask) {
    ((void (__thiscall*)(igo_3d_widget*, u32))retail::sub_6898F0)(this, mask);
}

// slot 18
void igo_3d_widget::set_color(f32 r, f32 g, f32 b) {
    ((void (__thiscall*)(igo_3d_widget*, f32, f32, f32))retail::sub_689900)(this, r, g, b);
}

// slot 19
void igo_3d_widget::set_uv_offset(f32 u, f32 v) {
    ((void (__thiscall*)(igo_3d_widget*, f32, f32))retail::sub_689980)(this, u, v);
}

// slot 20
void igo_3d_widget::unk_020(u32 value) {
    ((void (__thiscall*)(igo_3d_widget*, u32))retail::sub_5B00D0)(this, value);
}

// slot 21
void igo_3d_widget::unk_021(u32 value) {
    ((void (__thiscall*)(igo_3d_widget*, u32))retail::sub_4F8A30)(this, value);
}

// slot 22
f32 igo_3d_widget::get_position_x() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_687260)(this);
}

// slot 23
f32 igo_3d_widget::get_position_y() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_687270)(this);
}

// slot 24
f32 igo_3d_widget::get_position_z() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_687280)(this);
}

// slot 25
f32 igo_3d_widget::get_rotation_angle() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_6F8580)(this);
}

// slot 26
f32 igo_3d_widget::get_tilt_angle() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_6F8590)(this);
}

// slot 27
f32 igo_3d_widget::get_spin_angle() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_6F85A0)(this);
}

// slot 28
f32 igo_3d_widget::get_scale() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_687290)(this);
}

// slot 29
f32 igo_3d_widget::get_alpha() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_90D180)(this);
}

// slot 30
bool igo_3d_widget::unk_030() {
    return ((bool (__thiscall*)(igo_3d_widget*))retail::sub_6F85B0)(this);
}

// slot 31
igo_3d_widget_parameters::e_wide_align_flags igo_3d_widget::get_widescreen_alignment() {
    return ((igo_3d_widget_parameters::e_wide_align_flags (__thiscall*)(igo_3d_widget*))retail::sub_6F85C0)(this);
}

// slot 32
u32 igo_3d_widget::suppression_mask() {
    return ((u32 (__thiscall*)(igo_3d_widget*))retail::sub_6F85D0)(this);
}

// slot 33
i32 igo_3d_widget::unk_033() {
    return ((i32 (__thiscall*)(igo_3d_widget*))retail::sub_699B40)(this);
}

// slot 34
i32 igo_3d_widget::unk_034() {
    return ((i32 (__thiscall*)(igo_3d_widget*))retail::sub_699BC0)(this);
}

// slot 35
void igo_3d_widget::set_scale_factor(f32 factor) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_6F6750)(this, factor);
}

// slot 36
f32 igo_3d_widget::get_scale_factor() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_6F85E0)(this);
}

// slot 37
void igo_3d_widget::set_alpha_factor(f32 factor) {
    ((void (__thiscall*)(igo_3d_widget*, f32))retail::sub_6F85F0)(this, factor);
}

// slot 38
f32 igo_3d_widget::get_alpha_factor() {
    return ((f32 (__thiscall*)(igo_3d_widget*))retail::sub_6F6760)(this);
}

// slot 39
void igo_3d_widget::play_sound(string_hash sound) {
    ((void (__thiscall*)(igo_3d_widget*, string_hash))retail::sub_68AC20)(this, sound);
}

// slot 40
void igo_3d_widget::start_timed_move(f32 duration, f32 from_x, f32 from_y, f32 from_z, f32 to_x, f32 to_y, f32 to_z, i32 curve) {
    ((void (__thiscall*)(igo_3d_widget*, f32, f32, f32, f32, f32, f32, f32, i32))retail::sub_68AC60)(this,
                                                                                                     duration,
                                                                                                     from_x,
                                                                                                     from_y,
                                                                                                     from_z,
                                                                                                     to_x,
                                                                                                     to_y,
                                                                                                     to_z,
                                                                                                     curve);
}

// slot 41
void igo_3d_widget::render_entity() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_6D9550)(this);
}

// slot 42
void igo_3d_widget::spawn_entity() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_6B1DB0)(this);
}

// slot 43
i32 igo_3d_widget::unk_043(i32 unk_0, void* unk_1, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, void*, i32, i32))retail::sub_689A40)(this, unk_0, unk_1, unk_2, unk_3);
}

// slot 44
i32 igo_3d_widget::unk_044(i32 unk_0, i32 unk_1, void* unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, void*, i32, i32, i32))retail::sub_689AD0)(this,
                                                                                                   unk_0,
                                                                                                   unk_1,
                                                                                                   unk_2,
                                                                                                   unk_3,
                                                                                                   unk_4,
                                                                                                   unk_5);
}

// slot 45
i32 igo_3d_widget::unk_045(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, i32, i32))retail::sub_68A130)(this, unk_0, unk_1, unk_2, unk_3);
}

// slot 46
i32 igo_3d_widget::unk_046(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, i32, i32, i32))retail::sub_68A1C0)(this, unk_0, unk_1, unk_2, unk_3, unk_4);
}

// slot 47
i32 igo_3d_widget::unk_047(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, i32, i32, i32))retail::sub_68A1E0)(this, unk_0, unk_1, unk_2, unk_3, unk_4);
}

// slot 48
i32 igo_3d_widget::unk_048(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4, i32 unk_5) {
    return ((i32 (__thiscall*)(igo_3d_widget*, i32, i32, i32, i32, i32, i32))retail::sub_6B1E40)(this,
                                                                                                 unk_0,
                                                                                                 unk_1,
                                                                                                 unk_2,
                                                                                                 unk_3,
                                                                                                 unk_4,
                                                                                                 unk_5);
}

// slot 49
void igo_3d_widget::unk_049(i32 unk_0, i32 unk_1, i32 unk_2, i32 unk_3, i32 unk_4) {
    ((void (__thiscall*)(igo_3d_widget*, i32, i32, i32, i32, i32))retail::sub_6997E0)(this, unk_0, unk_1, unk_2, unk_3, unk_4);
}

// slot 50
i32 igo_3d_widget::unk_050() {
    return ((i32 (__thiscall*)(igo_3d_widget*))retail::sub_70B160)(this);
}

// slot 51
void* igo_3d_widget::unk_051() {
    return ((void* (__thiscall*)(igo_3d_widget*))retail::sub_700FC0)(this);
}

// slot 52
void igo_3d_widget::unk_052() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_5B4A70)(this);
}

// slot 53
void igo_3d_widget::unk_053() {
    ((void (__thiscall*)(igo_3d_widget*))retail::sub_5B4A70)(this);
}


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
void igo_3d_widget::reset_entity_po() {
    if (arch_base* ent = entity_vh.resolve())
        retail::sub_626BE0((i32)ent, (u64*)&trans_matrix); // entity_base::set_rel_po
}

// sub_6899A0
// inlined @ sub_6B2700
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
