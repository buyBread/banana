#include "retail.hh"
#include "game/arch_base_vhandle.hh"
#include "game/math/references.hh"
#include "game/script/chuck_lib_ai.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/cpu_combat_inode.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"

using namespace treyarch;

bool slf__get_camera_y_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83F9A0)(this, stack);
}

bool slf__get_camera_z_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842000)(this, stack);
}

bool slf__create_generic_event_callback__arch_base__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867E00)(this, stack);
}

bool slf__get_best_surface_pos__vector3d__num__line_info__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867FB0)(this, stack);
}

bool slf__random_attack_set___frame_advance__random_attack_set__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868E80)(this, stack);
}

bool slf__random_attack_set___frame_advance_array__dynamic_array__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868EA0)(this, stack);
}

bool slf__random_attack_set___request_attack_index__random_attack_set__dynamic_array__dynamic_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868F40)(this, stack);
}

// sub_866A90
bool slf__entity__enable_tether__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto center = (const vector3d*)&stack.local_uint(0x04);
    auto radius = stack.local_num(0x10);

    if (self && self->is_an_actor()) {
        if (ai_core* core = ((actor*)self)->get_ai_core())
            core->enable_tether(*center, radius);
    }

    return true;
}

// sub_866AE0
bool slf__entity__disable_tether__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_actor()) {
        if (ai_core* core = ((actor*)self)->get_ai_core())
            core->disable_tether();
    }

    return true;
}

// sub_866B20
bool slf__entity__get_tether_center__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = references::unk_010f7140.get();

    if (self) {
        result = self->my_abs_po->get_position();

        if (self->is_an_actor()) {
            ai_core* core = ((actor*)self)->get_ai_core();

            if (core && core->is_tether_active())
                result = core->get_tether_center();
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_866C00
bool slf__entity__get_tether_radius__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_actor()) {
        ai_core* core = ((actor*)self)->get_ai_core();

        if (core && core->is_tether_active())
            result = core->get_tether_radius();
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_866C70
bool slf__entity__is_tether_active__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_actor()) {
        ai_core* core = ((actor*)self)->get_ai_core();

        if (core && core->is_tether_active())
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_866CE0
bool slf__cpu_combat_movement__get_tether_center__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);

    vector3d result;

    if (self)
        result = self->core->get_tether_center();
    else
        result = references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_866D50
bool slf__cpu_combat_movement__get_tether_radius__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);

    f32 result = self ? self->core->get_tether_radius() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_866DA0
bool slf__cpu_combat_movement__is_tether_active__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);

    f32 result = self && self->core->is_tether_active() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_866E00
bool slf__cpu_combat_movement__clip_to_tether__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);
    auto pos  = (const vector3d*)&stack.local_uint(0x04);

    vector3d result;

    if (self)
        result = self->core->clip_to_tether(pos, nullptr);
    else
        result = references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__cpu_combat_movement__get_pie_target__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866E70)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866EC0)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_normal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866F30)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_facing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_866FA0)(this, stack);
}

bool slf__cpu_combat_movement__get_new_pie_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867010)(this, stack);
}

bool slf__cpu_combat_movement__pos_inside_pie_slice__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867090)(this, stack);
}

bool slf__cpu_combat_movement__inside_pie_slice__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867100)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_min_dist__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8671A0)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_max_dist__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8671F0)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_ring__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867240)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_num_rings__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867290)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_slice__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8672F0)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_num_slices__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867340)(this, stack);
}

bool slf__cpu_combat_movement__get_pie_buffer_arc_pct__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8673A0)(this, stack);
}

// sub_8673F0
bool slf__cpu_combat_movement__is_moving__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);

    f32 result = self && self->core->is_moving() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_867430
bool slf__cpu_combat_movement__get_movement_dest__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);

    vector3d result;

    if (self)
        result = self->core->get_movement_dest();
    else
        result = references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8674A0
bool slf__cpu_combat_movement__strafe_to_position__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self, the vector3d once
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 3));

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);
    auto pos  = (const vector3d*)&stack.local_uint(0x04);

    if (!self)
        return true;

    i32 optional_count = count - 3;

    bool clip             = optional_count <= 0 || stack.local_num(0x18) > 0.5f;
    bool allow_nonpathed  = optional_count > 1  && stack.local_num(0x1C) > 0.5f;
    bool force_non_pathed = optional_count > 2  && stack.local_num(0x20) > 0.5f;
    bool unk              = optional_count > 3  && stack.local_num(0x24) > 0.5f;

    vector3d clipped;

    if (clip) {
        clipped = self->core->clip_to_tether(pos, nullptr);
        pos     = &clipped;
    }

    auto goto_position = (bool (__thiscall*)(ai_core*, const vector3d*, f32, f32, bool, bool, bool, bool))retail::sub_4CC850; // ai_core::goto_position

    goto_position(self->core,
                  pos,
                  stack.local_num(0x10),
                  stack.local_num(0x14),
                  true,
                  allow_nonpathed,
                  force_non_pathed,
                  unk);

    return true;
}

