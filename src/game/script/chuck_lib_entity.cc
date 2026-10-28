#include <cstring>

#include "retail.hh"
#include "game/als/als.hh"
#include "game/als/param.hh"
#include "game/arch_base.hh"
#include "game/math/references.hh"
#include "game/script/chuck_lib_entity.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_vehicle.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/ai/param_block.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/vehicle.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"

using namespace treyarch;

// sub_8239C0
string_hash treyarch::get_ai_param_hash_and_core(arch_base_vhandle handle, const char* param_name, ai_core** core) {
    arch_base* entity = handle.resolve();

    *core = entity ? ((actor*)entity)->get_ai_core() : nullptr;

    string_hash parameter;
    parameter.initialize(mash::ALLOCATED, param_name);

    return parameter;
}

// sub_83DFC0
string_hash treyarch::get_ai_param_hash_and_inode(arch_base_vhandle  handle,
                                                  const char*        param_name,
                                                  const char*        inode_name,
                                                  info_node**        node) {

    arch_base* entity = handle.resolve();
    ai_core*   core   = entity ? ((actor*)entity)->get_ai_core() : nullptr;

    string_hash parameter;
    parameter.initialize(mash::ALLOCATED, param_name);

    if (!core) {
        *node = nullptr;

        return parameter;
    }

    string_hash type_name;
    type_name.initialize(mash::ALLOCATED, inode_name);

    *node = core->get_info_node(get_info_node_type(type_name));

    return parameter;
}

// inlined @ sub_84C540
// inlined @ sub_84C620
// inlined @ sub_84C740
// inlined @ sub_84C840
// inlined @ sub_84C910
// inlined @ sub_84C9F0
// inlined @ sub_84CAD0
// inlined @ sub_84CBD0
// inlined @ sub_84CCD0
// inlined @ sub_84CDC0
// inlined @ sub_84CEA0
// inlined @ sub_84CF60
// inlined @ sub_84D0D0
// inlined @ sub_84D1E0
// inlined @ sub_84D3C0
// inlined @ sub_84D470
// inlined @ sub_84D520
// inlined @ sub_84D600
// inlined @ sub_84D6B0
// inlined @ sub_84D760
// inlined @ sub_84D830
// inlined @ sub_84D900
param_block* treyarch::get_ai_param_block(arch_base_vhandle  handle,
                                          const char*        param_name,
                                          string_hash       &parameter,
                                          bool*              resolved) {

    arch_base* entity = handle.resolve();

    if (resolved)
        *resolved = entity != nullptr;

    if (!entity)
        return nullptr;

    // a vehicle's are its ai's
    if (entity->is_a_vehicle()) {
        parameter.initialize(mash::ALLOCATED, param_name);

        return &((vehicle*)entity)->my_ai->my_inode->my_param_block;
    }

    ai_core* core;
    parameter = get_ai_param_hash_and_core(handle, param_name, &core);

    return core ? &core->my_param_block : nullptr;
}

// sub_83DAE0
u32 slc_entity_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    if (!entity)
        return 0;

    return entity->my_handle.value;
}

bool slf__hero__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83DB50)(this, stack);
}

bool slf__chase_cam__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83DBA0)(this, stack);
}

bool slf__create_devopt_entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84AA40)(this, stack);
}

bool slf__create_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84AEA0)(this, stack);
}

bool slf__create_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B040)(this, stack);
}

bool slf__create_preregistered_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B160)(this, stack);
}

bool slf__create_preregistered_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B300)(this, stack);
}

bool slf__create_next_preregistered_debug_obstacle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_83DDF0)(this, stack);
}

bool slf__find_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B420)(this, stack);
}

bool slf__find_entity__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B490)(this, stack);
}

bool slf__entity_exists__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B4F0)(this, stack);
}

bool slf__destroy_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B560)(this, stack);
}

bool slf__entity_has_entity_tracker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B8C0)(this, stack);
}

bool slf__entity_get_entity_tracker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B910)(this, stack);
}

bool slf__enable_entity_fading__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BBA0)(this, stack);
}

bool slf__script_get_entities_of_type__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880970)(this, stack);
}

bool slf__script_get_sorted_entities_of_type__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880AB0)(this, stack);
}

bool slf__crawl_attach_to_surface__entity__line_info__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850550)(this, stack);
}

// sub_843680
bool slf__entity__rvb_set_needs_repathfind__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        ((void (__cdecl*)(arch_base*))retail::sub_4C74D0)(self); // actor::rvb_set_needs_repathfind;

    return true;
}

// sub_8436A0
bool slf__entity__rvb_snapshot_behavior__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        ((void (__cdecl*)(arch_base*))retail::sub_4CB150)(self); // rvb_snapshot_behavior;

    return true;
}

// sub_8436C0
bool slf__entity__rvb_reset_behavior__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        rvb_reset_behavior(self);

    return true;
}

// sub_8436E0
bool slf__entity__rvb_restart_timer__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    if (!self)
        return true;

    if (ai_core* core = ((actor*)self)->get_ai_core()) {
        string_hash timer;
        timer.initialize(mash::ALLOCATED, name);

        ((void (__thiscall*)(ai_core*, const string_hash*, bool))retail::sub_4DF260)(core, &timer, true); // ai_core::restart_timer
    }

    return true;
}

// sub_843730
bool slf__entity__rvb_set_target__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slot   = stack.local_num(0x04);
    auto target = arch_base_vhandle(stack.local_uint(0x08));

    // the target resolves once for the test and again for the call
    if (self && target.resolve())
        ((void (__cdecl*)(arch_base*, i32, arch_base*))retail::sub_4CB190)(self, (i32)slot, target.resolve()); // rvb_set_target

    return true;
}

// sub_843780
bool slf__entity__rvb_set_target__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self     = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slot     = stack.local_num(0x04);
    auto position = (const vector3d*)&stack.local_uint(0x08);

    if (self)
        ((void (__cdecl*)(arch_base*, i32, vector3d))retail::sub_4CB1C0)(self, (i32)slot, *position); // rvb_set_target

    return true;
}

// sub_8437C0
bool slf__entity__rvb_has_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slot = stack.local_num(0x04);

    // the target's type, 0 without one
    f32 result = self ? (f32)((i32 (__cdecl*)(arch_base*, i32))retail::sub_4CB220)(self, (i32)slot) : 0.0f; // rvb_has_target

    stack.push(&result, sizeof(result));

    return true;
}

