#include "retail.hh"
#include "game/arch_base.hh"
#include "game/mission/mission_progression_data.hh"
#include "game/script/chuck_lib_mission_objective.hh"

using namespace treyarch;

bool slf__create_stage_data__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8773A0)(this, stack);
}

bool slf__has_stage_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8773E0)(this, stack);
}

bool slf__retrieve_stage_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877420)(this, stack);
}

bool slf__get_mission_script_retry_count__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_842540)(this, stack);
}

bool slf__has_global_objective_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877480)(this, stack);
}

bool slf__create_global_objective_data__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8774C0)(this, stack);
}

bool slf__retrieve_global_objective_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877560)(this, stack);
}

bool slf__set_mission_name__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8775A0)(this, stack);
}

bool slf__set_mission_name_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877650)(this, stack);
}

bool slf__set_mission_description__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8776D0)(this, stack);
}

bool slf__set_mission_description_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877780)(this, stack);
}

bool slf__set_mission_success_string__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877800)(this, stack);
}

bool slf__set_mission_success_string_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8778B0)(this, stack);
}

bool slf__set_mission_failure_string__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877930)(this, stack);
}

bool slf__set_mission_failure_string_debug__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8779E0)(this, stack);
}

bool slf__register_mission_objective__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877A60)(this, stack);
}

bool slf__register_mission_objective__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877AF0)(this, stack);
}

bool slf__unregister_mission_objective__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877BC0)(this, stack);
}

bool slf__mission_objective_is_registered__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877C40)(this, stack);
}

bool slf__mission_objective_actual__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877CC0)(this, stack);
}

bool slf__mission_objective_is_complete__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877D40)(this, stack);
}

bool slf__set_mission_objective_actual__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877DC0)(this, stack);
}

bool slf__set_mission_objective_complete__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877E50)(this, stack);
}

bool slf__set_mission_objective_complete__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_877F00)(this, stack);
}

// sub_876970
bool slf__mission_objective_data__create_child_data__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self          = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto required      = stack.local_num(0x04);
    auto autoactivated = stack.local_num(0x08);
    auto autocompleted = stack.local_num(0x0C);
    auto displayable   = stack.local_num(0x10);

    u32 result = 0;

    if (self) {
        auto child = ((arch_base* (__thiscall*)(mission_progression_data*, bool, bool, bool, bool))retail::sub_768140) // mission_objective_data::create_child_data
            (self, required != 0.0f, autoactivated != 0.0f, autocompleted != 0.0f, displayable != 0.0f);

        result = child->my_handle.value;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877070
bool slf__mission_objective_data__retrieve_child_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    u32 result = 0;

    // an unknown id files a null child and faults
    if (self)
        result = ((arch_base* (__thiscall*)(mission_progression_data*, i32))retail::sub_766530)(self, (i32)id)->my_handle.value; // mission_progression_data::get_child_data

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876A30
bool slf__mission_objective_data__restart__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->restart();

    return true;
}

// sub_8770D0
bool slf__mission_objective_data__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877110
bool slf__mission_objective_data__is_failed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_failed) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876A60
bool slf__mission_objective_data__is_required__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_required) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877170
bool slf__mission_objective_data__is_activated__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_activated) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8771D0
bool slf__mission_objective_data__is_completed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_completed) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876AC0
bool slf__mission_objective_data__is_autoactivated__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_autoactivated) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876B20
bool slf__mission_objective_data__is_autocompleted__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_autocompleted) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876B80
bool slf__mission_objective_data__has_child_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    using has_child_data_t = bool (__thiscall*)(mission_progression_data*, i32);

    f32 result = self && ((has_child_data_t)retail::sub_7591B0)(self, (i32)id) ? 1.0f : 0.0f; // mission_progression_data::has_child_data

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876BF0
bool slf__mission_objective_data__get_id__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? (f32)self->id : -1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876C50
bool slf__mission_objective_data__get_actual_score__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? self->actual_score : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876CA0
bool slf__mission_objective_data__get_target_score__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? self->target_score : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877340
bool slf__mission_objective_data__set_failed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->flags |= mission_progression_flag_failed;

    return true;
}

// sub_876CF0
bool slf__mission_objective_data__set_activated__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->flags |= mission_progression_flag_activated;

    // told even without one
    retail::sub_768750(retail::sub_7685B0()); // mission_progression_registry::inst()->notify_state_change

    return true;
}

// sub_876D20
bool slf__mission_objective_data__set_completed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->flags |= mission_progression_flag_completed;

    // told even without one
    retail::sub_768750(retail::sub_7685B0()); // mission_progression_registry::inst()->notify_state_change

    return true;
}