// sub_8675B0
bool slf__cpu_combat_movement__goto_position__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self, the vector3d once
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 3));

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);
    auto pos  = (const vector3d*)&stack.local_uint(0x04);

    if (!self)
        return true;

    i32 optional_count = count - 3;

    bool clip             = optional_count <= 0 || stack.local_num(0x18) > 0.5f;
    bool allow_nonpathed  = optional_count > 1  && stack.local_num(0x1C) > 0.5f;
    bool force_non_pathed = optional_count > 2  && stack.local_num(0x20) > 0.5f;
    bool unk              = optional_count > 3  && stack.local_num(0x24) > 0.5f;

    vector3d clipped;

    if (clip) {
        clipped = self->core->clip_to_tether(pos, nullptr);
        pos     = &clipped;
    }

    auto goto_position = (bool (__thiscall*)(ai_core*, const vector3d*, f32, f32, bool, bool, bool, bool))retail::sub_4CC850; // ai_core::goto_position

    goto_position(self->core,
                  pos,
                  stack.local_num(0x10),
                  stack.local_num(0x14),
                  false,
                  allow_nonpathed,
                  force_non_pathed,
                  unk);

    return true;
}

// sub_8676C0
bool slf__cpu_combat_movement__stop_movement__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (cpu_combat_movement*)stack.local_uint(0x00);

    if (self)
        self->core->stop_movement();

    return true;
}

// sub_8676E0
bool slf__cpu_combat_movement__set_facing_point__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self  = (cpu_combat_movement*)stack.local_uint(0x00);
    auto point = (const vector3d*)&stack.local_uint(0x04);

    if (self)
        self->core->set_facing_point(*point, 0.0f);

    return true;
}

bool slf__cpu_combat_movement__set_facing_pie_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867710)(this, stack);
}

bool slf__cpu_combat_movement__rappel_allowed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867750)(this, stack);
}

bool slf__cpu_combat_movement__rappel_is_firing__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8677E0)(this, stack);
}

bool slf__cpu_combat_movement__rappel_is_engaged__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867880)(this, stack);
}

bool slf__cpu_combat_movement__rappel_fire__vector3d__entity__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867920)(this, stack);
}

bool slf__cpu_combat_movement__rappel_attach__vector3d__entity__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867A20)(this, stack);
}

bool slf__cpu_combat_movement__rappel_disengage__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867AF0)(this, stack);
}

bool slf__cpu_combat_movement__rappel_set_speed__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867B50)(this, stack);
}

bool slf__cpu_combat_movement__rappel_set_rope_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867BA0)(this, stack);
}

bool slf__cpu_combat_movement__rappel_get_constraint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867C00)(this, stack);
}

bool slf__cpu_combat_movement__rappel_get_speed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867CB0)(this, stack);
}

bool slf__cpu_combat_movement__rappel_set_rope_pivot__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867D40)(this, stack);
}

bool slf__generic_event_callback__add__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867E90)(this, stack);
}

bool slf__generic_event_callback__remove__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867EE0)(this, stack);
}

bool slf__generic_event_callback__clear__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867F30)(this, stack);
}

bool slf__generic_event_callback__raised__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_867F50)(this, stack);
}

bool slf__entity__ai_puppet_lock_access__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868030)(this, stack);
}

bool slf__entity__ai_puppet_unlock_access__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8680A0)(this, stack);
}

bool slf__entity__ai_set_mutation_scale__str__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868110)(this, stack);
}

bool slf__entity__ai_set_mutation_scale_anim__str__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868280)(this, stack);
}

bool slf__entity__ai_stop_mutation_scale_anim__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8683F0)(this, stack);
}

bool slf__entity__ai_get_mutation_scale__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868490)(this, stack);
}

bool slf__entity__ai_set_mutation_trans__str__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868580)(this, stack);
}

bool slf__entity__ai_set_mutation_trans_anim__str__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868720)(this, stack);
}

bool slf__entity__ai_stop_mutation_trans_anim__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8688A0)(this, stack);
}

bool slf__entity__ai_get_mutation_current_trans__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868940)(this, stack);
}

bool slf__entity__ai_set_mutation_trans_abs_position__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868A30)(this, stack);
}

bool slf__entity__ai_is_mutation_trans_affect_children__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868AF0)(this, stack);
}

bool slf__entity__push_ai_disable__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868BB0)(this, stack);
}

bool slf__entity__pop_ai_disable__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_868C10)(this, stack);
}