// sub_843830
bool slf__entity__rvb_get_target_entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slot = stack.local_num(0x04);

    arch_base_vhandle result;

    if (self)
        ((arch_base_vhandle* (__cdecl*)(arch_base_vhandle*, arch_base*, i32))retail::sub_4CB250)(&result, self, (i32)slot); // rvb_get_target_entity

    stack.push(&result, sizeof(result));

    return true;
}

// sub_843890
bool slf__entity__rvb_get_target_vector__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slot = stack.local_num(0x04);

    vector3d result;

    if (self)
        ((vector3d* (__cdecl*)(vector3d*, arch_base*, i32))retail::sub_4CB390)(&result, self, (i32)slot); // rvb_get_target_vector
    else
        result = references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_843910
bool slf__entity__rvb_disable_tether__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        ((void (__cdecl*)(arch_base*, i32))retail::sub_4CB200)(self, 5); // rvb_clear_target, on the tether's slot;

    return true;
}

// sub_843940
bool slf__entity__rvb_clear_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slot = stack.local_num(0x04);

    if (self)
        ((void (__cdecl*)(arch_base*, i32))retail::sub_4CB200)(self, (i32)slot); // rvb_clear_target

    return true;
}

// sub_843970
bool slf__entity__rvb_set_machine__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self    = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slot    = stack.local_num(0x04);
    auto machine = (const char*)stack.local_uint(0x08);

    if (self) {
        mash::string name(machine);

        ((void (__cdecl*)(arch_base*, u32, const char*))retail::sub_4CB420)(self, (u32)(i32)slot, name.c_str()); // rvb_set_machine
    }

    return true;
}

// sub_843A00
bool slf__entity__rvb_clear_machine__num__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self    = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slot    = stack.local_num(0x04);
    auto machine = (const char*)stack.local_uint(0x08);

    if (self) {
        // built and dropped unread
        mash::string name(machine);

        ((void (__cdecl*)(arch_base*, u32))retail::sub_4CB460)(self, (u32)(i32)slot); // rvb_clear_machine
    }

    return true;
}

// sub_843A80
bool slf__entity__rvb_add_patrol_waypoint__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto point = (const vector3d*)&stack.local_uint(0x04);

    if (self)
        ((void (__cdecl*)(arch_base*, vector3d, f32, i32))retail::sub_4CB4A0)(self, *point, -1.0f, 0); // rvb_add_patrol_waypoint

    return true;
}

// sub_843AD0
bool slf__entity__rvb_add_patrol_waypoint__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self   = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto point = (const vector3d*)&stack.local_uint(0x04);
    auto wait  = stack.local_num(0x10);

    if (self)
        ((void (__cdecl*)(arch_base*, vector3d, f32, i32))retail::sub_4CB4A0)(self, *point, wait, 0); // rvb_add_patrol_waypoint

    return true;
}

// sub_843B20
bool slf__entity__rvb_add_patrol_waypoint__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self   = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto point = (const vector3d*)&stack.local_uint(0x04);
    auto wait  = stack.local_num(0x10);
    auto unk   = stack.local_num(0x14);

    if (self)
        ((void (__cdecl*)(arch_base*, vector3d, f32, i32))retail::sub_4CB4A0)(self, *point, wait, (i32)unk); // rvb_add_patrol_waypoint

    return true;
}

// sub_843B70
bool slf__entity__rvb_add_patrol_waypoint__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto waypoint = arch_base_vhandle(stack.local_uint(0x04)).resolve();

    if (self)
        ((void (__cdecl*)(arch_base*, arch_base*, f32, i32))retail::sub_4CB5A0)(self, waypoint, -1.0f, 0); // rvb_add_patrol_waypoint

    return true;
}

// sub_843BB0
bool slf__entity__rvb_add_patrol_waypoint__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self      = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto waypoint = arch_base_vhandle(stack.local_uint(0x04)).resolve();
    auto wait     = stack.local_num(0x08);

    if (self)
        ((void (__cdecl*)(arch_base*, arch_base*, f32, i32))retail::sub_4CB5A0)(self, waypoint, wait, 0); // rvb_add_patrol_waypoint

    return true;
}

// sub_843BF0
bool slf__entity__rvb_add_patrol_waypoint__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self      = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto waypoint = arch_base_vhandle(stack.local_uint(0x04)).resolve();
    auto wait     = stack.local_num(0x08);
    auto unk      = stack.local_num(0x0C);

    if (self)
        ((void (__cdecl*)(arch_base*, arch_base*, f32, i32))retail::sub_4CB5A0)(self, waypoint, wait, (i32)unk); // rvb_add_patrol_waypoint

    return true;
}

// sub_843C30
bool slf__entity__rvb_clear_patrol_waypoints__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        ((void (__cdecl*)(arch_base*))retail::sub_4CB6A0)(self); // rvb_clear_patrol_waypoints;

    return true;
}

// sub_843C50
bool slf__entity__rvb_get_patrol_waypoint_count__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self ? (f32)((i32 (__cdecl*)(arch_base*))retail::sub_4CB710)(self) : -1.0f; // rvb_get_patrol_waypoint_count

    stack.push(&result, sizeof(result));

    return true;
}

// sub_843CB0
bool slf__entity__rvb_set_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    if (!self)
        return true;

    string_hash flag;
    flag.initialize(mash::ALLOCATED, name);

    rvb_set_flag(self, &flag, true);

    return true;
}

// sub_843CF0
bool slf__entity__rvb_get_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self) {
        string_hash flag;
        flag.initialize(mash::ALLOCATED, name);

        result = rvb_get_flag(self, &flag) ? 1.0f : 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_843D70
bool slf__entity__rvb_clear_flag__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    if (!self)
        return true;

    string_hash flag;
    flag.initialize(mash::ALLOCATED, name);

    rvb_set_flag(self, &flag, false);

    return true;
}

// sub_843DB0
bool slf__entity__rvb_set_var__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = stack.local_num(0x08);

    if (!self)
        return true;

    string_hash var;
    var.initialize(mash::ALLOCATED, name);

    rvb_set_var(self, &var, value);

    return true;
}

// sub_843E00
bool slf__entity__rvb_get_var__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self) {
        string_hash var;
        var.initialize(mash::ALLOCATED, name);

        result = rvb_get_var(self, &var);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_843E80
bool slf__entity__rvb_set_vector__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = (const vector3d*)&stack.local_uint(0x08);

    if (self) {
        string_hash var;
        var.initialize(mash::ALLOCATED, name);

        ((void (__cdecl*)(arch_base*, const string_hash*, vector3d))retail::sub_4CB800)(self, &var, *value); // rvb_set_vector
    }

    return true;
}