// sub_876D50
bool slf__mission_objective_data__set_required__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->set_required(on != 0.0f);

    return true;
}

// sub_876DA0
bool slf__mission_objective_data__set_displayable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->set_displayable(on != 0.0f);

    // told even without one
    retail::sub_768750(retail::sub_7685B0()); // mission_progression_registry::inst()->notify_state_change

    return true;
}

// sub_876DF0
bool slf__mission_objective_data__set_autoactivated__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->set_autoactivated(on != 0.0f);

    return true;
}

// sub_876E40
bool slf__mission_objective_data__set_autocompleted__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->set_autocompleted(on != 0.0f);

    return true;
}

// sub_876E90
bool slf__mission_objective_data__set_description__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self)
        self->set_description((i32)value);

    return true;
}

// sub_876EC0
bool slf__mission_objective_data__set_short_description__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self)
        self->set_short_description((i32)value);

    return true;
}

// sub_876EF0
bool slf__mission_objective_data__set_actual_score__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self)
        self->set_actual_score(value);

    return true;
}

// sub_876F20
bool slf__mission_objective_data__init_tracking_scores__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto start  = stack.local_num(0x04);
    auto target = stack.local_num(0x08);

    if (self)
        self->init_tracking_scores(start, target);

    return true;
}

// sub_876F60
bool slf__mission_objective_data__set_awarded_points__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self     = (mission_objective_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto awarded  = stack.local_num(0x04);
    auto unk_b    = stack.local_num(0x08);
    auto unk_c    = stack.local_num(0x0C);

    if (self)
        self->set_awarded_points(awarded, unk_b, unk_c != 0.0f);

    return true;
}

// sub_876FB0
bool slf__mission_stage_data__create_objective_data__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self          = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto required      = stack.local_num(0x04);
    auto autoactivated = stack.local_num(0x08);
    auto autocompleted = stack.local_num(0x0C);
    auto displayable   = stack.local_num(0x10);

    u32 result = 0;

    if (self) {
        auto child = ((arch_base* (__thiscall*)(mission_progression_data*, bool, bool, bool, bool))retail::sub_7682D0) // mission_stage_data::create_objective_data
            (self, required != 0.0f, autoactivated != 0.0f, autocompleted != 0.0f, displayable != 0.0f);

        result = child->my_handle.value;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877070
bool slf__mission_stage_data__retrieve_objective_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    u32 result = 0;

    // an unknown id files a null child and faults
    if (self)
        result = ((arch_base* (__thiscall*)(mission_progression_data*, i32))retail::sub_766530)(self, (i32)id)->my_handle.value; // mission_progression_data::get_child_data

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876A30
bool slf__mission_stage_data__restart__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->restart();

    return true;
}

// sub_8770D0
bool slf__mission_stage_data__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877110
bool slf__mission_stage_data__is_failed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_failed) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877170
bool slf__mission_stage_data__is_activated__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_activated) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8771D0
bool slf__mission_stage_data__is_completed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_flagged(mission_progression_flag_completed) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876B80
bool slf__mission_stage_data__has_objective_data__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    using has_child_data_t = bool (__thiscall*)(mission_progression_data*, i32);

    f32 result = self && ((has_child_data_t)retail::sub_7591B0)(self, (i32)id) ? 1.0f : 0.0f; // mission_progression_data::has_child_data

    stack.push(&result, sizeof(result));

    return true;
}

// sub_876BF0
bool slf__mission_stage_data__get_id__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? (f32)self->id : -1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877230
bool slf__mission_stage_data__get_restart_position__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_stage_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->restart_position : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8772A0
bool slf__mission_stage_data__get_facing_direction__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_stage_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->facing_direction : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_877310
bool slf__mission_stage_data__set_restart_data__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self     = (mission_stage_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto position = (const vector3d*)&stack.local_uint(0x04);
    auto facing   = (const vector3d*)&stack.local_uint(0x10);

    if (self)
        self->set_restart_data(position, facing);

    return true;
}

// sub_877340
bool slf__mission_stage_data__set_failed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_progression_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->flags |= mission_progression_flag_failed;

    return true;
}

// sub_877360
bool slf__mission_stage_data__set_activated__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_stage_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->flags |= mission_progression_flag_activated;

    return true;
}

// sub_877380
bool slf__mission_stage_data__set_completed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (mission_stage_data*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        self->flags |= mission_progression_flag_completed;

    return true;
}