// sub_843EE0
bool slf__entity__rvb_get_vector__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    // built and dropped unread
    mash::string unused(name);

    vector3d result;

    if (self) {
        string_hash var;
        var.initialize(mash::ALLOCATED, name);

        ((vector3d* (__cdecl*)(vector3d*, arch_base*, const string_hash*))retail::sub_4CB840)(&result, self, &var); // rvb_get_vector
    } else
        result = references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_843FB0
bool slf__entity__rvb_set_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = (const char*)stack.local_uint(0x08);

    if (self) {
        string_hash hash;
        hash.initialize(mash::ALLOCATED, value);
        string_hash var;
        var.initialize(mash::ALLOCATED, name);

        ((void (__cdecl*)(arch_base*, const string_hash*, const string_hash*))retail::sub_4CB8A0)(self, &var, &hash); // rvb_set_hash
    }

    return true;
}

// sub_844010
bool slf__entity__is_rvb_character__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && ((ai_basic_inode* (__cdecl*)(arch_base*))retail::sub_4C77B0)(self) ? 1.0f : 0.0f; // get_ai_basic_inode_if_slim_actor

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844070
bool slf__entity__rvb_is_crawling__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && ((bool (__cdecl*)(arch_base*))retail::sub_4C7590)(self) ? 1.0f : 0.0f; // rvb_is_crawling

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8440D0
bool slf__entity__rvb_set_blocked_attack_types__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto types = stack.local_num(0x04);

    if (self)
        ((void (__cdecl*)(arch_base*, i32))retail::sub_4CB8D0)(self, (i32)types); // rvb_set_blocked_attack_types

    return true;
}

// sub_844100
bool slf__entity__rvb_set_dodged_attack_types__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto types = stack.local_num(0x04);

    if (self)
        ((void (__cdecl*)(arch_base*, i32))retail::sub_4CB910)(self, (i32)types); // rvb_set_dodged_attack_types

    return true;
}

// sub_844130
bool slf__entity__rvb_enter_car__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self    = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto car     = arch_base_vhandle(stack.local_uint(0x04));
    auto seat    = stack.local_num(0x08);
    auto animate = stack.local_num(0x0C);

    // the car resolves once for the test and again for the call
    if (self && car.resolve())
        ((void (__cdecl*)(arch_base*, arch_base*, f32, bool))retail::sub_4CBA00)(self, car.resolve(), seat, animate != 0.0f); // rvb_enter_car

    return true;
}

// sub_8441A0
bool slf__entity__rvb_exit_car__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self    = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto animate = stack.local_num(0x04);

    if (self)
        ((void (__cdecl*)(arch_base*, bool))retail::sub_4CBB70)(self, animate != 0.0f); // rvb_exit_car

    return true;
}

// sub_8441F0
bool slf__entity__rvb_enable_tether__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self   = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto center = (const vector3d*)&stack.local_uint(0x04);
    auto radius = stack.local_num(0x10);

    if (self)
        ((void (__cdecl*)(arch_base*, vector3d, f32))retail::sub_4CBC00)(self, *center, radius); // rvb_enable_tether

    return true;
}

// sub_844230
bool slf__entity__rvb_enable_combat_tether__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self   = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto center = (const vector3d*)&stack.local_uint(0x04);
    auto radius = stack.local_num(0x10);

    if (self) {
        ((void (__cdecl*)(arch_base*, vector3d, f32))retail::sub_4CBC00)(self, *center, radius); // rvb_enable_tether

        string_hash flag;
        flag.initialize(mash::ALLOCATED, "tether_maneuver_flag");

        rvb_set_flag(self, &flag, true);
    }

    return true;
}

// sub_8442A0
bool slf__entity__rvb_enable_tether__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = arch_base_vhandle(stack.local_uint(0x04));
    auto radius = stack.local_num(0x08);

    // the target resolves once for the test and again for the call
    if (self && target.resolve())
        ((void (__cdecl*)(arch_base*, arch_base*, f32))retail::sub_4CBC90)(self, target.resolve(), radius); // rvb_enable_tether

    return true;
}

// sub_8442F0
bool slf__entity__rvb_enable_combat_tether__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = arch_base_vhandle(stack.local_uint(0x04));
    auto radius = stack.local_num(0x08);

    // same
    if (self && target.resolve()) {
        ((void (__cdecl*)(arch_base*, arch_base*, f32))retail::sub_4CBC90)(self, target.resolve(), radius); // rvb_enable_tether

        string_hash flag;
        flag.initialize(mash::ALLOCATED, "tether_maneuver_flag");

        rvb_set_flag(self, &flag, true);
    }

    return true;
}

// sub_844360
bool slf__entity__rvb_set_speed_modifier__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto modifier = stack.local_num(0x04);

    if (self)
        ((void (__cdecl*)(arch_base*, f32))retail::sub_4C4B30)(self, modifier); // rvb_set_speed_modifier

    return true;
}

// sub_844390
bool slf__entity__rvb_get_animation_coordinator__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result;

    if (self)
        ((arch_base_vhandle* (__cdecl*)(arch_base_vhandle*, arch_base*))retail::sub_4C77E0)(&result, self); // rvb_get_animation_coordinator

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__get_visual_center__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8443F0)(this, stack);
}

bool slf__entity__get_visual_radius__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844490)(this, stack);
}

// sub_844500
bool slf__entity__get_abs_position__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_abs_po->get_position() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__compute_sector__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844570)(this, stack);
}

// sub_8445C0
bool slf__entity__get_facing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result(0.0f, 0.0f, 0.0f);

    // a particle generator faces along y
    if (self)
        result = self->get_flavor() == entity_flavor_particle_generator ? self->my_abs_po->get_y_facing() : self->my_abs_po->get_z_facing();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844660
bool slf__entity__get_x_facing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_abs_po->get_x_facing() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8446D0
bool slf__entity__get_y_facing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_abs_po->get_y_facing() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844740
bool slf__entity__get_z_facing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_abs_po->get_z_facing() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8447B0
bool slf__entity__get_rel_x_facing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_rel_po->get_x_facing() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844820
bool slf__entity__get_rel_y_facing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_rel_po->get_y_facing() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844890
bool slf__entity__get_rel_z_facing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_rel_po->get_z_facing() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__get_last_anchor__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844900)(this, stack);
}

// sub_8449A0
bool slf__entity__get_rel_position__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_rel_po->get_position() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__set_parent__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844A10)(this, stack);
}

bool slf__entity__set_parent_rel__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844B10)(this, stack);
}

bool slf__entity__get_vehicle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844BA0)(this, stack);
}

// sub_844C40
bool slf__entity__get_parent__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    u32 result = self && self->my_parent ? self->my_parent->my_handle.value : 0;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844C80
bool slf__entity__get_conglom_owner__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    u32 result = 0;

    // the attach root's handle, or the entity's own without one
    if (self) {
        auto root = (entity*)retail::sub_601910((u32*)self); // entity_base::get_attach_root

        result = root && root->my_handle.value ? root->my_handle.value : self->my_handle.value;
    }

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__set_rel_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879C80)(this, stack);
}

bool slf__entity__move_rel_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879D10)(this, stack);
}

bool slf__entity__set_velocity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844CD0)(this, stack);
}

bool slf__entity__set_abs_velocity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844E70)(this, stack);
}

bool slf__entity__look_at__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844EF0)(this, stack);
}

bool slf__entity__set_xz_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_844F70)(this, stack);
}

bool slf__entity__set_facing__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_845230)(this, stack);
}

bool slf__entity__set_abs_xz_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8454B0)(this, stack);
}

bool slf__entity__set_po_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8456D0)(this, stack);
}

bool slf__entity__set_rel_po_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8458B0)(this, stack);
}

bool slf__entity__apply_continuous_rotation__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_845A90)(this, stack);
}

bool slf__entity__wait_rotate__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_845AE0)(this, stack);
}

bool slf__entity__wait_rotate_WCS__vector3d__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_845EF0)(this, stack);
}

bool slf__entity__wait_translate__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8462C0)(this, stack);
}

bool slf__entity__wait_translate_with_compute_sector__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8466A0)(this, stack);
}

bool slf__entity__wait_translate_abs__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_846AA0)(this, stack);
}

bool slf__entity__get_rel_velocity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_846EE0)(this, stack);
}

bool slf__entity__force_region__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8470B0)(this, stack);
}

bool slf__entity__force_current_region__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847140)(this, stack);
}

bool slf__entity__unforce_regions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8471B0)(this, stack);
}

// sub_847230
bool slf__entity__operator_equals_equals__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = stack.local_uint(0x00);
    auto other = stack.local_uint(0x04);

    // the handles, unresolved
    f32 result = self == other ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_847270
bool slf__entity__operator_not_equals__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = stack.local_uint(0x00);
    auto other = stack.local_uint(0x04);

    f32 result = self == other ? 0.0f : 1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__set_damage_locked__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852680)(this, stack);
}

// sub_8472B0
bool slf__entity__is_invulnerable__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && (self->ext_flags & entity_ext_flag_invulnerable) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__is_immune_to_death_from_damage__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847330)(this, stack);
}

bool slf__entity__enable_push_away_force_on_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8473D0)(this, stack);
}

bool slf__entity__enable_die_on_fall_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847430)(this, stack);
}

bool slf__entity__set_invulnerable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8474A0)(this, stack);
}

bool slf__entity__set_targetting__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847570)(this, stack);
}

bool slf__entity__set_beamable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8475E0)(this, stack);
}

bool slf__entity__set_ai_los_block__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847650)(this, stack);
}

bool slf__entity__set_shadow_casting__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8476C0)(this, stack);
}

// sub_847710
bool slf__entity__is_visible__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_visible() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__set_visible__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8477A0)(this, stack);
}

bool slf__entity__set_physical__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847840)(this, stack);
}

bool slf__entity__is_auto_activate_prop__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8478A0)(this, stack);
}

bool slf__entity__set_distance_clip__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847920)(this, stack);
}

bool slf__entity__set_distance_clip_far__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847990)(this, stack);
}

bool slf__entity__is_hanging_from_pole__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847A00)(this, stack);
}

bool slf__entity__enable_pain_sounds__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847AB0)(this, stack);
}

bool slf__entity__apply_web_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847B30)(this, stack);
}

bool slf__entity__apply_web_damage_with_directed_react__num__num__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847C30)(this, stack);
}

bool slf__entity__apply_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847E20)(this, stack);
}

bool slf__entity__apply_damage_with_directed_react__num__num__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_847F00)(this, stack);
}

bool slf__entity__apply_damage_with_react__num__num__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8480F0)(this, stack);
}

bool slf__entity__apply_damage_with_react_and_scales__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_848380)(this, stack);
}

bool slf__entity__apply_damage_with_react_at_pos__num__num__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_848800)(this, stack);
}

bool slf__entity__apply_damage_with_react_and_scales_at_pos__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_848A10)(this, stack);
}

bool slf__entity__apply_damage_with_react_and_scales_at_pos_no_physics__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_848E20)(this, stack);
}

bool slf__entity__apply_melee_damage__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849230)(this, stack);
}

bool slf__entity__apply_explosive_damage__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849440)(this, stack);
}

bool slf__entity__attach_to_wall__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849710)(this, stack);
}

bool slf__entity__attach_to_wall__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849820)(this, stack);
}

bool slf__entity__apply_explosive_damage__num__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8498D0)(this, stack);
}

bool slf__entity__get_last_entity_that_damaged_me__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849B90)(this, stack);
}

bool slf__entity__set_damage_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849C00)(this, stack);
}

bool slf__entity__set_damage_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849C50)(this, stack);
}

bool slf__entity__get_damage_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849C90)(this, stack);
}

bool slf__entity__was_occluded_last_frame__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849D10)(this, stack);
}

bool slf__entity__snap_to__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849DB0)(this, stack);
}

bool slf__entity__abs_snap_to__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_849F90)(this, stack);
}

bool slf__entity__camera_set_target__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A170)(this, stack);
}

bool slf__entity__camera_get_target__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A1E0)(this, stack);
}

bool slf__entity__camera_set_roll__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A290)(this, stack);
}

bool slf__entity__wait_camera_set_roll__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A300)(this, stack);
}

bool slf__entity__camera_orbit__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A500)(this, stack);
}

bool slf__entity__get_member__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A590)(this, stack);
}

bool slf__entity__has_member__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A640)(this, stack);
}

bool slf__entity__get_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A6E0)(this, stack);
}

bool slf__entity__get_trigger_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A790)(this, stack);
}

bool slf__entity__has_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A850)(this, stack);
}

bool slf__entity__get_member_abs_position__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A8F0)(this, stack);
}

bool slf__entity__get_target_member__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84A9B0)(this, stack);
}

bool slf__entity__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84ABE0)(this, stack);
}

bool slf__entity__entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84AD80)(this, stack);
}

bool slf__entity__suspend__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B5B0)(this, stack);
}

// sub_84B610
bool slf__entity__is_suspended__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && (self->flags & entity_flag_suspended) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__disable_collisions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B690)(this, stack);
}

bool slf__entity__enable_collisions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B6D0)(this, stack);
}

bool slf__entity__add_collision_ignorance__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B710)(this, stack);
}

bool slf__entity__remove_collision_ignorance__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B770)(this, stack);
}

bool slf__entity__unsuspend__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B7D0)(this, stack);
}

bool slf__entity__get_district_id__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B830)(this, stack);
}

bool slf__entity__als_play_animation_by_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84B950)(this, stack);
}

bool slf__entity__als_play_animation_by_filename__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BAB0)(this, stack);
}

bool slf__entity__update_member_visible__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BBD0)(this, stack);
}

bool slf__entity__update_member_color__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BCC0)(this, stack);
}

bool slf__entity__wait_update_member_color__str__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84BE70)(this, stack);
}

bool slf__entity__particle_generator_set_scalar__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C2F0)(this, stack);
}

bool slf__entity__get_enx_script_instance__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C310)(this, stack);
}

bool slf__entity__get_enx_script_object_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C3A0)(this, stack);
}

bool slf__entity__get_enx_script_exec_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C410)(this, stack);
}

bool slf__entity__is_enx_script_related__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84C490)(this, stack);
}

// sub_84C540
bool slf__entity__get_ai_param_float__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    bool         resolved;
    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter, &resolved);

    f32 result = !resolved ? -1.0f : block ? block->get_pb_float(parameter) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84C620
bool slf__entity__get_ai_param_float_variance__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    bool         resolved;
    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter, &resolved);

    f32 result = !resolved ? -1.0f : block ? block->get_pb_float_variance(parameter).get_random_value() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84C740
bool slf__entity__get_ai_param_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    arch_base_vhandle result = block ? block->get_pb_entity(parameter) : arch_base_vhandle();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84C840
bool slf__entity__get_ai_param_int__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    bool         resolved;
    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter, &resolved);

    f32 result = !resolved ? -1.0f : block ? (f32)block->get_pb_int(parameter) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84C910
bool slf__entity__get_ai_param_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    const char* result = block ? block->get_pb_fixedstring(parameter) : "";

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84C9F0
bool slf__entity__get_ai_param_hash__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    string_hash result = block ? block->get_pb_hash(parameter) : string_hash();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84CAD0
bool slf__entity__get_ai_param_vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    vector3d result = block ? block->get_pb_vector3d(parameter) : references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84CBD0
bool slf__entity__get_optional_ai_param_entity__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto default_value = arch_base_vhandle(stack.local_uint(0x08));

    bool         resolved;
    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter, &resolved);

    arch_base_vhandle result = !resolved ? arch_base_vhandle() :
        block ? block->get_optional_pb_entity(parameter, default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84CCD0
bool slf__entity__get_optional_ai_param_float__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto default_value = stack.local_num(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    f32 result = block ? block->get_optional_pb_float(parameter, default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84CDC0
bool slf__entity__get_optional_ai_param_int__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto default_value = stack.local_num(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    f32 result = block ? (f32)block->get_optional_pb_int(parameter, (i32)default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84CEA0
bool slf__entity__get_optional_ai_param_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto default_value = (const char*)stack.local_uint(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    const char* result = block ? block->get_optional_pb_fixedstring(parameter, default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84CF60
bool slf__entity__get_optional_ai_param_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto default_name  = (const char*)stack.local_uint(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    string_hash default_value;
    default_value.initialize(mash::ALLOCATED, default_name);

    string_hash result = block ? block->get_optional_pb_hash(parameter, default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84D0D0
bool slf__entity__get_optional_ai_param_vector3d__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto default_value = (const vector3d*)&stack.local_uint(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    vector3d result = block ? block->get_optional_pb_vector3d(parameter, *default_value, nullptr) : *default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84D1E0
bool slf__entity__get_optional_ai_param_float_variance__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    variance_variable<f32> default_value = { stack.local_num(0x08), stack.local_num(0x0C) };

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    f32 result = block ?
        block->get_optional_pb_float_variance(parameter, default_value, nullptr).get_random_value() :
        default_value.get_random_value_inline();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84D3C0
bool slf__entity__set_ai_param_entity__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto value      = arch_base_vhandle(stack.local_uint(0x08));

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    if (block)
        block->set_pb_entity(parameter, value);

    return true;
}

// sub_84D470
bool slf__entity__set_ai_param_float__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto value      = stack.local_num(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    if (block)
        block->set_pb_float(parameter, value);

    return true;
}

// sub_84D520
bool slf__entity__set_ai_param_float_variance__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    if (!block)
        return true;

    variance_variable<f32> value = { stack.local_num(0x08), stack.local_num(0x0C) };

    block->set_pb_float_variance(parameter, value);

    return true;
}

// sub_84D600
bool slf__entity__set_ai_param_int__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto value      = stack.local_num(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    if (block)
        block->set_pb_int(parameter, (i32)value);

    return true;
}

// sub_84D6B0
bool slf__entity__set_ai_param_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto value      = (const char*)stack.local_uint(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    if (block)
        block->set_pb_fixedstring(parameter, value);

    return true;
}

// sub_84D760
bool slf__entity__set_ai_param_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto value      = (const char*)stack.local_uint(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    if (!block)
        return true;

    string_hash hash;
    hash.initialize(mash::ALLOCATED, value);

    block->set_pb_hash(parameter, hash);

    return true;
}

// sub_84D830
bool slf__entity__set_ai_param_hash__str__uint__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto value      = stack.local_uint(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    if (!block)
        return true;

    string_hash hash;
    hash.initialize(mash::ALLOCATED, nullptr, value);

    block->set_pb_hash(parameter, hash);

    return true;
}

// sub_84D900
bool slf__entity__set_ai_param_vector3d__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto param_name = (const char*)stack.local_uint(0x04);
    auto value      = (const vector3d*)&stack.local_uint(0x08);

    string_hash  parameter;
    param_block* block = get_ai_param_block(self, param_name, parameter);

    if (block)
        block->set_pb_vector3d(parameter, *value);

    return true;
}

bool slf__entity__get_ai_idle_type__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84D9B0)(this, stack);
}

bool slf__entity__set_ai_idle_type__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DA10)(this, stack);
}

bool slf__entity__has_inode__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84DA50)(this, stack);
}

// sub_84DAE0
bool slf__entity__get_inode_param_float__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    f32 result = node ? node->my_param_block.get_pb_float(parameter) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DB60
bool slf__entity__get_inode_param_float_variance__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    f32 result = node ? node->my_param_block.get_pb_float_variance(parameter).get_random_value_inline() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DC30
bool slf__entity__get_inode_param_int__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    f32 result = node ? (f32)node->my_param_block.get_pb_int(parameter) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DCB0
bool slf__entity__get_inode_param_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    arch_base_vhandle result = node ? node->my_param_block.get_pb_entity(parameter) : arch_base_vhandle();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DD30
bool slf__entity__get_inode_param_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    const char* result = node ? node->my_param_block.get_pb_fixedstring(parameter) : "";

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DDB0
bool slf__entity__get_inode_param_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    f32 result = node ? (f32)node->my_param_block.get_pb_hash(parameter).source_hash_code : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DE40
bool slf__entity__get_inode_param_string_hash__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    string_hash result = node ? node->my_param_block.get_pb_hash(parameter) : string_hash();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DEC0
bool slf__entity__get_inode_param_vector3d__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    vector3d result = node ? node->my_param_block.get_pb_vector3d(parameter) : references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DF50
bool slf__entity__get_optional_inode_param_float__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto default_value = stack.local_num(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    f32 result = node ? node->my_param_block.get_optional_pb_float(parameter, default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DFE0
bool slf__entity__get_optional_inode_param_int__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto default_value = stack.local_num(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    f32 result = node ? (f32)node->my_param_block.get_optional_pb_int(parameter, (i32)default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84E060
bool slf__entity__get_optional_inode_param_entity__str__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto default_value = arch_base_vhandle(stack.local_uint(0x0C));

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    arch_base_vhandle result = node ? node->my_param_block.get_optional_pb_entity(parameter, default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84E0E0
bool slf__entity__get_optional_inode_param_str__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto default_value = (const char*)stack.local_uint(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    const char* result = node ? node->my_param_block.get_optional_pb_fixedstring(parameter, default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84E160
bool slf__entity__get_optional_inode_param_hash__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto default_name  = (const char*)stack.local_uint(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    string_hash default_value;
    default_value.initialize(mash::ALLOCATED, default_name);

    string_hash hash = node ? node->my_param_block.get_optional_pb_hash(parameter, default_value, nullptr) : default_value;

    f32 result = (f32)hash.source_hash_code;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84E220
bool slf__entity__get_optional_inode_param_string_hash__str__str__uint__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto default_hash  = stack.local_uint(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    string_hash default_value;
    default_value.initialize(mash::ALLOCATED, nullptr, default_hash);

    string_hash result = node ? node->my_param_block.get_optional_pb_hash(parameter, default_value, nullptr) : default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84E2D0
bool slf__entity__get_optional_inode_param_vector3d__str__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto default_value = (const vector3d*)&stack.local_uint(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    vector3d result = node ? node->my_param_block.get_optional_pb_vector3d(parameter, *default_value, nullptr) : *default_value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84E360
bool slf__entity__set_inode_param_float__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto value      = stack.local_num(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    if (node)
        node->my_param_block.set_pb_float(parameter, value);

    return true;
}

// sub_84E3C0
bool slf__entity__set_inode_param_float_variance__str__str__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    if (!node)
        return true;

    variance_variable<f32> value = { stack.local_num(0x0C), stack.local_num(0x10) };

    node->my_param_block.set_pb_float_variance(parameter, value);

    return true;
}

// sub_84E440
bool slf__entity__set_inode_param_int__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto value      = stack.local_num(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    if (node)
        node->my_param_block.set_pb_int(parameter, (i32)value);

    return true;
}

// sub_84E4A0
bool slf__entity__set_inode_param_entity__str__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto value      = arch_base_vhandle(stack.local_uint(0x0C));

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    if (node)
        node->my_param_block.set_pb_entity(parameter, value);

    return true;
}

// sub_84E500
bool slf__entity__set_inode_param_str__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto value      = (const char*)stack.local_uint(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    if (node)
        node->my_param_block.set_pb_fixedstring(parameter, value);

    return true;
}

// sub_84E560
bool slf__entity__set_inode_param_hash__str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto value      = (const char*)stack.local_uint(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    if (!node)
        return true;

    string_hash hash;
    hash.initialize(mash::ALLOCATED, value);

    node->my_param_block.set_pb_hash(parameter, hash);

    return true;
}

// sub_84E5E0
bool slf__entity__set_inode_param_hash__str__str__uint__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto value      = stack.local_uint(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    if (!node)
        return true;

    string_hash hash;
    hash.initialize(mash::ALLOCATED, nullptr, value);

    node->my_param_block.set_pb_hash(parameter, hash);

    return true;
}

// sub_84E660
bool slf__entity__set_inode_param_vector3d__str__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self       = arch_base_vhandle(stack.local_uint(0x00));
    auto inode_name = (const char*)stack.local_uint(0x04);
    auto param_name = (const char*)stack.local_uint(0x08);
    auto value      = (const vector3d*)&stack.local_uint(0x0C);

    info_node*  node;
    string_hash parameter = get_ai_param_hash_and_inode(self, param_name, inode_name, &node);

    if (node)
        node->my_param_block.set_pb_vector3d(parameter, *value);

    return true;
}

bool slf__entity__push_ai_base_machine_immediately__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E6C0)(this, stack);
}

bool slf__entity__pop_ai_base_machine_immediately__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E840)(this, stack);
}

bool slf__entity__change_locomotion_machine__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84E9A0)(this, stack);
}

bool slf__entity__change_locomotion_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EBB0)(this, stack);
}

bool slf__entity__ai_break_morale_force_cower__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84ECF0)(this, stack);
}

bool slf__entity__ai_break_morale_force_flee__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84ED70)(this, stack);
}

bool slf__entity__ai_break_morale_force_flee__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EE00)(this, stack);
}

bool slf__entity__get_ai_base_machine_state_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EEB0)(this, stack);
}

bool slf__entity__is_enemy__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EF10)(this, stack);
}

bool slf__entity__is_enemy_of_team__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84EFC0)(this, stack);
}

bool slf__entity__is_friend__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F060)(this, stack);
}

bool slf__entity__is_friend_of_team__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F110)(this, stack);
}

bool slf__entity__ai_has_clear_line_of_fire__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F1B0)(this, stack);
}

bool slf__entity__has_als_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F550)(this, stack);
}

bool slf__entity__get_als_category_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F610)(this, stack);
}

bool slf__entity__get_als_state_name__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F6C0)(this, stack);
}

bool slf__entity__force_strength_test__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F770)(this, stack);
}

bool slf__entity__begin_strength_test__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F840)(this, stack);
}

bool slf__entity__begin_strength_test__entity__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_880770)(this, stack);
}

bool slf__entity__begin_strength_test_trigger__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84F990)(this, stack);
}

bool slf__entity__run_strength_test__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FA60)(this, stack);
}

bool slf__entity__begin_slingshot_point_test__entity__vector3d__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FB40)(this, stack);
}

bool slf__entity__is_in_reflexes__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FCD0)(this, stack);
}

bool slf__entity__is_in_senses__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FD20)(this, stack);
}

bool slf__entity__is_sync_anim__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FD70)(this, stack);
}

bool slf__entity__get_als_time_to_end_of_anim__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FDD0)(this, stack);
}

bool slf__entity__get_als_current_anim_time__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FE50)(this, stack);
}

bool slf__entity__get_als_current_normalized_time__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_84FED0)(this, stack);
}

// sub_84FF50
bool slf__entity__set_als_param_float__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = arch_base_vhandle(stack.local_uint(0x00));
    auto param = (const char*)stack.local_uint(0x04);
    auto value = stack.local_num(0x08);

    arch_base* entity = self.resolve();

    if (!entity || !entity->is_an_actor())
        return true;

    als::animation_logic_system_interface* als = ((actor*)entity)->get_als();

    if (!als)
        return true;

    als::state_machine_interface* layer = als->get_als_layer(als::base_layer);

    if (!layer)
        return true;

    string_hash parameter;
    parameter.initialize(mash::ALLOCATED, param);

    layer->set_desired_param((als::external_float_parameter_types)als::find_param_id(parameter), value);

    return true;
}

// sub_84FFD0
bool slf__entity__set_als_param_vector3d__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self  = arch_base_vhandle(stack.local_uint(0x00));
    auto param = (const char*)stack.local_uint(0x04);
    auto value = (const vector3d*)&stack.local_uint(0x08);

    arch_base* entity = self.resolve();

    if (!entity || !entity->is_an_actor())
        return true;

    als::animation_logic_system_interface* als = ((actor*)entity)->get_als();

    if (!als)
        return true;

    als::state_machine_interface* layer = als->get_als_layer(als::base_layer);

    if (!layer)
        return true;

    string_hash parameter;
    parameter.initialize(mash::ALLOCATED, param);

    layer->set_desired_param((als::external_vector_parameter_types)als::find_param_id(parameter), *value);

    return true;
}

bool slf__entity__get_als_data_float__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850050)(this, stack);
}

bool slf__entity__get_als_data_int__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850150)(this, stack);
}

bool slf__entity__get_als_data_vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850250)(this, stack);
}

bool slf__entity__get_als_data_string__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850390)(this, stack);
}

// sub_8504E0
bool slf__entity__has_ai_core__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() && ((actor*)self)->get_ai_core() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__crawl_attach_to_surface__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8505E0)(this, stack);
}

bool slf__entity__crawl_release_from_surface__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850990)(this, stack);
}

bool slf__entity__hates_spiderman__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850A60)(this, stack);
}

bool slf__entity__set_detectable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850AE0)(this, stack);
}

bool slf__entity__set_spidey_sense_glow_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850B40)(this, stack);
}

bool slf__entity__clear_spidey_sense_glow_info__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850BA0)(this, stack);
}

bool slf__entity__set_target_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850BE0)(this, stack);
}

bool slf__entity__clear_target_icon__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850C20)(this, stack);
}

// sub_850C50
bool slf__entity__is_an_actor__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_850CB0
bool slf__entity__is_a_rocket__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() && ((actor*)self)->is_a_rocket() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_850D30
bool slf__entity__is_a_vehicle__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_vehicle() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

bool slf__entity__is_a_placeholder_vehicle__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850DA0)(this, stack);
}

bool slf__entity__set_vehicle_model__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850E10)(this, stack);
}

bool slf__entity__damage_vehicle_part__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850E60)(this, stack);
}

bool slf__entity__damage_all_vehicle_parts__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850EC0)(this, stack);
}

bool slf__entity__repair_vehicle_part__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850F00)(this, stack);
}

bool slf__entity__repair_all_vehicle_parts__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850F60)(this, stack);
}

bool slf__entity__get_vehicle_parts__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_86FF80)(this, stack);
}

bool slf__entity__set_vehicle_mass__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850FA0)(this, stack);
}

bool slf__entity__set_vehicle_self_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_850FE0)(this, stack);
}

bool slf__entity__set_vehicle_other_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851040)(this, stack);
}

bool slf__entity__get_vehicle_mass__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8510A0)(this, stack);
}

bool slf__entity__get_vehicle_dimensions__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851110)(this, stack);
}

bool slf__entity__get_vehicle_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8511A0)(this, stack);
}

bool slf__entity__get_vehicle_angular_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851240)(this, stack);
}

bool slf__entity__set_max_vehicle_speed__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8512E0)(this, stack);
}

bool slf__entity__get_max_vehicle_speed__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851320)(this, stack);
}

bool slf__entity__set_vehicle_flags__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851390)(this, stack);
}

bool slf__entity__get_vehicle_flags__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851400)(this, stack);
}

bool slf__entity__make_vehicle_clown_car__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851480)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8514E0)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851520)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851570)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8516F0)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851790)(this, stack);
}

bool slf__entity__set_vehicle_purpose__num__dynamic_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87EA20)(this, stack);
}

bool slf__entity__get_vehicle_purpose__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8518F0)(this, stack);
}

bool slf__entity__get_vehicle_purpose_entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851990)(this, stack);
}

bool slf__entity__get_vehicle_purpose_pos__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851A30)(this, stack);
}

bool slf__entity__get_vehicle_purpose_radius__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851AC0)(this, stack);
}

bool slf__entity__is_vehicle_purpose_done__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851B60)(this, stack);
}

bool slf__entity__set_vehicle_steering__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C190)(this, stack);
}

bool slf__entity__get_vehicle_steering__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C220)(this, stack);
}

bool slf__entity__set_vehicle_gas_pedal__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C2D0)(this, stack);
}

bool slf__entity__get_vehicle_gas_pedal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C360)(this, stack);
}

bool slf__entity__set_vehicle_brake_pedal__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C410)(this, stack);
}

bool slf__entity__get_vehicle_brake_pedal__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C4A0)(this, stack);
}

bool slf__entity__set_vehicle_power_brake__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C550)(this, stack);
}

bool slf__entity__get_vehicle_power_brake__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C5F0)(this, stack);
}

bool slf__entity__set_vehicle_reverse__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C6A0)(this, stack);
}

bool slf__entity__get_vehicle_reverse__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_87C740)(this, stack);
}

bool slf__entity__enable_vehicle_physics__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851C10)(this, stack);
}

bool slf__entity__is_vehicle_physics_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851C70)(this, stack);
}

bool slf__entity__set_vehicle_condition__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851CD0)(this, stack);
}

bool slf__entity__get_vehicle_condition__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851D10)(this, stack);
}

bool slf__entity__can_vehicle_be_stolen__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851D60)(this, stack);
}

bool slf__entity__set_vehicle_can_be_stolen__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851E00)(this, stack);
}

bool slf__entity__set_vehicle_sleeps_on_stop__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851E60)(this, stack);
}

bool slf__entity__weapon_fire_as_gun__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851ED0)(this, stack);
}

bool slf__entity__weapon_fire_as_rocket__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851F20)(this, stack);
}

bool slf__entity__weapon_fire_as_grenade__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851F70)(this, stack);
}

bool slf__entity__rocket_redirect__entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_851FC0)(this, stack);
}

bool slf__entity__rocket_redirect_dir__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852020)(this, stack);
}

bool slf__entity__rocket_disable_guidance__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852080)(this, stack);
}

bool slf__entity__force_ai_enter_car__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8520C0)(this, stack);
}

bool slf__entity__force_ai_exit_car__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8521B0)(this, stack);
}

bool slf__entity__get_car_seat__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852270)(this, stack);
}

bool slf__entity__is_seat_taken__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8522D0)(this, stack);
}

bool slf__entity__is_car_occupied__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852380)(this, stack);
}

bool slf__entity__vehicle_disabled_escape_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852410)(this, stack);
}

bool slf__entity__vehicle_eject_kills_occupants__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852480)(this, stack);
}

bool slf__entity__set_vehicle_occupant_damage_tolerance_percentage__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8524F0)(this, stack);
}

bool slf__entity__get_actor_in_seat__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852560)(this, stack);
}

bool slf__entity__get_tentacle__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8525E0)(this, stack);
}

bool slf__entity__set_attack_damage_locked__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852680)(this, stack);
}

bool slf__entity__is_a_variant__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8526C0)(this, stack);
}

bool slf__entity__get_variant_name__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852740)(this, stack);
}

bool slf__entity__get_variant_weight__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8527D0)(this, stack);
}

bool slf__entity__get_num_variants__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852850)(this, stack);
}

bool slf__entity__get_max_variant_texture__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8528C0)(this, stack);
}

bool slf__entity__get_min_variant_texture__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852940)(this, stack);
}

bool slf__entity__has_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8529C0)(this, stack);
}

bool slf__entity__find_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852A90)(this, stack);
}

bool slf__entity__prepare_change_variant__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852B50)(this, stack);
}

bool slf__entity__set_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852B90)(this, stack);
}

bool slf__entity__set_variant_id__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852C40)(this, stack);
}

bool slf__entity__set_variant_textures__num__num__num__num__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852CB0)(this, stack);
}

bool slf__entity__set_variant_and_textures__str__num__num__num__num__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852D30)(this, stack);
}

bool slf__entity__set_random_variant__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852E20)(this, stack);
}

bool slf__entity__set_random_textures__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852E70)(this, stack);
}

bool slf__entity__set_simple_variant_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852EC0)(this, stack);
}

bool slf__entity__get_simple_variant_mode__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852F70)(this, stack);
}

bool slf__entity__set_simple_variant__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_852FF0)(this, stack);
}

bool slf__entity__get_simple_variant__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8530A0)(this, stack);
}

bool slf__entity__aim_head__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853120)(this, stack);
}

bool slf__entity__aim_head_direct__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8531B0)(this, stack);
}

bool slf__entity__aim_head_and_eyes__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853240)(this, stack);
}

bool slf__entity__aim_head_eyes_and_upper_body__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8532D0)(this, stack);
}

bool slf__entity__aim_head_and_upper_body__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853360)(this, stack);
}

bool slf__entity__aim_head_and_upper_body_direct__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8533F0)(this, stack);
}

bool slf__entity__aim_left_arm__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853480)(this, stack);
}

bool slf__entity__aim_right_arm__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853510)(this, stack);
}

bool slf__entity__update_target_to_look_at__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8535A0)(this, stack);
}

bool slf__entity__physical_ifc_add_dynamic_pendulum__vector3d__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853620)(this, stack);
}

bool slf__entity__physical_ifc_add_dynamic_pendulum__entity__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8537F0)(this, stack);
}

bool slf__entity__physical_ifc_remove_dynamic_pendulum__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8539C0)(this, stack);
}

bool slf__entity__physical_ifc_set_dynamic_pendulum_length__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883960)(this, stack);
}

bool slf__entity__physical_ifc_get_dynamic_pendulum_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8839B0)(this, stack);
}

bool slf__entity__physical_ifc_set_dynamic_pendulum_attach_limb__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883A20)(this, stack);
}

bool slf__entity__physical_ifc_get_dynamic_pendulum_attach_limb__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883A70)(this, stack);
}

bool slf__entity__physical_ifc_set_dynamic_pendulum_maintain_rel_orient__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883AF0)(this, stack);
}

bool slf__entity__physical_ifc_set_dynamic_pendulum_maintain_abs_orient__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_883B70)(this, stack);
}

bool slf__entity__set_puppet_slave_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_879DA0)(this, stack);
}

bool slf__entity__get_floor_offset__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853A20)(this, stack);
}

bool slf__entity__set_floor_offset_override__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853AB0)(this, stack);
}

bool slf__entity__can_be_web_yanked__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853B00)(this, stack);
}

bool slf__entity__get_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853BD0)(this, stack);
}

bool slf__entity__set_crawlable__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853DC0)(this, stack);
}

bool slf__entity__in_combat_state__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853E10)(this, stack);
}

bool slf__entity__set_tentacle_end_target__uint__entity__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853EC0)(this, stack);
}

bool slf__entity__set_tentacle_default_pct__uint__num__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853F00)(this, stack);
}

bool slf__entity__set_tentacle_effect__uint__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853F40)(this, stack);
}

bool slf__entity__play_tentacle_effect__uint__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853F90)(this, stack);
}

bool slf__entity__spawn_effect__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_853FE0)(this, stack);
}

bool slf__entity__spawn_effect_at_entity_position__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854030)(this, stack);
}

bool slf__entity__spawn_effect_at_world_position__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8542E0)(this, stack);
}

bool slf__entity__spawn_looping_effect__str__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854360)(this, stack);
}

bool slf__entity__stop_looping_effect__uint__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_8543E0)(this, stack);
}

bool slf__entity__stop_all_effects__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854410)(this, stack);
}

bool slf__entity__get_teleport_target_position__t::operator()(chuck::vm::vm_stack &stack) {
    return ((invoke_t)retail::sub_854450)(this, stack);
}
