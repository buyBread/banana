#include <bit>
#include <cmath>
#include <cstring>

#include "retail.hh"
#include "game/als/als.hh"
#include "game/als/param.hh"
#include "game/amalga/resource_types.hh"
#include "game/arch_base.hh"
#include "game/event/event.hh"
#include "game/frontend/entity_tracker.hh"
#include "game/frontend/frontend_manager.hh"
#include "game/frontend/ui_frontend.hh"
#include "game/global.hh"
#include "game/line_info.hh"
#include "game/math/references.hh"
#include "game/mission/mission_manager.hh"
#include "game/physics/physics_system.hh"
#include "game/physics/rigid_body_prop_system.hh"
#include "game/polytube.hh"
#include "game/script/chuck_lib_entity.hh"
#include "game/script/script_access.hh"
#include "game/trigger.hh"
#include "game/wds/ai/ai_basic_inode.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_state_machine.hh"
#include "game/wds/ai/ai_team.hh"
#include "game/wds/ai/ai_weapon_inode.hh"
#include "game/wds/ai/ai_vehicle.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/ai/param_block.hh"
#include "game/wds/camera/camera.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/grenade.hh"
#include "game/wds/entity/interface/standing_interface.hh"
#include "game/wds/entity/pendulum.hh"
#include "game/wds/entity/vehicle.hh"
#include "game/wds/references.hh"
#include "game/wds/render/references.hh"
#include "game/wds/region.hh"
#include "game/wds/world_dynamics_system.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/jq/native_locks.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mm/mm.hh"
#include "vc8/list.hh"
#include "vc8/vector.hh"

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

        return &((vehicle*)entity)->my_ai->m_inode->my_param_block;
    }

    ai_core* core;
    parameter = get_ai_param_hash_and_core(handle, param_name, &core);

    return core ? &core->my_param_block : nullptr;
}

// inlined @ sub_8470B0
// inlined @ sub_847140
// inlined @ sub_8471B0
bool treyarch::marky_camera_allows(const arch_base* self, const chuck::vm::vm_stack &stack) {
    auto* marky = (marky_camera*)references::g_world_ptr.read()->camera_mgr.marky_camera;

    return self != marky || stack.thread->camera_priority == marky->current_priority;
}

// inlined @ sub_87C190
// inlined @ sub_87C220
// inlined @ sub_87C2D0
// inlined @ sub_87C360
// inlined @ sub_87C410
// inlined @ sub_87C4A0
// inlined @ sub_87C550
// inlined @ sub_87C5F0
// inlined @ sub_87C6A0
// inlined @ sub_87C740
ai_vehicle_state_script* treyarch::get_script_controlled_state(vehicle* self) {
    ai_vehicle_state* state = self->my_ai->m_machine->get_state();

    // an empty machine is asked through null
    if (state->get_type() != ai_vehicle_state::type_script)
        return nullptr;

    if (self->my_ai->m_inode->get_purpose() != ai_vehicle_purpose_script_controlled)
        return nullptr;

    return (ai_vehicle_state_script*)state;
}

// inlined @ sub_84BE70
// inlined @ sub_86A5A0
// inlined @ sub_86ABF0
f32 treyarch::get_wait_time_step(const entity* self) {
    f32 dilation = self->has_ifc(entity_ifc_time) ?
        self->time_ifc()->calc_time_dilation() :
        references::g_world_ptr.read()->time_mgr.get_time_dilation_factor();

    return chuck::vm::script_manager::inst()->get_time_inc() * dilation;
}

// inlined @ sub_844F70
// inlined @ sub_845230
// inlined @ sub_8454B0
// inlined @ sub_8456D0
// inlined @ sub_8458B0
// inlined @ sub_845AE0
// inlined @ sub_845EF0
// inlined @ sub_849DB0
// inlined @ sub_849F90
// inlined @ sub_871190
void treyarch::update_als_facing(entity* self) {
    if (!self->is_an_actor() || !((actor*)self)->get_ai_core())
        return;

    auto* node = (als_inode*)((actor*)self)->get_ai_core()->get_info_node(info_node_type_als);

    if (!node)
        return;

    retail::sub_4029D0((u32**)node, als::param_desired_forward_vector, (i32)&self->my_abs_po->get_z_facing(), 0); // als_inode::set_desired_param

    if (node->my_als)
        ((als::animation_logic_system_interface*)node->my_als)->force_matrix_update();
}

void treyarch::set_or_move_rel_position(entity* self, const vector3d* position, bool set, f32 camera_priority) {
    u32 set_word = set;

    __asm {
        push camera_priority
        push set_word
        push position
        mov  ecx, self
        call retail::sub_879860
        add  esp, 0Ch
    }
}

// inlined @ sub_83DDF0
// inlined @ sub_84AA40
// inlined @ sub_84ABE0
// inlined @ sub_84AEA0
// inlined @ sub_84B160
string_hash treyarch::next_script_ent_name(const util::memory_reference<void*> &pool, const util::memory_reference<u32> &guard) {
    if (!(guard.read() & 1)) {
        guard.write(guard.read() | 1);

        mash::string prefix("SCRIPT_ENT");
        pool.write(retail::sub_64CBB0((char*)&prefix)); // the name pool for a prefix
    }

    string_hash slot;

    return *(string_hash*)retail::sub_64CCA0((u32*)&slot, (u32*)pool.read(), (void*)""); // the pool's next name
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

// sub_83DB50
bool slf__hero__t::operator()(chuck::vm::vm_stack &stack) {
    entity* hero = references::g_world_ptr.read()->get_hero_ptr();

    arch_base_vhandle result = hero ? hero->my_handle : arch_base_vhandle();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83DBA0
bool slf__chase_cam__t::operator()(chuck::vm::vm_stack &stack) {
    arch_base_vhandle result = references::g_world_ptr.read()->get_chase_cam_ptr() ?
        references::g_world_ptr.read()->get_chase_cam_ptr()->my_handle :
        arch_base_vhandle();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84AA40
bool slf__create_devopt_entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    mash::string name("");

    switch ((i32)stack.local_num(0x00)) {
        case 1: name = references::devopt_entity_name_1.get(); break;
        case 2: name = references::devopt_entity_name_2.get(); break;
        case 3: name = references::devopt_entity_name_3.get(); break;
        case 4: name = references::devopt_entity_name_4.get(); break;
    }

    arch_base_vhandle result;

    if (!name.empty()) {
        string_hash instance_name = next_script_ent_name(references::devopt_entity_script_ent_pool, references::devopt_entity_script_ent_pool_guard);

        auto spawned = ((entity* (__cdecl*)(mash::string, string_hash, bool))retail::sub_83DC00)(mash::string(name), instance_name, false); // spawn by name

        if (spawned) {
            result = spawned->my_handle;

            stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, result.value);
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84AEA0
bool slf__create_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    arch_base_vhandle result;

    {
        native_resource_context context(stack);

        string_hash instance_name = next_script_ent_name(references::create_entity_script_ent_pool, references::create_entity_script_ent_pool_guard);

        auto spawned = ((entity* (__cdecl*)(mash::string, string_hash, bool))retail::sub_83DC00)(mash::string(name), instance_name, false); // spawn by name

        if (spawned) {
            result = spawned->my_handle;

            stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, result.value);
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B040
bool slf__create_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto name     = (const char*)stack.local_uint(0x00);
    auto instance = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    {
        native_resource_context context(stack);

        string_hash  instance_name;
        string_hash* instance_slot = &instance_name;

        // a name ending in '*' takes the next name from that prefix's pool
        __asm {
            push instance
            mov  esi, instance_slot
            call retail::sub_8238E0
            add  esp, 4
        }

        auto spawned = ((entity* (__cdecl*)(mash::string, string_hash, bool))retail::sub_83DC00)(mash::string(name), instance_name, false); // spawn by name

        if (spawned) {
            result = spawned->my_handle;

            stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, result.value);
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B160
bool slf__create_preregistered_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    arch_base_vhandle result;

    {
        native_resource_context context(stack);

        string_hash instance_name = next_script_ent_name(references::preregistered_script_ent_pool, references::preregistered_script_ent_pool_guard);

        auto spawned = ((entity* (__cdecl*)(mash::string, string_hash, bool))retail::sub_83DC00)(mash::string(name), instance_name, true); // spawn by name, preregistered

        if (spawned) {
            result = spawned->my_handle;

            stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, result.value);
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B300
bool slf__create_preregistered_entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto name     = (const char*)stack.local_uint(0x00);
    auto instance = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    {
        native_resource_context context(stack);

        string_hash  instance_name;
        string_hash* instance_slot = &instance_name;

        // a name ending in '*' takes the next name from that prefix's pool
        __asm {
            push instance
            mov  esi, instance_slot
            call retail::sub_8238E0
            add  esp, 4
        }

        auto spawned = ((entity* (__cdecl*)(mash::string, string_hash, bool))retail::sub_83DC00)(mash::string(name), instance_name, true); // spawn by name, preregistered

        if (spawned) {
            result = spawned->my_handle;

            stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, result.value);
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_83DDF0
bool slf__create_next_preregistered_debug_obstacle__t::operator()(chuck::vm::vm_stack &stack) {
    arch_base_vhandle result;

    {
        native_resource_context context(stack);

        bool spawned_one = false;

        // two tries at the entity that's literally named "only use this if it is enabled."
        for (i32 i = 0; i < 2 && !spawned_one; ++i) {
            string_hash instance_name = next_script_ent_name(references::debug_obstacle_script_ent_pool, references::debug_obstacle_script_ent_pool_guard);

            auto spawned = ((entity* (__cdecl*)(mash::string, string_hash, bool))retail::sub_83DC00)(mash::string("only use this if it is enabled."), instance_name, true); // spawn by name, preregistered

            if (spawned) {
                result = spawned->my_handle;

                stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, result.value);

                spawned_one = true;
            }
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B420
bool slf__find_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    string_hash hash;
    hash.initialize(mash::ALLOCATED, name);

    arch_base_vhandle result;

    if (auto* found = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true)) // entity_handle_manager::find_entity
        result = found->my_handle;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B490
bool slf__find_entity__uint__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    string_hash hash;
    hash.initialize(mash::ALLOCATED, nullptr, stack.local_uint(0x00));

    arch_base_vhandle result;

    if (auto* found = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true)) // entity_handle_manager::find_entity
        result = found->my_handle;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B4F0
bool slf__entity_exists__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    string_hash hash;
    hash.initialize(mash::ALLOCATED, name);

    f32 result = retail::sub_640AE0(&hash.source_hash_code, 28, true) ? 1.0f : 0.0f; // entity_handle_manager::find_entity

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B560
bool slf__destroy_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    arch_base_vhandle handle(stack.local_uint(0x00));

    stack.thread->get_instance(-1)->remove_allocated_stuff(chuck::vm::script_garbage_collection_entity, handle.value);

    if (arch_base* self = handle.resolve())
        retail::sub_7FBF20((u32*)&references::g_world_ptr.read()->ent_mgr, (i32*)self); // wds_entity_manager::destroy_entity

    return true;
}

// sub_84B8C0
bool slf__entity_has_entity_tracker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    entity_tracker_manager* trackers = references::frontend.get().igo->entity_tracker_mgr;

    f32 result = trackers->get_tracker_by_entity(arch_base_vhandle(stack.local_uint(0x00))) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B910
bool slf__entity_get_entity_tracker__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    entity_tracker_id result = references::frontend.get().igo->entity_tracker_mgr->get_tracker_by_entity(arch_base_vhandle(stack.local_uint(0x00)));

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84BBA0
bool slf__enable_entity_fading__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    references::entity_fading_enabled.write(stack.local_num(0x00) != 0.0f);

    return true;
}

// sub_880970
bool slf__script_get_entities_of_type__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto type    = (i32)stack.local_num(0x00);
    auto results = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);

    if (!results) {
        u32 none = 0;
        stack.push(&none, sizeof(none));

        return true;
    }

    results->clear();

    void* allocation = mm::allocate(sizeof(vc8::list<arch_base_vhandle>));

    auto found = allocation ?
        new (allocation) vc8::list<arch_base_vhandle>() : nullptr;

    retail::sub_643290((i32)found, type); // lists the handles of every entity of the type

    // the count takes in the ones that no longer resolve
    i32 count = 0;

    for (auto* node = found->begin(); node != found->end(); node = node->next) {
        ++count;

        arch_base_vhandle handle = node->value;

        if (handle.resolve()) {
            chuck::vm::chuck_dynamic_array_element_t cell;
            results->push_back(cell);

            *(arch_base_vhandle*)results->back().stuff = handle;
        }
    }

    found->~list();
    mm::free(found);

    f32 result = (f32)count;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_880AB0
bool slf__script_get_sorted_entities_of_type__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto results = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);

    if (!results) {
        u32 none = 0;
        stack.push(&none, sizeof(none));

        return true;
    }

    results->clear();

    struct sorted_entity {
        f32               distance_squared;
        arch_base_vhandle handle;
    };

    void* allocation = mm::allocate(sizeof(vc8::list<sorted_entity>));

    auto found = allocation ?
        new (allocation) vc8::list<sorted_entity>() : nullptr;

    // lists them with their distance to the hero's rel position, sorted
    ((void (__cdecl*)(vc8::list<sorted_entity>*, i32, vector3d))retail::sub_659020)(found,
                                                                                   (i32)stack.local_num(0x00),
                                                                                   references::g_world_ptr.read()->hero_ptr->my_rel_po->get_position());

    i32 count = 0;

    for (auto* node = found->begin(); node != found->end(); node = node->next) {
        ++count;

        arch_base_vhandle handle = node->value.handle;

        if (handle.resolve()) {
            chuck::vm::chuck_dynamic_array_element_t cell;
            results->push_back(cell);

            *(arch_base_vhandle*)results->back().stuff = handle;
        }
    }

    found->~list();
    mm::free(found);

    f32 result = (f32)count;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_850550
bool slf__crawl_attach_to_surface__entity__line_info__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self            = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto lif             = (const line_info*)stack.local_uint(0x04);
    auto desired_forward = (const vector3d*)&stack.local_uint(0x08);

    if (!self || !self->is_an_actor())
        return true;

    ai_core* core = self->get_ai_core();

    if (!core)
        return true;

    auto* crawl = (crawl_layer_inode*)core->get_info_node(info_node_type_crawl_layer);

    // only while crawling is the actor's locomotion
    if (crawl && core->my_loco_inode == crawl)
        crawl->attach_to_surface(*lif, *desired_forward);

    return true;
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

// sub_8443F0
bool slf__entity__get_visual_center__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result(0.0f, 0.0f, 0.0f);

    // anything but an actor answers its position
    if (self)
        result = self->is_an_actor() ? self->get_visual_center() : self->my_abs_po->get_position();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844490
bool slf__entity__get_visual_radius__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() ? self->get_visual_radius() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844500
bool slf__entity__get_abs_position__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_abs_po->get_position() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844570
bool slf__entity__compute_sector__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_entity())
        self->compute_sector(references::g_world_ptr.read()->the_terrain, false, nullptr);

    return true;
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

// sub_844900
bool slf__entity__get_last_anchor__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result;

    // the hero's last anchor, whichever actor asks
    if (self && self->is_an_actor() && ((actor*)self)->get_ai_core()) {
        entity* anchor = references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->get_last_anchor();

        if (anchor && anchor->is_an_entity())
            result = anchor->my_handle;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8449A0
bool slf__entity__get_rel_position__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self ? self->my_rel_po->get_position() : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_844A10
bool slf__entity__set_parent__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self)
        return true;

    auto parent = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    // a conglomerate member keeps its parent
    if ((self->flags & entity_flag_member) || !marky_camera_allows(self, stack))
        return true;

    if (!parent)
        retail::sub_640910((u32*)self); // entity_base::clear_parent
    else {
        retail::sub_653DA0((u32*)self, (i32*)parent);                          // entity_base::set_parent
        retail::sub_626CA0((u32*)self, (u32*)&references::unk_010f7140.get()); // entity_base::set_rel_position
    }

    if (self->is_an_entity()) {
        self->compute_sector(references::g_world_ptr.read()->the_terrain, false, nullptr);
        retail::sub_612EB0((u32*)self, 0); // entity::update_proximity_maps
    }

    if (self->is_an_actor())
        ((actor*)self)->save_last_collision_free_state();

    return true;
}

// sub_844B10
bool slf__entity__set_parent_rel__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self)
        return true;

    auto parent = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    if ((self->flags & entity_flag_member) || !marky_camera_allows(self, stack))
        return true;

    retail::sub_653EB0((u32*)self, (i32)parent); // attaches it keeping its world po

    if (self->is_an_actor())
        ((actor*)self)->save_last_collision_free_state();

    return true;
}

// sub_844BA0
bool slf__entity__get_vehicle__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    u32 result = 0;

    if (self && self->my_parent) {
        auto basic = ((ai_basic_inode* (__cdecl*)(arch_base*))retail::sub_4C77B0)(self); // get_ai_basic_inode_if_slim_actor

        if (basic && basic->get_flag(963)) {
            auto vehicle = (entity*)arch_base_vhandle(self->my_parent->my_handle.value).resolve();

            if (vehicle && (vehicle->flags & entity_flag_member))
                vehicle = (entity*)retail::sub_601910((u32*)vehicle); // entity_base::get_attach_root

            if (vehicle && vehicle->is_a_vehicle())
                result = vehicle->my_handle.value;
        }
    }

    stack.push(&result, sizeof(result));

    return true;
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

// sub_879C80
bool slf__entity__set_rel_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto position = (const vector3d*)&stack.local_uint(0x04);
    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self)
        return true;

    mash::string unused; // built and dropped around the call

    set_or_move_rel_position(self, position, true, stack.thread->camera_priority);

    return true;
}

// sub_879D10
bool slf__entity__move_rel_position__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto position = (const vector3d*)&stack.local_uint(0x04);
    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self)
        return true;

    mash::string unused; // built and dropped around the call

    set_or_move_rel_position(self, position, false, stack.thread->camera_priority);

    return true;
}

// sub_844CD0
bool slf__entity__set_velocity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto velocity = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !marky_camera_allows(self, stack))
        return true;

    // the velocity is in the entity's own frame
    vector3d world_velocity = self->my_abs_po->non_affine_slow_xform(*velocity);

    if (self->is_an_actor() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_velocity(world_velocity);

    return true;
}

// sub_844E70
bool slf__entity__set_abs_velocity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto velocity = (const vector3d*)&stack.local_uint(0x04);

    if (self &&
        marky_camera_allows(self, stack) &&
        self->is_an_actor() &&
        self->has_ifc(entity_ifc_physical))

        self->physical_ifc()->set_velocity(*velocity);

    return true;
}

// sub_844EF0
bool slf__entity__look_at__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (const vector3d*)&stack.local_uint(0x04);

    if (!self)
        return true;

    // a biped under physics isn't turned
    if (self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
        return true;

    if (marky_camera_allows(self, stack))
        retail::sub_626E30((u32*)self, (f32*)target);

    return true;
}

// sub_844F70
bool slf__entity__set_xz_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto facing = (vector3d*)&stack.local_uint(0x04);

    if (!self)
        return true;

    // a zero facing is replaced on the stack by the current one
    const vector3d &zero = references::unk_010f7140.get();

    if (facing->x == zero.x && facing->y == zero.y && facing->z == zero.z)
        *facing = self->my_rel_po->get_z_facing();

    if (self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
        return true;

    if (!marky_camera_allows(self, stack))
        return true;

    math::vector4 facing_4(facing->x, facing->y, facing->z, 0.0f);
    math::vector4 unit;

    retail::sub_401AA0((u64*)&unit, (f32*)&facing_4);

    // too close to straight up turns nothing
    if (!(0.95 > (f64)unit[1]))
        return true;

    vector3d z_facing(unit[0], 0.0f, unit[2]);
    retail::sub_402050((f32*)&z_facing);

    const vector3d &up = references::unk_00f2213c.get();

    vector3d x_facing = math::cross(up, z_facing);
    retail::sub_402050((f32*)&x_facing);

    po rel_po;
    retail::sub_5FC820((u64*)&rel_po, (i32)&x_facing, (i32)&up, (u32*)&z_facing, (u32*)&self->my_rel_po->get_position());
    retail::sub_9595B0((i32)&rel_po);             // orthonormalize
    retail::sub_626BE0((i32)self, (u64*)&rel_po); // entity_base::set_rel_po

    update_als_facing(self);

    return true;
}

// sub_845230
bool slf__entity__set_facing__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto facing = (const vector3d*)&stack.local_uint(0x04);
    auto up     = (const vector3d*)&stack.local_uint(0x10);

    if (!self)
        return true;

    if (self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
        return true;

    if (0.01f > facing->length2() || 0.01f > up->length2())
        return true;

    vector3d z_facing = *facing;
    retail::sub_402050((f32*)&z_facing);

    vector3d y_facing = *up;
    retail::sub_402050((f32*)&y_facing);

    vector3d x_facing = math::cross(y_facing, z_facing);

    if (0.01f > x_facing.length2())
        return true;

    retail::sub_402050((f32*)&x_facing);

    po rel_po;
    retail::sub_5FC820((u64*)&rel_po, (i32)&x_facing, (i32)&y_facing, (u32*)&z_facing, (u32*)&self->my_rel_po->get_position());
    retail::sub_9595B0((i32)&rel_po);             // orthonormalize
    retail::sub_626BE0((i32)self, (u64*)&rel_po); // entity_base::set_rel_po

    update_als_facing(self);

    return true;
}

// sub_8454B0
bool slf__entity__set_abs_xz_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !marky_camera_allows(self, stack))
        return true;

    const vector3d &position = self->my_abs_po->get_position();

    vector3d z_facing((f32)((f64)target->x - position.x), 0.0f, (f32)((f64)target->z - position.z));

    if (!((f32)std::sqrt((f64)z_facing.length2()) > 0.01f))
        return true;

    retail::sub_402050((f32*)&z_facing);

    const vector3d &up = references::unk_00f2213c.get();

    vector3d x_facing = math::cross(up, z_facing);
    retail::sub_402050((f32*)&x_facing);

    po abs_po;
    retail::sub_5FC820((u64*)&abs_po, (i32)&x_facing, (i32)&up, (u32*)&z_facing, (u32*)&position);
    retail::sub_627180((i32)self, (f32*)&abs_po); // entity_base::set_abs_po

    update_als_facing(self);

    return true;
}

// sub_8456D0
bool slf__entity__set_po_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto facing = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !marky_camera_allows(self, stack))
        return true;

    if (0.01f > facing->length2())
        return true;

    vector3d z_facing = *facing;
    retail::sub_402050((f32*)&z_facing);

    po       abs_po = references::unk_010f76b0.read();
    vector4d z_facing_4(z_facing.x, z_facing.y, z_facing.z, 0.0f);

    retail::sub_44DE20((f32*)&abs_po, (f32*)&z_facing_4); // turns it to look along the facing

    const vector3d &position = self->my_abs_po->get_position();

    abs_po.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

    retail::sub_627180((i32)self, (f32*)&abs_po); // entity_base::set_abs_po

    update_als_facing(self);

    return true;
}

// sub_8458B0
bool slf__entity__set_rel_po_facing__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto facing = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !marky_camera_allows(self, stack))
        return true;

    if (0.01f > facing->length2())
        return true;

    vector3d z_facing = *facing;
    retail::sub_402050((f32*)&z_facing);

    po       rel_po = references::unk_010f76b0.read();
    vector4d z_facing_4(z_facing.x, z_facing.y, z_facing.z, 0.0f);

    retail::sub_44DE20((f32*)&rel_po, (f32*)&z_facing_4); // turns it to look along the facing

    const vector3d &position = self->my_rel_po->get_position();

    rel_po.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

    retail::sub_626BE0((i32)self, (u64*)&rel_po); // entity_base::set_rel_po

    update_als_facing(self);

    return true;
}

// sub_845A90
bool slf__entity__apply_continuous_rotation__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto axis  = (f32*)&stack.local_uint(0x04);
    auto speed = stack.local_num(0x10);

    retail::sub_605320(&stack.local_uint(0x00)); // arch_base_vhandle::is_possibly_valid, answer unused

    retail::sub_60FEB0(stack.local_uint(0x00), axis, speed, stack.local_num(0x14) > 0.0f);

    return true;
}

// sub_845AE0
bool slf__entity__wait_rotate__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto  self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto  axis     = (const vector3d*)&stack.local_uint(0x04);
    auto &angle    = stack.local_num(0x10);
    auto &duration = stack.local_num(0x14);

    if (!self)
        return true;

    auto root = (entity*)retail::sub_601910((u32*)self); // entity_base::get_attach_root

    if (root) {
        if (root->unk_030)
            retail::sub_796C70(root->my_handle.value); // add_moved
    } else if (self->unk_030)
        retail::sub_796C70(self->my_handle.value);

    // a short enough turn happens at once
    if (duration <= 0.0001) {
        if (!marky_camera_allows(self, stack))
            return true;

        po rotation;
        retail::sub_4015B0((u64*)&rotation);

        vector4d axis_4(axis->x, axis->y, axis->z, 0.0f);
        retail::sub_40A750((u64*)&rotation, (f32*)&axis_4, &angle);

        if (self->is_an_actor())
            ((actor*)self)->set_frame_delta(rotation, 1.0f);

        retail::sub_44E230((u64*)&rotation, (f32*)&rotation, (f32*)self->my_rel_po);
        retail::sub_626BE0((i32)self, (u64*)&rotation); // entity_base::set_rel_po

        update_als_facing(self);

        return true;
    }

    if (!stack.thread->entry)
        return false;

    // the remaining time and angle stay in the argument words
    f32 step      = get_wait_time_step(self);
    f32 remaining = (f32)((f64)duration - step);

    if (0.0f > remaining) {
        step      = (f32)((f64)remaining + step);
        remaining = 0.0f;
    }

    f32 angle_step = 0.0f;

    if (marky_camera_allows(self, stack)) {
        angle_step = (f32)((f64)step / duration * angle);

        po rotation;
        retail::sub_4015B0((u64*)&rotation);

        vector4d axis_4(axis->x, axis->y, axis->z, 0.0f);
        retail::sub_40A750((u64*)&rotation, (f32*)&axis_4, &angle_step);

        if (self->is_an_actor())
            ((actor*)self)->set_frame_delta_no_update(rotation, step);

        retail::sub_44E230((u64*)&rotation, (f32*)&rotation, (f32*)self->my_rel_po);
        retail::sub_626BE0((i32)self, (u64*)&rotation); // entity_base::set_rel_po

        update_als_facing(self);
    }

    if ((f64)remaining > 0.0001) {
        duration = remaining;
        angle    = (f32)((f64)angle - angle_step);

        return false;
    }

    return true;
}

// sub_845EF0
bool slf__entity__wait_rotate_WCS__vector3d__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x24;

    auto  self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto  pivot    = (const vector3d*)&stack.local_uint(0x04);
    auto  axis     = (const vector3d*)&stack.local_uint(0x10);
    auto &angle    = stack.local_num(0x1C);
    auto &duration = stack.local_num(0x20);

    if (!self)
        return true;

    auto root = (entity*)retail::sub_601910((u32*)self); // entity_base::get_attach_root

    if (root) {
        if (root->unk_030)
            retail::sub_796C70(root->my_handle.value); // add_moved
    } else if (self->unk_030)
        retail::sub_796C70(self->my_handle.value);

    // a short enough turn is over before it starts
    if (!stack.thread->entry)
        return duration <= 0.0001;

    f32 step      = get_wait_time_step(self);
    f32 remaining = (f32)((f64)duration - step);

    if (0.0f > remaining) {
        step      = (f32)((f64)remaining + step);
        remaining = 0.0f;
    }

    f32 angle_step = 0.0f;

    // turned about the pivot: moved to it, turned in the parent's frame and moved back
    if (marky_camera_allows(self, stack)) {
        angle_step = (f32)((f64)step / duration * angle);

        const vector3d &position = self->my_rel_po->get_position();

        vector3d offset((f32)((f64)position.x - pivot->x),
                        (f32)((f64)position.y - pivot->y),
                        (f32)((f64)position.z - pivot->z));

        retail::sub_626CA0((u32*)self, (u32*)&offset); // entity_base::set_rel_position

        po rotation;
        retail::sub_4015B0((u64*)&rotation);

        vector4d axis_4(axis->x, axis->y, axis->z, 0.0f);
        retail::sub_40A750((u64*)&rotation, (f32*)&axis_4, &angle_step);

        if (self->is_an_actor())
            ((actor*)self)->set_frame_delta(rotation, step);

        retail::sub_44E230((u64*)&rotation, (f32*)self->my_rel_po, (f32*)&rotation);
        retail::sub_626BE0((i32)self, (u64*)&rotation); // entity_base::set_rel_po

        const vector3d &turned = self->my_rel_po->get_position();

        vector3d back((f32)((f64)turned.x + pivot->x),
                      (f32)((f64)turned.y + pivot->y),
                      (f32)((f64)turned.z + pivot->z));

        retail::sub_626CA0((u32*)self, (u32*)&back); // entity_base::set_rel_position

        update_als_facing(self);
    }

    if ((f64)remaining > 0.0001) {
        duration = remaining;
        angle    = (f32)((f64)angle - angle_step);

        return false;
    }

    return true;
}

// sub_8462C0
bool slf__entity__wait_translate__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto  self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto  delta    = (vector3d*)&stack.local_uint(0x04);
    auto &duration = stack.local_num(0x10);

    if (!self)
        return true;

    // a short enough move is over before it starts
    if (!stack.thread->entry)
        return duration <= 0.0001;

    // the remaining time and offset stay in the argument words
    f32 step      = get_wait_time_step(self);
    f32 remaining = (f32)((f64)duration - step);

    if (0.0f > remaining) {
        step      = (f32)((f64)remaining + step);
        remaining = 0.0f;
    }

    vector3d move = references::unk_010f7140.read();

    if (marky_camera_allows(self, stack)) {
        if (self->has_ifc(entity_ifc_standing) && self->is_an_actor() && ((actor*)self)->get_ai_core())
            self->standing_ifc()->set_control_parent(nullptr);

        f32 fraction = (f32)((f64)step / duration);

        move = vector3d((f32)((f64)delta->x * fraction),
                        (f32)((f64)delta->y * fraction),
                        (f32)((f64)delta->z * fraction));

        po       translation = references::unk_010f76b0.read();
        vector4d move_4(move.x, move.y, move.z, 0.0f);

        retail::sub_686C30((u64*)&translation, (u32*)&move_4); // the translation po

        if (self->is_an_actor())
            ((actor*)self)->set_frame_delta(translation, step);

        retail::sub_44E230((u64*)&translation, (f32*)self->my_rel_po, (f32*)&translation);
        retail::sub_796C70(stack.local_uint(0x00)); // add_moved

        if (self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
            self->physical_ifc()->get_biped_system();
        else
            retail::sub_626BE0((i32)self, (u64*)&translation); // entity_base::set_rel_po

        if (self->is_an_actor())
            ((actor*)self)->update_colgeom();

        if (self->has_ifc(entity_ifc_player))
            self->player_ifc()->unk_140();
    }

    if ((f64)remaining > 0.0001) {
        duration = remaining;
        delta->x = (f32)((f64)delta->x - move.x);
        delta->y = (f32)((f64)delta->y - move.y);
        delta->z = (f32)((f64)delta->z - move.z);

        return false;
    }

    return true;
}

// sub_8466A0
bool slf__entity__wait_translate_with_compute_sector__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto  self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto  delta    = (vector3d*)&stack.local_uint(0x04);
    auto &duration = stack.local_num(0x10);

    if (!self)
        return true;

    if (!stack.thread->entry)
        return duration <= 0.0001;

    f32 step      = get_wait_time_step(self);
    f32 remaining = (f32)((f64)duration - step);

    if (0.0f > remaining) {
        step      = (f32)((f64)remaining + step);
        remaining = 0.0f;
    }

    vector3d move = references::unk_010f7140.read();

    if (marky_camera_allows(self, stack)) {
        if (self->has_ifc(entity_ifc_standing) && self->is_an_actor() && ((actor*)self)->get_ai_core())
            self->standing_ifc()->set_control_parent(nullptr);

        f32 fraction = (f32)((f64)step / duration);

        move = vector3d((f32)((f64)delta->x * fraction),
                        (f32)((f64)delta->y * fraction),
                        (f32)((f64)delta->z * fraction));

        po       translation = references::unk_010f76b0.read();
        vector4d move_4(move.x, move.y, move.z, 0.0f);

        retail::sub_686C30((u64*)&translation, (u32*)&move_4); // the translation po

        if (self->is_an_actor())
            ((actor*)self)->set_frame_delta(translation, step);

        retail::sub_44E230((u64*)&translation, (f32*)&translation, (f32*)self->my_rel_po);

        if (self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
            self->physical_ifc()->get_biped_system();
        else
            retail::sub_626BE0((i32)self, (u64*)&translation); // entity_base::set_rel_po

        if (self->is_an_entity() && !(self->flags & entity_flag_member)) {
            self->compute_sector(references::g_world_ptr.read()->the_terrain, false, nullptr);
            retail::sub_612EB0((u32*)self, 0); // entity::update_proximity_maps
        }

        if (self->is_an_actor())
            ((actor*)self)->update_colgeom();

        if (self->has_ifc(entity_ifc_player))
            self->player_ifc()->unk_140();
    }

    if ((f64)remaining > 0.0001) {
        duration = remaining;
        delta->x = (f32)((f64)delta->x - move.x);
        delta->y = (f32)((f64)delta->y - move.y);
        delta->z = (f32)((f64)delta->z - move.z);

        return false;
    }

    return true;
}

// sub_846AA0
bool slf__entity__wait_translate_abs__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto  self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto  delta    = (vector3d*)&stack.local_uint(0x04);
    auto &duration = stack.local_num(0x10);

    if (!self)
        return true;

    // the target becomes the offset from where the entity stands
    if (!stack.thread->entry) {
        const vector3d &position = self->my_abs_po->get_position();

        delta->x = (f32)((f64)delta->x - position.x);
        delta->y = (f32)((f64)delta->y - position.y);
        delta->z = (f32)((f64)delta->z - position.z);

        return false;
    }

    f32 step      = get_wait_time_step(self);
    f32 remaining = (f32)((f64)duration - step);

    if (0.0f > remaining) {
        step      = (f32)((f64)remaining + step);
        remaining = 0.0f;
    }

    vector3d move = references::unk_010f7140.read();

    if (marky_camera_allows(self, stack)) {
        if (self->has_ifc(entity_ifc_standing) && self->is_an_actor() && ((actor*)self)->get_ai_core())
            self->standing_ifc()->set_control_parent(nullptr);

        move = *delta;

        if ((f64)duration > 0.0001) {
            f32 fraction = (f32)((f64)step / duration);

            move = vector3d((f32)((f64)delta->x * fraction),
                            (f32)((f64)delta->y * fraction),
                            (f32)((f64)delta->z * fraction));
        }

        po       translation = references::unk_010f76b0.read();
        vector4d move_4(move.x, move.y, move.z, 0.0f);

        retail::sub_686C30((u64*)&translation, (u32*)&move_4); // the translation po

        if (self->is_an_actor())
            ((actor*)self)->set_frame_delta(translation, step);

        retail::sub_44E230((u64*)&translation, (f32*)self->my_rel_po, (f32*)&translation);

        if (self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
            self->physical_ifc()->get_biped_system();
        else
            retail::sub_626BE0((i32)self, (u64*)&translation); // entity_base::set_rel_po

        if (self->is_an_actor())
            ((actor*)self)->update_colgeom();

        retail::sub_796C70(stack.local_uint(0x00)); // add_moved

        if (self->has_ifc(entity_ifc_player))
            self->player_ifc()->unk_140();
    }

    if ((f64)remaining > 0.0001) {
        duration = remaining;
        delta->x = (f32)((f64)delta->x - move.x);
        delta->y = (f32)((f64)delta->y - move.y);
        delta->z = (f32)((f64)delta->z - move.z);

        return false;
    }

    return true;
}

// sub_846EE0
bool slf__entity__get_rel_velocity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto frame = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    vector3d result(0.0f, 0.0f, 0.0f);

    // the velocity turned by the other entity's frame
    if (self && frame) {
        if (self->has_ifc(entity_ifc_physical))
            result = self->physical_ifc()->get_velocity();

        result = frame->my_abs_po->non_affine_slow_xform(result);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8470B0
bool slf__entity__force_region__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto other = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    if (!self || !other || !marky_camera_allows(self, stack))
        return true;

    if (self->is_an_entity() && other->is_an_entity()) {
        self->unforce_regions();
        self->force_regions(other);
    }

    return true;
}

// sub_847140
bool slf__entity__force_current_region__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && marky_camera_allows(self, stack) && self->is_an_entity())
        self->force_current_region();

    return true;
}

// sub_8471B0
bool slf__entity__unforce_regions__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !marky_camera_allows(self, stack) || !self->is_an_entity())
        return true;

    self->unforce_regions();
    self->compute_sector(references::g_world_ptr.read()->the_terrain, false, nullptr);

    return true;
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

// sub_852680
bool slf__entity__set_damage_locked__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto locked = stack.local_num(0x04);

    if (!self)
        return true;

    if (locked <= 0.5f)
        self->ext_flags |= entity_ext_flag_attack_damage_unlocked;
    else
        self->ext_flags &= ~entity_ext_flag_attack_damage_unlocked;

    return true;
}

// sub_8472B0
bool slf__entity__is_invulnerable__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && (self->ext_flags & entity_ext_flag_invulnerable) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_847330
bool slf__entity__is_immune_to_death_from_damage__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->has_ifc(entity_ifc_damage) ? (self->damage_ifc()->flags & damage_ifc_flag_immune_to_death_from_damage ? 1.0f : 0.0f) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8473D0
bool slf__entity__enable_push_away_force_on_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!(self && self->has_ifc(entity_ifc_damage)))
        return true;

    if (enable > 0.0f)
        self->damage_ifc()->ext_flags &= ~damage_ifc_ext_flag_disable_push_away_force_on_damage;
    else
        self->damage_ifc()->ext_flags |= damage_ifc_ext_flag_disable_push_away_force_on_damage;

    return true;
}

// sub_847430
bool slf__entity__enable_die_on_fall_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!(self && self->has_ifc(entity_ifc_damage)))
        return true;

    if (enable > 0.0f)
        self->damage_ifc()->flags |= damage_ifc_flag_die_on_fall_damage;
    else
        self->damage_ifc()->flags &= ~damage_ifc_flag_die_on_fall_damage;

    return true;
}

// sub_8474A0
bool slf__entity__set_invulnerable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    // a vehicle also goes to condition 3 and back, unless it's in 1
    if (self->is_a_vehicle()) {
        auto* car = (vehicle*)self;

        if (on != 0.0f) {
            if (car->condition != 1)
                car->set_condition(3);
        }
        else if (car->condition == 3)
            car->set_condition(0);
    }

    // the hero's are counted, so clearing can't go past them
    if (retail::sub_601C50((u32*)self)) { // entity_base::is_hero
        if (on != 0.0f)
            ++mission_manager::inst()->saved_script_hero_invulnerable_count;
        else {
            if (mission_manager::inst()->saved_script_hero_invulnerable_count <= 0)
                return true;

            --mission_manager::inst()->saved_script_hero_invulnerable_count;
        }
    }

    if (on != 0.0f)
        self->ext_flags |= entity_ext_flag_invulnerable;
    else
        self->ext_flags &= ~entity_ext_flag_invulnerable;

    return true;
}

// sub_847570
bool slf__entity__set_targetting__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    // off makes it a non-target and unbeamable
    if (on == 0.0f)
        self->flags |= entity_flag_nontarget;
    else
        self->flags &= ~entity_flag_nontarget;

    if (on == 0.0f)
        self->flags &= ~entity_flag_beamable;
    else
        self->flags |= entity_flag_beamable;

    return true;
}

// sub_8475E0
bool slf__entity__set_beamable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on == 0.0f)
        self->flags &= ~entity_flag_beamable;
    else
        self->flags |= entity_flag_beamable;

    self->set_flag_recursive(entity_flag_beamable, on != 0.0f);

    return true;
}

// sub_847650
bool slf__entity__set_ai_los_block__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    // off lets the ai see through it
    if (on == 0.0f)
        self->ext_flags |= entity_ext_flag_ai_los_see_through;
    else
        self->ext_flags &= ~entity_ext_flag_ai_los_see_through;

    self->set_ext_flag_recursive(entity_ext_flag_ai_los_see_through, on == 0.0f);

    return true;
}

// sub_8476C0
bool slf__entity__set_shadow_casting__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on > 0.0001f)
        self->ext_flags |= entity_ext_flag_shadow_casting;
    else
        self->ext_flags &= ~entity_ext_flag_shadow_casting;

    return true;
}

// sub_847710
bool slf__entity__is_visible__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_visible() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8477A0
bool slf__entity__set_visible__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    const bool was_visible = self->is_visible();

    self->set_visible(on != 0.0f, false);

    // newly shown entities join the proximity maps
    if (!was_visible && self->is_visible() && self->is_an_entity())
        retail::sub_612EB0((u32*)self, 0); // entity::update_proximity_maps

    return true;
}

// sub_847840
bool slf__entity__set_physical__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->enable(on != 0.0f);

    return true;
}

// sub_8478A0
bool slf__entity__is_auto_activate_prop__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && (self->ext_flags & entity_ext_flag_is_auto_activate_prop) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_847920
bool slf__entity__set_distance_clip__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto distance = stack.local_num(0x04);

    if (!self)
        return true;

    i32 preset = retail::sub_77F090(distance); // the fader preset closest to the distance

    self->ext_flags = preset | (self->ext_flags & ~0xFu);
    retail::sub_6123B0((u32*)self, preset, (i32)i_am_not_sure);

    if (auto* record = (u32*)retail::sub_92BB10(self->my_handle.value)) // tracker record lookup
        retail::sub_905ED0(record, distance);

    return true;
}

// sub_847990
bool slf__entity__set_distance_clip_far__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto distance = stack.local_num(0x04);

    if (!self)
        return true;

    i32 preset = retail::sub_77F090(distance); // the fader preset closest to the distance

    self->ext_flags = preset | (self->ext_flags & ~0xFu);
    retail::sub_6123B0((u32*)self, preset, (i32)i_know_what_i_am_doing);

    if (auto* record = (u32*)retail::sub_92BB10(self->my_handle.value)) // tracker record lookup
        retail::sub_905ED0(record, distance);

    return true;
}

// sub_847A00
bool slf__entity__is_hanging_from_pole__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() && self->get_ai_core() &&
                 (self->get_ai_core()->flags & ai_core_flag_hanging_from_pole) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_847AB0
bool slf__entity__enable_pain_sounds__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!self || !self->has_ifc(entity_ifc_damage))
        return true;

    if (std::fabs(value) > 0.0001f)
        self->damage_ifc()->ext_flags &= ~damage_ifc_ext_flag_disable_pain_sounds;
    else
        self->damage_ifc()->ext_flags |= damage_ifc_ext_flag_disable_pain_sounds;

    return true;
}

// sub_847B30
bool slf__entity__apply_web_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto web_damage = stack.local_num(0x04);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        const vector3d &z_facing = self->my_abs_po->get_z_facing();

        vector3d dir(-z_facing.x, -z_facing.y, -z_facing.z);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage(nullptr,
                                                       nullptr,
                                                       0,
                                                       web_damage,
                                                       8,
                                                       3,
                                                       nullptr,
                                                       nullptr,
                                                       &references::unk_010f7140.get(),
                                                       &dir,
                                                       0,
                                                       &wounded_category,
                                                       &references::unk_010f7140.get(),
                                                       num_rb_phys_bones,
                                                       nullptr,
                                                       nullptr,
                                                       false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_847C30
bool slf__entity__apply_web_damage_with_directed_react__num__num__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self               = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto web_damage         = stack.local_num(0x04);
    auto react_level        = stack.local_num(0x08);
    auto second_react_level = stack.local_num(0x0C);
    auto dir                = (vector3d*)&stack.local_uint(0x10);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        math::vector4 direction(dir->x, dir->y, dir->z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        // the script's vector is normalized in place
        dir->x = unit[0];
        dir->y = unit[1];
        dir->z = unit[2];

        const vector3d &position = self->my_abs_po->get_position();

        vector3d loc(position.x + unit[0], position.y + unit[1], position.z + unit[2]);

        hit_react_record react((i32)react_level, 0.0f);
        hit_react_record second_react((i32)second_react_level, 0.0f);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage(nullptr,
                                                       nullptr,
                                                       0,
                                                       web_damage,
                                                       8,
                                                       3,
                                                       &react,
                                                       &second_react,
                                                       &loc,
                                                       dir,
                                                       0,
                                                       &wounded_category,
                                                       &references::unk_010f7140.get(),
                                                       num_rb_phys_bones,
                                                       nullptr,
                                                       nullptr,
                                                       false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_847E20
bool slf__entity__apply_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage = stack.local_num(0x04);

    if (!self || !self->has_ifc(entity_ifc_damage))
        return true;

    const vector3d &z_facing = self->my_abs_po->get_z_facing();

    vector3d dir(-z_facing.x, -z_facing.y, -z_facing.z);

    string_hash wounded_category;
    wounded_category.initialize(mash::ALLOCATED);

    self->damage_ifc()->apply_damage_with_combo_web_damage(nullptr,
                                                           nullptr,
                                                           (i32)damage,
                                                           8,
                                                           2,
                                                           nullptr,
                                                           nullptr,
                                                           &references::unk_010f7140.get(),
                                                           &dir,
                                                           0,
                                                           &wounded_category,
                                                           &references::unk_010f7140.get(),
                                                           num_rb_phys_bones,
                                                           nullptr,
                                                           nullptr,
                                                           false);

    return true;
}

// sub_847F00
bool slf__entity__apply_damage_with_directed_react__num__num__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self               = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage             = stack.local_num(0x04);
    auto react_level        = stack.local_num(0x08);
    auto second_react_level = stack.local_num(0x0C);
    auto dir                = (vector3d*)&stack.local_uint(0x10);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        math::vector4 direction(dir->x, dir->y, dir->z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        // the script's vector is normalized in place
        dir->x = unit[0];
        dir->y = unit[1];
        dir->z = unit[2];

        const vector3d &position = self->my_abs_po->get_position();

        vector3d loc(position.x + unit[0], position.y + unit[1], position.z + unit[2]);

        hit_react_record react((i32)react_level, 0.0f);
        hit_react_record second_react((i32)second_react_level, 0.0f);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage_with_combo_web_damage(nullptr,
                                                                             nullptr,
                                                                             (i32)damage,
                                                                             8,
                                                                             3,
                                                                             &react,
                                                                             &second_react,
                                                                             &loc,
                                                                             dir,
                                                                             0,
                                                                             &wounded_category,
                                                                             &references::unk_010f7140.get(),
                                                                             num_rb_phys_bones,
                                                                             nullptr,
                                                                             nullptr,
                                                                             false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8480F0
bool slf__entity__apply_damage_with_react__num__num__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self               = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage             = stack.local_num(0x04);
    auto react_level        = stack.local_num(0x08);
    auto second_react_level = stack.local_num(0x0C);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        auto attacker = (entity*)arch_base_vhandle(stack.local_uint(0x10)).resolve();

        vector3d loc;

        // without an attacker entity, the hit comes from one unit along the entity's z facing
        if (attacker && attacker->is_an_entity())
            loc = attacker->my_abs_po->get_position();
        else {
            attacker = nullptr;

            const vector3d &position = self->my_abs_po->get_position();
            const vector3d &z_facing = self->my_abs_po->get_z_facing();

            loc = vector3d(position.x + z_facing.x, position.y + z_facing.y, position.z + z_facing.z);
        }

        const vector3d &position = self->my_abs_po->get_position();

        math::vector4 direction(position.x - loc.x, position.y - loc.y, position.z - loc.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        vector3d dir(unit[0], unit[1], unit[2]);

        hit_react_record react((i32)react_level, 0.0f);
        hit_react_record second_react((i32)second_react_level, 0.0f);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage_with_combo_web_damage(attacker,
                                                                             nullptr,
                                                                             (i32)damage,
                                                                             8,
                                                                             3,
                                                                             &react,
                                                                             &second_react,
                                                                             &loc,
                                                                             &dir,
                                                                             0,
                                                                             &wounded_category,
                                                                             &references::unk_010f7140.get(),
                                                                             num_rb_phys_bones,
                                                                             nullptr,
                                                                             nullptr,
                                                                             false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_848380
bool slf__entity__apply_damage_with_react_and_scales__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage = stack.local_num(0x04);

    // the second react's arguments default to the first's
    i32 optional_count = count - 2;

    i32 react_level        = (i32)(optional_count > 0 ? stack.local_num(0x0C) : 0.0f);
    f32 scale_0            = optional_count > 1 ? stack.local_num(0x10) : 1.0f;
    f32 scale_1            = optional_count > 2 ? stack.local_num(0x14) : 1.0f;
    f32 scale_2            = optional_count > 3 ? stack.local_num(0x18) : 1.0f;
    f32 unk_04             = optional_count > 4 ? stack.local_num(0x1C) : 0.0f;
    i32 second_react_level = (i32)(optional_count > 5 ? stack.local_num(0x20) : (f32)react_level);
    f32 second_scale_0     = optional_count > 6 ? stack.local_num(0x24) : scale_0;
    f32 second_scale_1     = optional_count > 7 ? stack.local_num(0x28) : scale_1;
    f32 second_scale_2     = optional_count > 8 ? stack.local_num(0x2C) : scale_2;
    f32 second_unk_04      = optional_count > 9 ? stack.local_num(0x30) : 0.0f;

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        auto attacker = (entity*)arch_base_vhandle(stack.local_uint(0x08)).resolve();

        vector3d loc;

        // without an attacker entity, the hit comes from one unit along the entity's z facing
        if (attacker && attacker->is_an_entity())
            loc = attacker->my_abs_po->get_position();
        else {
            attacker = nullptr;

            const vector3d &position = self->my_abs_po->get_position();
            const vector3d &z_facing = self->my_abs_po->get_z_facing();

            loc = vector3d(position.x + z_facing.x, position.y + z_facing.y, position.z + z_facing.z);
        }

        const vector3d &position = self->my_abs_po->get_position();

        math::vector4 direction(position.x - loc.x, position.y - loc.y, position.z - loc.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        vector3d dir(unit[0], unit[1], unit[2]);

        hit_react_record react(react_level, unk_04, scale_0, scale_1, scale_2);
        hit_react_record second_react(second_react_level, second_unk_04, second_scale_0, second_scale_1, second_scale_2);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage_with_combo_web_damage(attacker,
                                                                             nullptr,
                                                                             (i32)damage,
                                                                             8,
                                                                             3,
                                                                             &react,
                                                                             &second_react,
                                                                             &loc,
                                                                             &dir,
                                                                             0,
                                                                             &wounded_category,
                                                                             &references::unk_010f7140.get(),
                                                                             num_rb_phys_bones,
                                                                             nullptr,
                                                                             nullptr,
                                                                             false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_848800
bool slf__entity__apply_damage_with_react_at_pos__num__num__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x20;

    auto self               = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage             = stack.local_num(0x04);
    auto react_level        = stack.local_num(0x08);
    auto second_react_level = stack.local_num(0x0C);
    auto position           = (const vector3d*)&stack.local_uint(0x10);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        auto attacker = (entity*)arch_base_vhandle(stack.local_uint(0x1C)).resolve();

        if (attacker && !attacker->is_an_entity())
            attacker = nullptr;

        const vector3d &abs_position = self->my_abs_po->get_position();

        math::vector4 direction(abs_position.x - position->x,
                                abs_position.y - position->y,
                                abs_position.z - position->z,
                                0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        vector3d dir(unit[0], unit[1], unit[2]);

        hit_react_record react((i32)react_level, 0.0f);
        hit_react_record second_react((i32)second_react_level, 0.0f);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage_with_combo_web_damage(attacker,
                                                                             nullptr,
                                                                             (i32)damage,
                                                                             8,
                                                                             3,
                                                                             &react,
                                                                             &second_react,
                                                                             position,
                                                                             &dir,
                                                                             0,
                                                                             &wounded_category,
                                                                             &references::unk_010f7140.get(),
                                                                             num_rb_phys_bones,
                                                                             nullptr,
                                                                             nullptr,
                                                                             false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_848A10
bool slf__entity__apply_damage_with_react_and_scales_at_pos__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self, the vector as one
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 3));

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage   = stack.local_num(0x04);
    auto position = (const vector3d*)&stack.local_uint(0x08);

    // the second react's arguments default to the first's
    i32 optional_count = count - 3;

    i32 react_level        = (i32)(optional_count > 0 ? stack.local_num(0x18) : 0.0f);
    f32 scale_0            = optional_count > 1 ? stack.local_num(0x1C) : 1.0f;
    f32 scale_1            = optional_count > 2 ? stack.local_num(0x20) : 1.0f;
    f32 scale_2            = optional_count > 3 ? stack.local_num(0x24) : 1.0f;
    f32 unk_04             = optional_count > 4 ? stack.local_num(0x28) : 0.0f;
    i32 second_react_level = (i32)(optional_count > 5 ? stack.local_num(0x2C) : (f32)react_level);
    f32 second_scale_0     = optional_count > 6 ? stack.local_num(0x30) : scale_0;
    f32 second_scale_1     = optional_count > 7 ? stack.local_num(0x34) : scale_1;
    f32 second_scale_2     = optional_count > 8 ? stack.local_num(0x38) : scale_2;
    f32 second_unk_04      = optional_count > 9 ? stack.local_num(0x3C) : 0.0f;

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        auto attacker = (entity*)arch_base_vhandle(stack.local_uint(0x14)).resolve();

        if (attacker && !attacker->is_an_entity())
            attacker = nullptr;

        const vector3d &abs_position = self->my_abs_po->get_position();

        math::vector4 direction(abs_position.x - position->x,
                                abs_position.y - position->y,
                                abs_position.z - position->z,
                                0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        vector3d dir(unit[0], unit[1], unit[2]);

        hit_react_record react(react_level, unk_04, scale_0, scale_1, scale_2);
        hit_react_record second_react(second_react_level, second_unk_04, second_scale_0, second_scale_1, second_scale_2);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage_with_combo_web_damage(attacker,
                                                                             nullptr,
                                                                             (i32)damage,
                                                                             8,
                                                                             3,
                                                                             &react,
                                                                             &second_react,
                                                                             position,
                                                                             &dir,
                                                                             0,
                                                                             &wounded_category,
                                                                             &references::unk_010f7140.get(),
                                                                             num_rb_phys_bones,
                                                                             nullptr,
                                                                             nullptr,
                                                                             false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_848E20
bool slf__entity__apply_damage_with_react_and_scales_at_pos_no_physics__num__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self, the vector as one
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 3));

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage   = stack.local_num(0x04);
    auto position = (const vector3d*)&stack.local_uint(0x08);

    // the second react's arguments default to the first's
    i32 optional_count = count - 3;

    i32 react_level        = (i32)(optional_count > 0 ? stack.local_num(0x18) : 0.0f);
    f32 scale_0            = optional_count > 1 ? stack.local_num(0x1C) : 1.0f;
    f32 scale_1            = optional_count > 2 ? stack.local_num(0x20) : 1.0f;
    f32 scale_2            = optional_count > 3 ? stack.local_num(0x24) : 1.0f;
    f32 unk_04             = optional_count > 4 ? stack.local_num(0x28) : 0.0f;
    i32 second_react_level = (i32)(optional_count > 5 ? stack.local_num(0x2C) : (f32)react_level);
    f32 second_scale_0     = optional_count > 6 ? stack.local_num(0x30) : scale_0;
    f32 second_scale_1     = optional_count > 7 ? stack.local_num(0x34) : scale_1;
    f32 second_scale_2     = optional_count > 8 ? stack.local_num(0x38) : scale_2;
    f32 second_unk_04      = optional_count > 9 ? stack.local_num(0x3C) : 0.0f;

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        auto attacker = (entity*)arch_base_vhandle(stack.local_uint(0x14)).resolve();

        if (attacker && !attacker->is_an_entity())
            attacker = nullptr;

        const vector3d &abs_position = self->my_abs_po->get_position();

        math::vector4 direction(abs_position.x - position->x,
                                abs_position.y - position->y,
                                abs_position.z - position->z,
                                0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        vector3d dir(unit[0], unit[1], unit[2]);

        hit_react_record react(react_level, unk_04, scale_0, scale_1, scale_2);
        hit_react_record second_react(second_react_level, second_unk_04, second_scale_0, second_scale_1, second_scale_2);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage_with_combo_web_damage(attacker,
                                                                             nullptr,
                                                                             (i32)damage,
                                                                             8,
                                                                             3,
                                                                             &react,
                                                                             &second_react,
                                                                             position,
                                                                             &dir,
                                                                             0x10,
                                                                             &wounded_category,
                                                                             &references::unk_010f7140.get(),
                                                                             num_rb_phys_bones,
                                                                             nullptr,
                                                                             nullptr,
                                                                             false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_849230
bool slf__entity__apply_melee_damage__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage = stack.local_num(0x04);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_damage)) {
        auto attacker = (entity*)arch_base_vhandle(stack.local_uint(0x08)).resolve();

        vector3d loc;

        // without an attacker entity, the hit comes from one unit along the entity's z facing
        if (attacker && attacker->is_an_entity())
            loc = attacker->my_abs_po->get_position();
        else {
            attacker = nullptr;

            const vector3d &position = self->my_abs_po->get_position();
            const vector3d &z_facing = self->my_abs_po->get_z_facing();

            loc = vector3d(position.x + z_facing.x, position.y + z_facing.y, position.z + z_facing.z);
        }

        const vector3d &position = self->my_abs_po->get_position();

        math::vector4 direction(position.x - loc.x, position.y - loc.y, position.z - loc.z, 0.0f);
        math::vector4 unit;

        retail::sub_401AA0((u64*)&unit, (f32*)&direction);

        vector3d dir(unit[0], unit[1], unit[2]);

        string_hash wounded_category;
        wounded_category.initialize(mash::ALLOCATED);

        result = (f32)self->damage_ifc()->apply_damage_with_combo_web_damage(attacker,
                                                                             nullptr,
                                                                             (i32)damage,
                                                                             8,
                                                                             3,
                                                                             nullptr,
                                                                             nullptr,
                                                                             &loc,
                                                                             &dir,
                                                                             0,
                                                                             &wounded_category,
                                                                             &references::unk_010f7140.get(),
                                                                             num_rb_phys_bones,
                                                                             nullptr,
                                                                             nullptr,
                                                                             false);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_849440
bool slf__entity__apply_explosive_damage__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage   = stack.local_num(0x04);
    auto position = (const vector3d*)&stack.local_uint(0x08);

    // the hero gets a react, anyone else a plain explosive hit
    bool hero = retail::sub_601C50((u32*)self); // entity_base::is_hero

    if (!self || !self->has_ifc(entity_ifc_damage))
        return true;

    const vector3d &abs_position = self->my_abs_po->get_position();

    vector3d dir(abs_position.x - position->x, abs_position.y - position->y, abs_position.z - position->z);

    retail::sub_402050((f32*)&dir); // vector3d::normalize

    string_hash wounded_category;

    if (hero) {
        hit_react_record react(6, 0.0f, 1.0f, 1.0f, 1.0f);
        hit_react_record second_react(6, 0.0f, 1.0f, 1.0f, 1.0f);

        wounded_category.initialize(mash::ALLOCATED);

        self->damage_ifc()->apply_damage_with_combo_web_damage(nullptr,
                                                               nullptr,
                                                               (i32)damage,
                                                               8,
                                                               3,
                                                               &react,
                                                               &second_react,
                                                               position,
                                                               &dir,
                                                               0,
                                                               &wounded_category,
                                                               &references::unk_010f7140.get(),
                                                               num_rb_phys_bones,
                                                               nullptr,
                                                               nullptr,
                                                               false);
    } else {
        wounded_category.initialize(mash::ALLOCATED);

        self->damage_ifc()->apply_damage_with_combo_web_damage(nullptr,
                                                               nullptr,
                                                               (i32)damage,
                                                               8,
                                                               4,
                                                               nullptr,
                                                               nullptr,
                                                               position,
                                                               &dir,
                                                               0,
                                                               &wounded_category,
                                                               &references::unk_010f7140.get(),
                                                               num_rb_phys_bones,
                                                               nullptr,
                                                               nullptr,
                                                               false);
    }

    return true;
}

// sub_849710
bool slf__entity__attach_to_wall__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto position = (const vector3d*)&stack.local_uint(0x04);

    if (!self->has_ifc(entity_ifc_player) || !retail::sub_601C50((u32*)self))
        return true;

    retail::sub_967860((u32*)references::g_world_ptr.read(), (i32)position, 0, 1); // world_dynamics_system::place_hero_at
    retail::sub_77BAB0((u32*)references::g_world_ptr.read()->the_terrain);

    references::g_world_ptr.read()->get_hero_ptr()->player_ifc()->unk_140();

    self->player_ifc()->unk_486(true, 0.0f);

    if (self->flags & entity_flag_suspended)
        self->player_ifc()->set_desired_mode(2);
    else {
        self->suspend(true);
        self->player_ifc()->set_desired_mode(2);
        self->unsuspend(true);
    }

    return true;
}

// sub_849820
bool slf__entity__attach_to_wall__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self->has_ifc(entity_ifc_player))
        return true;

    self->player_ifc()->unk_486(true, 0.0f);

    if (self->flags & entity_flag_suspended)
        self->player_ifc()->set_desired_mode(2);
    else {
        self->suspend(true);
        self->player_ifc()->set_desired_mode(2);
        self->unsuspend(true);
    }

    return true;
}

// sub_8498D0
bool slf__entity__apply_explosive_damage__num__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x20;

    auto self         = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage       = stack.local_num(0x04);
    auto position     = (const vector3d*)&stack.local_uint(0x08);
    auto override_dir = (const vector3d*)&stack.local_uint(0x14);

    // the hero gets a react, anyone else a plain explosive hit
    bool hero = retail::sub_601C50((u32*)self); // entity_base::is_hero

    if (!self || !self->has_ifc(entity_ifc_damage))
        return true;

    const vector3d &abs_position = self->my_abs_po->get_position();

    vector3d dir(abs_position.x - position->x, abs_position.y - position->y, abs_position.z - position->z);

    retail::sub_402050((f32*)&dir); // vector3d::normalize

    string_hash wounded_category;

    if (hero) {
        hit_react_record react(6, 0.0f, 1.0f, 1.0f, 1.0f);
        hit_react_record second_react(6, 0.0f, 1.0f, 1.0f, 1.0f);

        wounded_category.initialize(mash::ALLOCATED);

        self->damage_ifc()->apply_damage_with_combo_web_damage(nullptr,
                                                               nullptr,
                                                               (i32)damage,
                                                               8,
                                                               3,
                                                               &react,
                                                               &second_react,
                                                               position,
                                                               &dir,
                                                               0,
                                                               &wounded_category,
                                                               &references::unk_010f7140.get(),
                                                               num_rb_phys_bones,
                                                               nullptr,
                                                               nullptr,
                                                               false);
    } else {
        wounded_category.initialize(mash::ALLOCATED);

        self->damage_ifc()->apply_damage_with_combo_web_damage(nullptr,
                                                               nullptr,
                                                               (i32)damage,
                                                               8,
                                                               4,
                                                               nullptr,
                                                               nullptr,
                                                               position,
                                                               &dir,
                                                               0,
                                                               &wounded_category,
                                                               override_dir,
                                                               num_rb_phys_bones,
                                                               nullptr,
                                                               nullptr,
                                                               false);
    }

    return true;
}

// sub_849B90
bool slf__entity__get_last_entity_that_damaged_me__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result;

    if (self && self->has_ifc(entity_ifc_damage))
        if (entity* attacker = self->damage_ifc()->get_damage_attacker())
            result = attacker->my_handle;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_849C00
bool slf__entity__set_damage_scale__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto type  = stack.local_num(0x04);
    auto scale = stack.local_num(0x08);

    if (self && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_damage_scale((i32)type, scale);

    return true;
}

// sub_849C50
bool slf__entity__set_damage_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto scale = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_damage_scale(scale);

    return true;
}

// sub_849C90
bool slf__entity__get_damage_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto type = stack.local_num(0x04);

    f32 result = self && self->has_ifc(entity_ifc_damage) ? self->damage_ifc()->get_damage_scale((i32)type) : 1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_849D10
bool slf__entity__was_occluded_last_frame__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->was_occluded_last_frame() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_849DB0
bool slf__entity__snap_to__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    if (!self || !target || !marky_camera_allows(self, stack))
        return true;

    if (self->has_ifc(entity_ifc_standing) && self->is_an_actor() && ((actor*)self)->get_ai_core())
        self->standing_ifc()->set_control_parent(nullptr);

    if (self->is_an_actor())
        ((actor*)self)->invalidate_frame_delta();

    // a biped under physics stays put
    if (self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
        self->physical_ifc()->get_biped_system();
    else {
        retail::sub_626BE0((i32)self, (u64*)target->my_abs_po); // entity_base::set_rel_po

        update_als_facing(self);
    }

    if (self->is_an_entity()) {
        self->compute_sector(references::g_world_ptr.read()->the_terrain, false, nullptr);
        retail::sub_612EB0((u32*)self, 0); // entity::update_proximity_maps
    }

    if (self->is_an_actor()) {
        ((actor*)self)->update_colgeom();
        ((actor*)self)->save_last_collision_free_state();
    }

    if (self->has_ifc(entity_ifc_player))
        self->player_ifc()->unk_140();

    return true;
}

// sub_849F90
bool slf__entity__abs_snap_to__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    if (!self || !target || !marky_camera_allows(self, stack))
        return true;

    if (self->has_ifc(entity_ifc_standing) && self->is_an_actor() && ((actor*)self)->get_ai_core())
        self->standing_ifc()->set_control_parent(nullptr);

    if (self->is_an_actor())
        ((actor*)self)->invalidate_frame_delta();

    if (self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))
        self->physical_ifc()->get_biped_system();
    else {
        retail::sub_627180((i32)self, (f32*)target->my_abs_po); // entity_base::set_abs_po

        update_als_facing(self);
    }

    if (self->is_an_entity()) {
        self->compute_sector(references::g_world_ptr.read()->the_terrain, false, nullptr);
        retail::sub_612EB0((u32*)self, 0); // entity::update_proximity_maps
    }

    if (self->is_an_actor()) {
        ((actor*)self)->update_colgeom();
        ((actor*)self)->save_last_collision_free_state();
    }

    if (self->has_ifc(entity_ifc_player))
        self->player_ifc()->unk_140();

    return true;
}

// sub_84A170
bool slf__entity__camera_set_target__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (const vector3d*)&stack.local_uint(0x04);

    if (self && self->is_a_marky_camera() && marky_camera_allows(self, stack))
        ((marky_camera*)self)->camera_set_target(*target);

    return true;
}

// sub_84A1E0
bool slf__entity__camera_get_target__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self && self->is_a_marky_camera() && marky_camera_allows(self, stack) ?
        ((marky_camera*)self)->camera_get_target() :
        references::unk_010f7140.read();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84A290
bool slf__entity__camera_set_roll__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto angle = stack.local_num(0x04);

    if (self && self->is_a_marky_camera() && marky_camera_allows(self, stack))
        ((marky_camera*)self)->camera_set_roll(angle);

    return true;
}

// sub_84A300
bool slf__entity__wait_camera_set_roll__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    struct wait_state {
        f32 start;
        f32 elapsed;
    };

    auto* state = pop_wait_arguments<wait_state>(stack, 0x0C);

    // no marky camera test
    auto self     = (marky_camera*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto angle    = stack.local_num(0x04);
    auto duration = stack.local_num(0x08);

    if (!self)
        return true;

    if (!stack.thread->entry) {
        if (duration <= 0.0001) {
            self->camera_set_roll(angle);

            return true;
        }

        state->start   = self->roll;
        state->elapsed = 0.0f;

        return false;
    }

    state->elapsed += get_wait_time_step(self);

    f32 t = (f32)((f64)state->elapsed / duration);

    if (t > 1.0)
        t = 1.0f;

    self->camera_set_roll((f32)(((f64)angle - state->start) * t + state->start));

    if (state->elapsed >= duration)
        return true;

    return false;
}

// sub_84A500
bool slf__entity__camera_orbit__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto center = (const vector3d*)&stack.local_uint(0x04);
    auto range  = stack.local_num(0x10);
    auto theta  = stack.local_num(0x14);
    auto psi    = stack.local_num(0x18);

    if (self && self->is_a_marky_camera() && marky_camera_allows(self, stack))
        ((marky_camera*)self)->camera_orbit(*center, range, theta, psi);

    return true;
}

// sub_84A590
bool slf__entity__get_member__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    if (self) {
        if (self->is_a_conglomerate()) {
            string_hash member_name;
            member_name.initialize(mash::ALLOCATED, name);

            if (entity* member = self->get_member(self->get_member_index(member_name)))
                result = member->my_handle;
        } else
            self->is_a_conglomerate_clone(); // the answer is dropped
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84A640
bool slf__entity__has_member__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self && self->is_a_conglomerate()) {
        string_hash member_name;
        member_name.initialize(mash::ALLOCATED, name);

        result = self->has_member(member_name) ? 1.0f : 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84A6E0
bool slf__entity__get_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    if (self) {
        if (self->is_a_conglomerate()) {
            string_hash member_name;
            member_name.initialize(mash::ALLOCATED, name);

            auto member = (entity*)retail::sub_655D80((i32)self, self->get_member_index(member_name)); // conglomerate::get_member_for_linking

            if (member)
                result = member->my_handle;
        } else
            self->is_a_conglomerate_clone(); // the answer is dropped
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84A790
bool slf__entity__get_trigger_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    if (self) {
        if (self->is_a_conglomerate()) {
            string_hash member_name;
            member_name.initialize(mash::ALLOCATED, name);

            auto member = (entity*)retail::sub_655D80((i32)self, self->get_member_index(member_name)); // conglomerate::get_member_for_linking

            if (member) {
                auto found = ((trigger* (__thiscall*)(trigger_manager*, entity*))retail::sub_72D5F0)(trigger_manager::inst(), member); // trigger_manager::find_instance

                if (found)
                    result = found->my_handle;
            }
        } else
            self->is_a_conglomerate_clone(); // the answer is dropped
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84A850
bool slf__entity__has_member_for_linking__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self && self->is_a_conglomerate()) {
        string_hash member_name;
        member_name.initialize(mash::ALLOCATED, name);

        result = self->has_member_for_linking(member_name) ? 1.0f : 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84A8F0
bool slf__entity__get_member_abs_position__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    vector3d result;

    if (self && self->is_a_conglomerate()) {
        string_hash member_name;
        member_name.initialize(mash::ALLOCATED, name);

        // an unknown member leaves the result unset
        retail::sub_62FA60((i32)self, self->get_member_index(member_name), (f32*)&result, 0); // conglomerate::get_member_abs_position
    } else {
        if (self)
            self->is_a_conglomerate_clone(); // the answer is dropped

        result = references::unk_010f7140.read();
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84A9B0
bool slf__entity__get_target_member__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self     = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto position = (const vector3d*)&stack.local_uint(0x04);

    arch_base_vhandle result;

    if (self && self->is_a_conglomerate()) {
        auto member = ((entity* (__thiscall*)(conglomerate*, vector3d, bool))retail::sub_65B8C0)(self, *position, true); // get_target_member

        if (member)
            result = member->my_handle;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84ABE0
bool slf__entity__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto name = (const char*)stack.local_uint(0x00);

    arch_base_vhandle result;

    {
        native_resource_context context(stack);

        string_hash instance_name = next_script_ent_name(references::script_ent_pool, references::script_ent_pool_guard);

        auto spawned = ((entity* (__cdecl*)(mash::string, string_hash, bool))retail::sub_83DC00)(mash::string(name), instance_name, false); // spawn by name

        if (spawned) {
            result = spawned->my_handle;

            stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, result.value);
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84AD80
bool slf__entity__entity__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto name     = (const char*)stack.local_uint(0x00);
    auto instance = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    {
        native_resource_context context(stack);

        string_hash  instance_name;
        string_hash* instance_slot = &instance_name;

        // a name ending in '*' takes the next name from that prefix's pool
        __asm {
            push instance
            mov  esi, instance_slot
            call retail::sub_8238E0
            add  esp, 4
        }

        auto spawned = ((entity* (__cdecl*)(mash::string, string_hash, bool))retail::sub_83DC00)(mash::string(name), instance_name, false); // spawn by name

        if (spawned) {
            result = spawned->my_handle;

            stack.thread->get_instance(-1)->add_allocated_stuff(chuck::vm::script_garbage_collection_entity, result.value);
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B5B0
bool slf__entity__suspend__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_an_entity() || (self->flags & entity_flag_suspended))
        return true;

    // the hero's are counted, so unsuspend can't go past them
    if (retail::sub_601C50((u32*)self)) // entity_base::is_hero
        ++mission_manager::inst()->saved_script_hero_suspend_count;

    self->suspend(true);

    return true;
}

// sub_84B610
bool slf__entity__is_suspended__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && (self->flags & entity_flag_suspended) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B690
bool slf__entity__disable_collisions__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_actor())
        self->set_collisions_active(false, true);

    return true;
}

// sub_84B6D0
bool slf__entity__enable_collisions__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_actor())
        self->set_collisions_active(true, true);

    return true;
}

// sub_84B710
bool slf__entity__add_collision_ignorance__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto other = arch_base_vhandle(stack.local_uint(0x04)).resolve();

    if (self && other && self->is_an_actor() && other->is_an_actor())
        retail::sub_65B290((u32**)self, (i32)other->my_handle.value); // actor::add_collision_ignorance

    return true;
}

// sub_84B770
bool slf__entity__remove_collision_ignorance__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto other = arch_base_vhandle(stack.local_uint(0x04)).resolve();

    if (self && other && self->is_an_actor() && other->is_an_actor())
        retail::sub_64B1D0((u32*)self, (i32)other->my_handle.value); // actor::remove_collision_ignorance

    return true;
}

// sub_84B7D0
bool slf__entity__unsuspend__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_an_entity())
        return true;

    if (retail::sub_601C50((u32*)self)) { // entity_base::is_hero
        if (mission_manager::inst()->saved_script_hero_suspend_count <= 0)
            return true;

        --mission_manager::inst()->saved_script_hero_suspend_count;
    }

    self->unsuspend(true);

    return true;
}

// sub_84B830
bool slf__entity__get_district_id__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    string_hash result;

    if (self && self->is_an_actor()) {
        using find_district_for_point_method = region* (__thiscall*)(terrain* self, const vector3d* point, region* primary);

        region* primary  = self->get_primary_region();
        region* district = ((find_district_for_point_method)retail::sub_789CC0)(references::g_world_ptr.read()->the_terrain,
                                                                                &self->my_abs_po->get_position(),
                                                                                primary);

        if (district)
            result = district->get_district_id();
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84B950
bool slf__entity__als_play_animation_by_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_an_actor() || self->get_ai_core())
        return true;

    als::animation_logic_system_interface* als = self->get_als();

    if (!als)
        return true;

    f32 force = count - 1 > 0 ? stack.local_num(0x08) : 0.0f;

    string_hash category;
    category.initialize(mash::ALLOCATED, (const char*)stack.local_uint(0x04));

    // the base layer's own category only plays again while its unk_045 allows it
    if (force > 0.5f ||
        !als->get_als_layer(als::base_layer) ||
        als->get_als_layer(als::base_layer)->get_category_id() != category ||
        (als->get_als_layer(als::base_layer)->unk_045() && als->get_als_layer(als::base_layer)->get_category_id() == category)) {

        als->suspend_logic_system(false);
        request_category_transition_helper(category, als::base_layer, als, true, false);
    }

    return true;
}

// sub_84BAB0
bool slf__entity__als_play_animation_by_filename__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto filename   = (const char*)stack.local_uint(0x04);
    auto use_prefix = stack.local_num(0x08);

    if (!self || !self->is_an_actor() || self->get_ai_core())
        return true;

    if (!self->has_ifc(entity_ifc_script_data))
        self->create_script_data_ifc();

    script_data_interface* script_data = self->script_data_ifc();

    string_hash parameter;
    parameter.initialize(mash::ALLOCATED, "als_anim_filename");

    script_data->data.set_pb_fixedstring(parameter, filename);
    script_data->set_als_anim_filename_use_prefix(use_prefix > 0.0f);

    als::animation_logic_system_interface* als = self->get_als();

    if (!als)
        return true;

    als->suspend_logic_system(false);

    string_hash category;
    category.initialize(mash::ALLOCATED, "Generic_Play_Filename");

    request_category_transition_helper(category, als::base_layer, als, true, false);

    return true;
}

// sub_84BBD0
bool slf__entity__update_member_visible__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self    = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name    = (const char*)stack.local_uint(0x04);
    auto visible = stack.local_num(0x08);

    if (!self)
        return true;

    string_hash member_name;

    if (self->is_a_dynamic_conglomerate_clone()) {
        member_name.initialize(mash::ALLOCATED, name);
        retail::sub_6543F0((u32*)self, &member_name.source_hash_code, visible != 0.0f); // the clone's member visibility
    } else if (self->is_a_conglomerate()) {
        member_name.initialize(mash::ALLOCATED, name);

        entity* member = ((conglomerate*)self)->get_member(((conglomerate*)self)->get_member_index(member_name));

        member->set_visible(visible != 0.0f, false);
    }

    return true;
}

// sub_84BCC0
bool slf__entity__update_member_color__str__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto rgb   = (const vector3d*)&stack.local_uint(0x08);
    auto alpha = stack.local_num(0x14);

    if (!self)
        return true;

    string_hash member_name;
    member_name.initialize(mash::ALLOCATED, name);

    entity* member = nullptr;
    color32 color;

    // the current colour is read, then every channel overwritten
    if (self->is_a_dynamic_conglomerate_clone()) {
        color32 current;
        color = *(color32*)retail::sub_6399E0((u32*)self, (u8*)&current, &member_name.source_hash_code); // the clone's member colour
    } else {
        member = ((conglomerate*)self)->get_member(((conglomerate*)self)->get_member_index(member_name));

        if (member->is_an_actor())
            color = member->get_render_color();
    }

    color.set_red_float  (rgb->x);
    color.set_green_float(rgb->y);
    color.set_blue_float (rgb->z);
    color.set_alpha_float(alpha);

    if (self->is_a_dynamic_conglomerate_clone())
        retail::sub_639950((u32*)self, &member_name.source_hash_code, (u32*)&color); // the clone's member colour
    else if (self->is_a_conglomerate() && member->is_an_actor())
        member->set_render_color(color);

    return true;
}

// sub_84BE70
bool slf__entity__wait_update_member_color__str__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    struct wait_state {
        entity* member;
        color   start;
        f32     elapsed;
    };

    auto* state = pop_wait_arguments<wait_state>(stack, 0x1C);

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name     = (const char*)stack.local_uint(0x04);
    auto target   = (const color*)&stack.local_uint(0x08);
    auto duration = stack.local_num(0x18);

    if (!self)
        return true;

    // the conglomerate test's answer is dropped
    if (!self->is_a_dynamic_conglomerate_clone())
        self->is_a_conglomerate();

    if (!stack.thread->entry) {
        string_hash member_name;
        member_name.initialize(mash::ALLOCATED, name);

        // a member that isn't an actor starts from the name hash's bytes
        auto member_color = std::bit_cast<color32>(member_name);

        if (self->is_a_dynamic_conglomerate_clone()) {
            state->member = (entity*)retail::sub_639A90((u32*)self, &member_name.source_hash_code); // the clone's member

            color32 current;
            member_color = *(color32*)retail::sub_62A580((u32*)self, (i32)&current, (i32)state->member); // the clone's member colour
        } else {
            state->member = ((conglomerate*)self)->get_member(((conglomerate*)self)->get_member_index(member_name));

            if (state->member->is_an_actor())
                member_color = state->member->get_render_color();
        }

        state->elapsed = 0.0f;
        state->start.r = (f32)(member_color.r / 255.0);
        state->start.g = (f32)(member_color.g / 255.0);
        state->start.b = (f32)(member_color.b / 255.0);
        state->start.a = (f32)(member_color.a / 255.0);

        return false;
    }

    state->elapsed += get_wait_time_step(self);

    if (state->elapsed > duration)
        state->elapsed = duration;

    // a negative target channel keeps its start
    color current;

    if (duration == 0.0f) {
        current.r = target->r >= 0.0f ? target->r : state->start.r;
        current.g = target->g >= 0.0f ? target->g : state->start.g;
        current.b = target->b >= 0.0f ? target->b : state->start.b;
        current.a = target->a >= 0.0f ? target->a : state->start.a;
    } else {
        f32 t = state->elapsed / duration;

        current.r = state->start.r + (target->r >= 0.0f ? (target->r - state->start.r) * t : 0.0f);
        current.g = state->start.g + (target->g >= 0.0f ? (target->g - state->start.g) * t : 0.0f);
        current.b = state->start.b + (target->b >= 0.0f ? (target->b - state->start.b) * t : 0.0f);
        current.a = state->start.a + (target->a >= 0.0f ? (target->a - state->start.a) * t : 0.0f);
    }

    if (current.r > 1.0f)
        current.r = 1.0f;

    if (current.g > 1.0f)
        current.g = 1.0f;

    if (current.b > 1.0f)
        current.b = 1.0f;

    if (current.a > 1.0f)
        current.a = 1.0f;

    if (self->is_a_dynamic_conglomerate_clone()) {
        color32 packed(current);
        retail::sub_6153D0((u32*)self, (i32)state->member, (u32*)&packed); // the clone's member colour
    } else if (state->member->is_an_actor())
        state->member->set_render_color(color32(current));

    if (duration > state->elapsed)
        return false;

    return true;
}

// sub_84C2F0
bool slf__entity__particle_generator_set_scalar__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // asked and ignored
    if (self)
        self->is_a_particle_generator();

    return true;
}

// sub_84C310
bool slf__entity__get_enx_script_instance__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    advanced_entity_ptrs*       ptrs   = self && self->is_an_actor() ? self->adv_ptrs : nullptr;
    chuck::vm::script_instance* result = ptrs ? ptrs->my_script : nullptr;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84C3A0
bool slf__entity__get_enx_script_object_name__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    advanced_entity_ptrs* ptrs   = self && self->is_an_actor() ? self->adv_ptrs : nullptr;
    string_hash           result = ptrs ? ptrs->script_obj_name : string_hash();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84C410
bool slf__entity__get_enx_script_exec_name__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    advanced_entity_ptrs* ptrs = self && self->is_an_actor() ? self->adv_ptrs : nullptr;

    string_hash result;

    if (ptrs)
        retail::sub_5FD120((u32*)ptrs->script_exe_name, &result.source_hash_code); // resource_key::get_hash

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84C490
bool slf__entity__is_enx_script_related__uint__t::operator()(chuck::vm::vm_stack &stack) { // whether the enx instance's object is or derives from the named one
    stack.sp -= 0x08;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto hash = stack.local_uint(0x04);

    advanced_entity_ptrs*       ptrs     = self && self->is_an_actor() ? self->adv_ptrs : nullptr;
    chuck::vm::script_instance* instance = ptrs ? ptrs->my_script : nullptr;

    f32 result = 0.0f;

    if (instance) {
        string_hash name;
        name.initialize(mash::ALLOCATED, nullptr, hash);

        for (chuck::vm::script_object* object = instance->parent; object; object = object->parent_object) {
            string_hash object_name;
            retail::sub_5FD120((u32*)object, &object_name.source_hash_code); // script_object::get_name

            if (object_name == name) {
                result = 1.0f;
                break;
            }
        }
    }

    stack.push(&result, sizeof(result));

    return true;
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

// sub_84D9B0
bool slf__entity__get_ai_idle_type__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    // the receiver resolves twice, the second answer is used
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = core ? (f32)core->idle_type : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84DA10
bool slf__entity__set_ai_idle_type__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // same
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self      = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto idle_type = stack.local_num(0x04);

    if (!self)
        return true;

    if (ai_core* core = self->get_ai_core())
        core->idle_type = (i32)idle_type;

    return true;
}

// sub_84DA50
bool slf__entity__has_inode__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // same
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        string_hash inode_name;
        inode_name.initialize(mash::ALLOCATED, name);

        if (core->get_info_node(get_info_node_type(inode_name)))
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
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

// sub_84E6C0
bool slf__entity__push_ai_base_machine_immediately__entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // the script's instance lock and the machine lock are let go while the machine changes
    ref_unlock_scope instance_unlock(stack.thread->inst->parent->instance_lock);
    ref_unlock_scope machine_unlock(&jq::references::unk_010f7748.get());

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto other      = arch_base_vhandle(stack.local_uint(0x04));
    auto graph_name = (const char*)stack.local_uint(0x08);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        string_hash name;
        name.initialize(mash::ALLOCATED, graph_name);

        resource_key graph;
        graph.set(name, (u32)amalga::e_resource_type::ai_state_graph);

        // the other entity's core, or this one's
        auto*    other_actor = (actor*)other.resolve();
        ai_core* other_core  = other_actor ? other_actor->get_ai_core() : nullptr;

        if (!other_core)
            other_core = core;

        if (core->push_base_machine(other_core, graph, true))
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84E840
bool slf__entity__pop_ai_base_machine_immediately__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // same
    ref_unlock_scope instance_unlock(stack.thread->inst->parent->instance_lock);
    ref_unlock_scope machine_unlock(&jq::references::unk_010f7748.get());

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto graph_name = (const char*)stack.local_uint(0x04);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        string_hash name;
        name.initialize(mash::ALLOCATED, graph_name);

        resource_key graph;
        graph.set(name, (u32)amalga::e_resource_type::ai_state_graph);

        if (core->pop_base_machine(graph, true))
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84E9A0
bool slf__entity__change_locomotion_machine__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // same
    ref_unlock_scope instance_unlock(stack.thread->inst->parent->instance_lock);
    ref_unlock_scope machine_unlock(&jq::references::unk_010f7748.get());

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto inode_name = (const char*)stack.local_uint(0x04);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (!core) {
        stack.push(&result, sizeof(result));

        return true;
    }

    auto target = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // the entity stays suspended until the machine has changed, which the recall reports
    if (stack.thread->entry) {
        target->unsuspend(true);

        result = 1.0f;
        stack.push(&result, sizeof(result));

        return true;
    }

    target->suspend(true);

    string_hash name;
    name.initialize(mash::ALLOCATED, inode_name);

    if (core->change_locomotion_machine(name))
        return false;

    target->unsuspend(true);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84EBB0
bool slf__entity__change_locomotion_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // same
    ref_unlock_scope instance_unlock(stack.thread->inst->parent->instance_lock);
    ref_unlock_scope machine_unlock(&jq::references::unk_010f7748.get());

    auto self     = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto category = (const char*)stack.local_uint(0x04);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        arch_base_vhandle(stack.local_uint(0x00)).resolve();

        string_hash name;
        name.initialize(mash::ALLOCATED, category);

        core->my_loco_inode->set_loco_als_category(name);

        result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84ECF0
bool slf__entity__ai_break_morale_force_cower__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // the receiver resolves twice, the second answer is used
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto unk  = stack.local_num(0x04);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    if (!core)
        return true;

    if (auto* fear = (std_fear_inode*)core->get_info_node(info_node_type_std_fear))
        fear->break_morale_force_cower(unk);

    return true;
}

// sub_84ED70
bool slf__entity__ai_break_morale_force_flee__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    // same
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self     = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto position = (const vector3d*)&stack.local_uint(0x04);
    auto unk      = stack.local_num(0x10);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    if (!core)
        return true;

    if (auto* fear = (std_fear_inode*)core->get_info_node(info_node_type_std_fear))
        fear->break_morale_force_flee(*position, unk);

    return true;
}

// sub_84EE00
bool slf__entity__ai_break_morale_force_flee__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    // same
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = arch_base_vhandle(stack.local_uint(0x04));
    auto unk    = stack.local_num(0x08);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    if (!core)
        return true;

    auto* fear = (std_fear_inode*)core->get_info_node(info_node_type_std_fear);

    if (!fear)
        return true;

    // a stale target goes in as handle 0
    arch_base* entity = target.resolve();

    fear->break_morale_force_flee(entity ? entity->my_handle : arch_base_vhandle(), unk);

    return true;
}

// sub_84EEB0
bool slf__entity__get_ai_base_machine_state_name__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    // the receiver resolves twice, the second answer is used
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // neither the entity nor its core is tested
    ai_core* core = self ? self->get_ai_core() : nullptr;

    string_hash result = core->my_base_machine->get_curr_state_name();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84EF10
bool slf__entity__is_enemy__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // same
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self  = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto other = arch_base_vhandle(stack.local_uint(0x04));

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        auto other_actor = (actor*)other.resolve();

        if (other_actor && other_actor->is_an_actor() && other_actor->has_base_ai_data() &&
            ai_team::is_enemy(core->my_team, other_actor->get_ai_core()->my_team))

            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84EFC0
bool slf__entity__is_enemy_of_team__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // same
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self      = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto team_name = (const char*)stack.local_uint(0x04);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        string_hash team;
        team.initialize(mash::ALLOCATED, team_name);

        if (ai_team::is_enemy(core->my_team, ai_team::get_enum_by_hash(team)))
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F060
bool slf__entity__is_friend__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // same
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self  = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto other = arch_base_vhandle(stack.local_uint(0x04));

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        auto other_actor = (actor*)other.resolve();

        if (other_actor && other_actor->is_an_actor() && other_actor->has_base_ai_data() &&
            ai_team::is_friend(core->my_team, other_actor->get_ai_core()->my_team))

            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F110
bool slf__entity__is_friend_of_team__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // same
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self      = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto team_name = (const char*)stack.local_uint(0x04);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        string_hash team;
        team.initialize(mash::ALLOCATED, team_name);

        if (ai_team::is_friend(core->my_team, ai_team::get_enum_by_hash(team)))
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F1B0
bool slf__entity__ai_has_clear_line_of_fire__entity__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 radius = count - 1 > 0 ? stack.local_num(0x08) : 1.0f;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    ai_core* core = self ? self->get_ai_core() : nullptr;

    bool clear = true;

    if (core) {
        auto target = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

        const vector3d* start = &core->get_actor()->my_abs_po->get_position();

        vector3d end;

        // without a target the line runs 50 units along the actor's facing
        if (target)
            end = target->my_abs_po->get_position();
        else
            end = *start + core->get_actor()->my_abs_po->get_z_facing() * 50.0f;

        line_info line(*start, end);

        // the world cuts it short, just off the hit
        if (retail::sub_798F50((char*)&line, (i32)references::unk_00e7b2a0.read(), (i32)references::unk_00e7b2d0.read(), nullptr, 0)) // line_info::check_collision
            end = line.hit_pos + line.hit_norm * 0.01f;

        // any core that isn't an enemy standing in the way blocks it
        for (ai_core_list* list : { &references::ai_core_list_high.get(), &references::ai_core_list_low.get() }) {
            for (ai_core_list_node* node = list->head; node && clear; node = node->next) {
                if (ai_team::is_enemy(core->my_team, node->core->my_team))
                    continue;

                if (retail::sub_823A00((i32*)node->core->get_actor(), (f32*)start, (f32*)&end, radius))
                    clear = false;
            }
        }

        retail::sub_798F10((u8*)&line); // line_info::~line_info
    }

    f32 result = clear ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F550
bool slf__entity__has_als_category__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self          = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto category_name = (const char*)stack.local_uint(0x04);

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;

    f32 result = 0.0f;

    if (als && als->get_als_layer(als::base_layer)) {
        string_hash category;
        category.initialize(mash::ALLOCATED, category_name);

        result = (f32)als->get_als_layer(als::base_layer)->does_category_exist(category);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F610
bool slf__entity__get_als_category_name__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;

    string_hash result;

    if (als && als->get_als_layer(als::base_layer))
        result = als->get_als_layer(als::base_layer)->get_category_id();
    else
        result.initialize(mash::ALLOCATED, "invalid");

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F6C0
bool slf__entity__get_als_state_name__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;

    string_hash result;

    if (als && als->get_als_layer(als::base_layer))
        result = als->get_als_layer(als::base_layer)->get_state_id();
    else
        result.initialize(mash::ALLOCATED, "invalid");

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F770
bool slf__entity__force_strength_test__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = arch_base_vhandle(stack.local_uint(0x04));

    f32 result = 0.0f;

    if (self && target.value) {
        void* memory = retail::sub_823740(0x2C);                              // strength_test_event::operator new
        auto  raised = memory ? (event*)retail::sub_92D270(memory) : nullptr; // strength_test_event::strength_test_event

        retail::sub_91DF70(raised, target.value, false); // strength_test_event: test against the target
        self->raise_event(raised);
        delete raised;

        result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F840
bool slf__entity__begin_strength_test__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self          = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target        = arch_base_vhandle(stack.local_uint(0x04));
    bool from_standing = stack.local_num(0x08) != 0.0f;
    bool from_crawling = stack.local_num(0x0C) != 0.0f;

    f32 result = 0.0f;

    // only out of a hit switch (10) the hero entered standing (1) or crawling (2)
    if (self && target.value &&
        self->player_ifc()->get_mode() == 10 &&
        ((from_standing && self->player_ifc()->get_prev_mode() == 1) ||
         (from_crawling && self->player_ifc()->get_prev_mode() == 2))) {

        void* memory = retail::sub_823740(0x2C);                              // strength_test_event::operator new
        auto  raised = memory ? (event*)retail::sub_92D270(memory) : nullptr; // strength_test_event::strength_test_event

        retail::sub_91DF70(raised, target.value, false); // strength_test_event: test against the target
        self->raise_event(raised);
        delete raised;

        result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_880770
bool slf__entity__begin_strength_test__entity__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self          = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target        = arch_base_vhandle(stack.local_uint(0x04));
    bool from_standing = stack.local_num(0x08) != 0.0f;
    bool from_crawling = stack.local_num(0x0C) != 0.0f;

    // only out of a hit switch (10) the hero entered standing (1) or crawling (2)
    if (!self || !target.value ||
        self->player_ifc()->get_mode() != 10 ||
        !((from_standing && self->player_ifc()->get_prev_mode() == 1) ||
          (from_crawling && self->player_ifc()->get_prev_mode() == 2))) {

        f32 result = 0.0f;

        stack.push(&result, sizeof(result));

        return true;
    }

    // the target first, then the array's entities
    vc8::vector<arch_base_vhandle> targets;
    targets.push_back(target);

    chuck::vm::script_dynamic_array<arch_base_vhandle> entities((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x10));

    for (i32 i = 0; i < entities.size(); ++i)
        targets.push_back(entities[i]);

    void* memory = retail::sub_823740(0x2C);                              // strength_test_event::operator new
    auto  raised = memory ? (event*)retail::sub_92D270(memory) : nullptr; // strength_test_event::strength_test_event

    using set_target_tests_method = void (__thiscall*)(event* self, vc8::vector<arch_base_vhandle> targets, bool trigger);

    ((set_target_tests_method)retail::sub_93F7A0)(raised, targets, false); // strength_test_event: test against the targets
    self->raise_event(raised);
    delete raised;

    f32 result = 1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84F990
bool slf__entity__begin_strength_test_trigger__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self    = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto trigger = arch_base_vhandle(stack.local_uint(0x04));

    f32 result = 0.0f;

    if (self && trigger.value) {
        void* memory = retail::sub_823740(0x2C);                              // strength_test_event::operator new
        auto  raised = memory ? (event*)retail::sub_92D270(memory) : nullptr; // strength_test_event::strength_test_event

        retail::sub_91DF70(raised, trigger.value, true); // strength_test_event: test against the trigger
        self->raise_event(raised);
        delete raised;

        result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84FA60
bool slf__entity__run_strength_test__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto test_name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self) {
        void* memory = retail::sub_823740(0x2C);                              // strength_test_event::operator new
        auto  raised = memory ? (event*)retail::sub_92D270(memory) : nullptr; // strength_test_event::strength_test_event

        string_hash test;
        test.initialize(mash::ALLOCATED, test_name);

        retail::sub_91DFC0(raised, test.source_hash_code); // strength_test_event: the named test
        self->raise_event(raised);
        delete raised;

        result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84FB40
bool slf__entity__begin_slingshot_point_test__entity__vector3d__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self       = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto object     = (conglomerate*)arch_base_vhandle(stack.local_uint(0x04)).resolve();
    auto position   = (const vector3d*)&stack.local_uint(0x08);
    auto unk_entity = stack.local_uint(0x14);
    auto unk_num    = stack.local_num(0x18);

    if (!self || !object || !object->is_a_conglomerate())
        return true;

    // without the number, only out of a hit switch (10) the hero entered standing (1)
    if (unk_num == 0.0f &&
        (self->player_ifc()->get_mode() != 10 || self->player_ifc()->get_prev_mode() != 1))

        return true;

    string_hash point_1;
    point_1.initialize(mash::ALLOCATED, "WEB_ATTACH_POINT1");

    entity* web_attach_point_1 = object->get_member(object->get_member_index(point_1));

    string_hash point_2;
    point_2.initialize(mash::ALLOCATED, "WEB_ATTACH_POINT2");

    entity* web_attach_point_2 = object->get_member(object->get_member_index(point_2));

    void* memory = retail::sub_823840(0x30);                              // slingshot_point_event::operator new
    auto  raised = memory ? (event*)retail::sub_91EE30(memory) : nullptr; // slingshot_point_event::slingshot_point_event

    retail::sub_90CDF0(raised,
                       stack.local_uint(0x04),
                       web_attach_point_1->my_handle.value,
                       web_attach_point_2->my_handle.value,
                       (const f32*)position,
                       unk_entity,
                       unk_num > 0.0f); // slingshot_point_event: the request

    self->raise_event(raised);
    delete raised;

    return true;
}

// sub_84FCD0
bool slf__entity__is_in_reflexes__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self->player_ifc()->get_reflexes()->is_enabled() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84FD20
bool slf__entity__is_in_senses__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self->player_ifc()->get_spider_senses()->is_enabled() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84FD70
bool slf__entity__is_sync_anim__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && (self->ext_flags & entity_ext_flag_sync_anim) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84FDD0
bool slf__entity__get_als_time_to_end_of_anim__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;
    als::state_machine_interface*          sm  = als ? als->get_als_layer(als::base_layer) : nullptr;

    f32 result = sm ? sm->get_time_to_end_of_anim(false, true) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84FE50
bool slf__entity__get_als_current_anim_time__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;
    als::state_machine_interface*          sm  = als ? als->get_als_layer(als::base_layer) : nullptr;

    f32 result = sm ? sm->get_current_anim_time() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_84FED0
bool slf__entity__get_als_current_normalized_time__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;
    als::state_machine_interface*          sm  = als ? als->get_als_layer(als::base_layer) : nullptr;

    f32 result = sm ? sm->get_current_normalized_time() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
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

// sub_850050
bool slf__entity__get_als_data_float__str__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto param_name = (const char*)stack.local_uint(0x04);

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;
    als::state_machine_interface*          sm  = als ? als->get_als_layer(als::base_layer) : nullptr;

    f32 result = 0.0f;

    if (sm) {
        string_hash parameter;
        parameter.initialize(mash::ALLOCATED, param_name);

        // only a lone extra argument is taken as the default; any other count makes the parameter required
        if (count == 2)
            result = sm->get_optional_pb_float(parameter, stack.local_num(0x08), nullptr);
        else
            result = sm->get_pb_float(parameter);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_850150
bool slf__entity__get_als_data_int__str__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto param_name = (const char*)stack.local_uint(0x04);

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;
    als::state_machine_interface*          sm  = als ? als->get_als_layer(als::base_layer) : nullptr;

    f32 result = 0.0f;

    if (sm) {
        string_hash parameter;
        parameter.initialize(mash::ALLOCATED, param_name);

        // only a lone extra argument is taken as the default; any other count makes the parameter required
        if (count == 2)
            result = (f32)sm->get_optional_pb_int(parameter, (i32)stack.local_num(0x08), nullptr);
        else
            result = (f32)sm->get_pb_int(parameter);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_850250
bool slf__entity__get_als_data_vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    auto self          = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto param_name    = (const char*)stack.local_uint(0x04);
    auto default_value = (const vector3d*)&stack.local_uint(0x08);

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;
    als::state_machine_interface*          sm  = als ? als->get_als_layer(als::base_layer) : nullptr;

    vector3d result(0.0f, 0.0f, 0.0f);

    if (sm) {
        string_hash parameter;
        parameter.initialize(mash::ALLOCATED, param_name);

        // any extra argument makes the parameter optional, its three words the default
        if (count - 1 > 0)
            result = sm->get_optional_pb_vector3d(parameter, *default_value, nullptr);
        else
            result = sm->get_pb_vector3d(parameter);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_850390
bool slf__entity__get_als_data_string__str__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto param_name = (const char*)stack.local_uint(0x04);

    als::animation_logic_system_interface* als = self && self->is_an_actor() ? self->get_als() : nullptr;
    als::state_machine_interface*          sm  = als ? als->get_als_layer(als::base_layer) : nullptr;

    const char* result = "";

    if (sm) {
        string_hash parameter;
        parameter.initialize(mash::ALLOCATED, param_name);

        bool is_string = sm->does_parameter_exist(parameter) && sm->get_parameter_data_type(parameter) == param_type_string;

        // a lone extra argument answers for a missing or non-string parameter
        if (count == 2)
            result = is_string ? sm->get_pb_mstring(parameter) : (const char*)stack.local_uint(0x08);
        else if (is_string)
            result = sm->get_pb_mstring(parameter);
    }

    result = stack.thread->create_string(result);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8504E0
bool slf__entity__has_ai_core__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() && ((actor*)self)->get_ai_core() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8505E0
bool slf__entity__crawl_attach_to_surface__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self            = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto position        = (const vector3d*)&stack.local_uint(0x04);
    auto desired_forward = (const vector3d*)&stack.local_uint(0x10);

    if (!self || !self->is_an_actor())
        return true;

    ai_core* core = self->get_ai_core();

    if (!core)
        return true;

    auto* crawl = (crawl_layer_inode*)core->get_info_node(info_node_type_crawl_layer);

    // only while crawling is the actor's locomotion
    if (!crawl || core->my_loco_inode != crawl)
        return true;

    line_info surface;
    vector3d  up(0.0f, 1.0f, 0.0f);

    if (retail::sub_42EFF0((f32*)position, 5.0f, (f32*)&surface, (i32*)&up, true, false)) { // get_best_surface_pos
        // a second look straight into the surface from just off it; the nearer hit wins
        line_info probe;
        probe.start = *position + surface.hit_norm * 0.1f;
        probe.end   = probe.start - surface.hit_norm * 5.25f;

        if (retail::sub_798F50((char*)&probe, (i32)references::unk_00e7b2a0.read(), (i32)references::unk_00e7b2d0.read(), nullptr, 0)) { // line_info::check_collision
            vector3d to_probe   = probe.hit_pos - *position;
            vector3d to_surface = surface.hit_pos - *position;

            f64 probe_distance   = retail::sub_582FC0((f32*)&to_probe);
            f64 surface_distance = retail::sub_582FC0((f32*)&to_surface);

            if (surface_distance > probe_distance)
                crawl->attach_to_surface(probe, *desired_forward);
            else
                crawl->attach_to_surface(surface, *desired_forward);
        } else
            crawl->attach_to_surface(surface, *desired_forward);

        retail::sub_798F10((u8*)&probe); // line_info::~line_info
    }

    retail::sub_798F10((u8*)&surface); // line_info::~line_info

    return true;
}

// sub_850990
bool slf__entity__crawl_release_from_surface__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self            = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto desired_forward = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !self->is_an_actor())
        return true;

    ai_core* core = self->get_ai_core();

    if (!core)
        return true;

    auto* crawl = (crawl_layer_inode*)core->get_info_node(info_node_type_crawl_layer);

    if (!crawl)
        return true;

    // a player lets go while its interface says it's crawling, anyone else while crawling is its locomotion
    if (self->has_ifc(entity_ifc_player)) {
        if (self->player_ifc()->crawling())
            crawl->release_from_surface(*desired_forward);
    } else if (core->my_loco_inode == crawl)
        crawl->release_from_surface(*desired_forward);

    return true;
}

// sub_850A60
bool slf__entity__hates_spiderman__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() && self->hates_spiderman() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_850AE0
bool slf__entity__set_detectable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_actor())
        self->set_flag_recursive(entity_flag_detectable, on != 0.0f);

    return true;
}

// sub_850B40
bool slf__entity__set_spidey_sense_glow_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto color = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !self->is_an_actor())
        return true;

    retail::sub_6E7470((u32*)references::frontend.get().igo, self->my_handle.value, 4, true);               // glow mode
    retail::sub_6F2190((u32*)references::frontend.get().igo, self->my_handle.value, (const f32*)color, true); // glow colour

    return true;
}

// sub_850BA0
bool slf__entity__clear_spidey_sense_glow_info__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_actor())
        retail::sub_6D90F0((u32*)references::frontend.get().igo, self->my_handle.value); // glow record removal

    return true;
}

// sub_850BE0
bool slf__entity__set_target_icon__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto icon = stack.local_num(0x04);

    if (self)
        retail::sub_6F6400((u32*)references::frontend.get().igo, self->my_handle.value, (i32)icon);

    return true;
}

// sub_850C20
bool slf__entity__clear_target_icon__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self)
        retail::sub_6D9260((i32)references::frontend.get().igo, self->my_handle.value);

    return true;
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

// sub_850DA0
bool slf__entity__is_a_placeholder_vehicle__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_placeholder_vehicle() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_850E10
bool slf__entity__set_vehicle_model__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto model_name = (const char*)stack.local_uint(0x04);

    if (self && self->is_a_vehicle()) {
        string_hash model;
        model.initialize(mash::ALLOCATED, model_name);

        ((vehicle*)self)->set_model(model);
    }

    return true;
}

// sub_850E60
bool slf__entity__damage_vehicle_part__uint__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto part = stack.local_uint(0x04);

    if (self && self->is_a_vehicle()) {
        string_hash member_id;
        member_id.initialize(mash::ALLOCATED, nullptr, part);

        ((vehicle*)self)->damage_member(member_id);
    }

    return true;
}

// sub_850EC0
bool slf__entity__damage_all_vehicle_parts__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->damage_all_members();

    return true;
}

// sub_850F00
bool slf__entity__repair_vehicle_part__uint__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto part = stack.local_uint(0x04);

    if (self && self->is_a_vehicle()) {
        string_hash member_id;
        member_id.initialize(mash::ALLOCATED, nullptr, part);

        ((vehicle*)self)->repair_member(member_id);
    }

    return true;
}

// sub_850F60
bool slf__entity__repair_all_vehicle_parts__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->repair_all_members();

    return true;
}

// sub_86FF80
bool slf__entity__get_vehicle_parts__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    return true;
}

// sub_850FA0
bool slf__entity__set_vehicle_mass__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto mass = stack.local_num(0x04);

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->set_mass(mass);

    return true;
}

// sub_850FE0
bool slf__entity__set_vehicle_self_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage = stack.local_num(0x04);

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->set_self_damage(damage, -1.0f, -1.0f, -1.0f);

    return true;
}

// sub_851040
bool slf__entity__set_vehicle_other_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto damage = stack.local_num(0x04);

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->set_other_damage(damage, -1.0f, -1.0f, -1.0f);

    return true;
}

// sub_8510A0
bool slf__entity__get_vehicle_mass__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_vehicle() ? ((vehicle*)self)->my_mass : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851110
bool slf__entity__get_vehicle_dimensions__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self && self->is_a_vehicle() ? ((vehicle*)self)->my_dimensions : vector3d(0.0f, 0.0f, 0.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8511A0
bool slf__entity__get_vehicle_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result(0.0f, 0.0f, 0.0f);

    if (self && self->is_a_vehicle() && ((vehicle*)self)->my_rb)
        result = ((vehicle*)self)->my_rb->m_t_vel.get_xyz();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851240
bool slf__entity__get_vehicle_angular_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result(0.0f, 0.0f, 0.0f);

    if (self && self->is_a_vehicle() && ((vehicle*)self)->my_rb)
        result = ((vehicle*)self)->my_rb->m_a_vel.get_xyz();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8512E0
bool slf__entity__set_max_vehicle_speed__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto speed = stack.local_num(0x04);

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->my_ai->m_inode->set_max_speed(speed);

    return true;
}

// sub_851320
bool slf__entity__get_max_vehicle_speed__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_vehicle() ? ((vehicle*)self)->my_ai->m_inode->get_max_speed() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851390
bool slf__entity__set_vehicle_flags__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto flags = stack.local_num(0x04);

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->my_ai->set_flags((u32)(i64)flags);

    return true;
}

// sub_851400
bool slf__entity__get_vehicle_flags__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_vehicle() ? (f32)((vehicle*)self)->my_ai->get_flags() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851480
bool slf__entity__make_vehicle_clown_car__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    // the only occupant native that doesn't ask for a conglomerate
    if (self && self->has_occupant_ifc())
        self->occupant_ifc()->clown_car = on != 0.0f;

    return true;
}

// sub_8514E0
bool slf__entity__set_vehicle_purpose__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self    = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto purpose = stack.local_num(0x04);

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->my_ai->set_purpose((e_ai_vehicle_purpose)(i32)purpose);

    return true;
}

// sub_851520
bool slf__entity__set_vehicle_purpose__num__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self    = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto purpose = stack.local_num(0x04);
    auto target  = arch_base_vhandle(stack.local_uint(0x08));

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->my_ai->set_purpose((e_ai_vehicle_purpose)(i32)purpose, target);

    return true;
}

// sub_851570
bool slf__entity__set_vehicle_purpose__num__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto purpose   = stack.local_num(0x04);
    auto target    = arch_base_vhandle(stack.local_uint(0x08));
    auto near_only = stack.local_num(0x0C);

    // made once and never read
    if (!(references::set_vehicle_purpose_name_guard.read() & 1)) {
        references::set_vehicle_purpose_name_guard.write(references::set_vehicle_purpose_name_guard.read() | 1);
        references::set_vehicle_purpose_name.get().initialize(mash::ALLOCATED, "set_vehicle_purpose");
    }

    if (!self || !self->is_a_vehicle())
        return true;

    // only a target within 200
    if (near_only != 0.0f) {
        auto target_entity = (entity*)target.resolve();

        if (!target_entity)
            return true;

        vector3d delta = self->my_abs_po->get_position() - target_entity->my_abs_po->get_position();

        if (delta.length2() > 40000.0f)
            return true;
    }

    ((vehicle*)self)->my_ai->set_purpose((e_ai_vehicle_purpose)(i32)purpose, target);

    return true;
}

// sub_8516F0
bool slf__entity__set_vehicle_purpose__num__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto purpose  = stack.local_num(0x04);
    auto position = (const vector3d*)&stack.local_uint(0x08);
    auto radius   = stack.local_num(0x14);

    bool taken = false;

    if (self && self->is_a_vehicle())
        taken = ((vehicle*)self)->my_ai->set_purpose((e_ai_vehicle_purpose)(i32)purpose, *position, radius);

    f32 result = taken ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851790
bool slf__entity__set_vehicle_purpose__num__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto purpose   = stack.local_num(0x04);
    auto position  = (const vector3d*)&stack.local_uint(0x08);
    auto radius    = stack.local_num(0x14);
    auto near_only = stack.local_num(0x18);

    bool taken = false;

    if (self && self->is_a_vehicle()) {
        // only a position within 200
        bool too_far = near_only != 0.0f && (self->my_abs_po->get_position() - *position).length2() > 40000.0f;

        if (!too_far)
            taken = ((vehicle*)self)->my_ai->set_purpose((e_ai_vehicle_purpose)(i32)purpose, *position, radius);
    }

    f32 result = taken ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_87EA20
bool slf__entity__set_vehicle_purpose__num__dynamic_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self    = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto purpose = stack.local_num(0x04);

    if (!self || !self->is_a_vehicle())
        return true;

    chuck::vm::script_dynamic_array<vector3d> points((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x08));
    chuck::vm::script_dynamic_array<f32>      radii ((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x0C));

    if (!points.array->size())
        return true;

    if (!radii.array->size() || radii.size() != points.size())
        return true;

    vc8::vector<vector3d> positions;
    vc8::vector<f32>      radius_list;

    for (i32 i = 0; i < points.size(); i++) {
        positions  .push_back(points[i]);
        radius_list.push_back(radii[i]);
    }

    ((vehicle*)self)->my_ai->set_purpose((e_ai_vehicle_purpose)(i32)purpose, positions, radius_list);

    return true;
}

// sub_8518F0
bool slf__entity__get_vehicle_purpose__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_vehicle() ? (f32)((vehicle*)self)->my_ai->m_inode->get_purpose() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851990
bool slf__entity__get_vehicle_purpose_entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result = self && self->is_a_vehicle() ? ((vehicle*)self)->my_ai->m_inode->get_purpose_entity() : arch_base_vhandle();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851A30
bool slf__entity__get_vehicle_purpose_pos__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self && self->is_a_vehicle() ?
        ((vehicle*)self)->my_ai->m_inode->get_purpose_pos() :
        references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851AC0
bool slf__entity__get_vehicle_purpose_radius__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_vehicle() ? ((vehicle*)self)->my_ai->m_inode->get_purpose_radius() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851B60
bool slf__entity__is_vehicle_purpose_done__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    // the only purpose native that minds a missing ai
    if (self && self->is_a_vehicle() && ((vehicle*)self)->my_ai)
        result = (f32)((vehicle*)self)->my_ai->m_inode->is_purpose_done();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_87C190
bool slf__entity__set_vehicle_steering__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!self || !self->is_a_vehicle())
        return true;

    if (auto* state = get_script_controlled_state((vehicle*)self))
        state->set_steer_angle(value);

    return true;
}

// sub_87C220
bool slf__entity__get_vehicle_steering__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    ai_vehicle_state_script* state = self && self->is_a_vehicle() ? get_script_controlled_state((vehicle*)self) : nullptr;

    f32 result = state ? state->my_steer_angle : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_87C2D0
bool slf__entity__set_vehicle_gas_pedal__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!self || !self->is_a_vehicle())
        return true;

    if (auto* state = get_script_controlled_state((vehicle*)self))
        state->set_gas_pedal(value);

    return true;
}

// sub_87C360
bool slf__entity__get_vehicle_gas_pedal__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    ai_vehicle_state_script* state = self && self->is_a_vehicle() ? get_script_controlled_state((vehicle*)self) : nullptr;

    f32 result = state ? state->my_gas_pedal : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_87C410
bool slf__entity__set_vehicle_brake_pedal__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!self || !self->is_a_vehicle())
        return true;

    if (auto* state = get_script_controlled_state((vehicle*)self))
        state->set_brake_pedal(value);

    return true;
}

// sub_87C4A0
bool slf__entity__get_vehicle_brake_pedal__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    ai_vehicle_state_script* state = self && self->is_a_vehicle() ? get_script_controlled_state((vehicle*)self) : nullptr;

    f32 result = state ? state->my_brake_pedal : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_87C550
bool slf__entity__set_vehicle_power_brake__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!self || !self->is_a_vehicle())
        return true;

    if (auto* state = get_script_controlled_state((vehicle*)self))
        state->power_brake = value != 0.0f;

    return true;
}

// sub_87C5F0
bool slf__entity__get_vehicle_power_brake__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    ai_vehicle_state_script* state = self && self->is_a_vehicle() ? get_script_controlled_state((vehicle*)self) : nullptr;

    f32 result = state ? (f32)state->power_brake : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_87C6A0
bool slf__entity__set_vehicle_reverse__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!self || !self->is_a_vehicle())
        return true;

    if (auto* state = get_script_controlled_state((vehicle*)self))
        state->reverse = value != 0.0f;

    return true;
}

// sub_87C740
bool slf__entity__get_vehicle_reverse__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    ai_vehicle_state_script* state = self && self->is_a_vehicle() ? get_script_controlled_state((vehicle*)self) : nullptr;

    f32 result = state ? (f32)state->reverse : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851C10
bool slf__entity__enable_vehicle_physics__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->enable_physics(enable != 0.0f);

    return true;
}

// sub_851C70
bool slf__entity__is_vehicle_physics_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    bool enabled = self && self->is_a_vehicle() && ((vehicle*)self)->physics_enabled;

    f32 result = (f32)enabled;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851CD0
bool slf__entity__set_vehicle_condition__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto condition = stack.local_num(0x04);

    if (self && self->is_a_vehicle())
        ((vehicle*)self)->set_condition((i32)condition);

    return true;
}

// sub_851D10
bool slf__entity__get_vehicle_condition__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    i32 condition = self && self->is_a_vehicle() ? ((vehicle*)self)->condition : 0;

    f32 result = (f32)condition;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851D60
bool slf__entity__can_vehicle_be_stolen__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_a_vehicle() && ((vehicle*)self)->my_ai)
        result = ((vehicle*)self)->my_ai->can_be_stolen ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_851E00
bool slf__entity__set_vehicle_can_be_stolen__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_a_vehicle() && ((vehicle*)self)->my_ai)
        ((vehicle*)self)->my_ai->can_be_stolen = on > 0.0001f;

    return true;
}

// sub_851E60
bool slf__entity__set_vehicle_sleeps_on_stop__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_a_vehicle() && ((vehicle*)self)->my_ai)
        ((vehicle*)self)->my_ai->set_sleeps_on_stop(on > 0.0001f);

    return true;
}

// sub_851ED0
bool slf__entity__weapon_fire_as_gun__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target   = arch_base_vhandle(stack.local_uint(0x04));
    auto position = (const vector3d*)&stack.local_uint(0x08);

    // the weapon is its own shooter
    if (self)
        fire_weapon_entity(self, self->my_handle, weapon_fire_kind_gun, true, target, *position);

    return true;
}

// sub_851F20
bool slf__entity__weapon_fire_as_rocket__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target   = arch_base_vhandle(stack.local_uint(0x04));
    auto position = (const vector3d*)&stack.local_uint(0x08);

    if (self)
        fire_weapon_entity(self, self->my_handle, weapon_fire_kind_rocket, true, target, *position);

    return true;
}

// sub_851F70
bool slf__entity__weapon_fire_as_grenade__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target   = arch_base_vhandle(stack.local_uint(0x04));
    auto position = (const vector3d*)&stack.local_uint(0x08);

    if (self)
        fire_weapon_entity(self, self->my_handle, weapon_fire_kind_grenade, true, target, *position);

    return true;
}

// sub_851FC0
bool slf__entity__rocket_redirect__entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self       = (grenade*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto redirector = arch_base_vhandle(stack.local_uint(0x04));
    auto target     = arch_base_vhandle(stack.local_uint(0x08));

    // the redirector resolves once for the test and again for the call
    if (self && self->is_a_grenade())
        self->redirect(target, redirector.resolve() ? (entity*)redirector.resolve() : nullptr);

    return true;
}

// sub_852020
bool slf__entity__rocket_redirect_dir__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self       = (grenade*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto redirector = arch_base_vhandle(stack.local_uint(0x04));
    auto dir        = (const vector3d*)&stack.local_uint(0x08);

    // same
    if (self && self->is_a_grenade())
        self->redirect(*dir, redirector.resolve() ? (entity*)redirector.resolve() : nullptr);

    return true;
}

// sub_852080
bool slf__entity__rocket_disable_guidance__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (grenade*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_grenade())
        self->disable_guidance();

    return true;
}

// sub_8520C0
bool slf__entity__force_ai_enter_car__entity__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self    = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto car     = arch_base_vhandle(stack.local_uint(0x04));
    auto seat    = stack.local_num(0x08);
    auto animate = stack.local_num(0x0C);

    ai_core* core = self && self->is_an_actor() ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        auto vehicle = (entity*)car.resolve();

        if (vehicle && vehicle->is_a_conglomerate() && vehicle->has_occupant_ifc()) {
            auto* car_inode = (ai_car_inode*)core->get_info_node(info_node_type_ai_car);

            if (car_inode && car_inode->force_enter_car(car, (i32)seat, animate > 0.5f))
                result = 1.0f;
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8521B0
bool slf__entity__force_ai_exit_car__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self    = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto animate = stack.local_num(0x04);

    ai_core* core = self && self->is_an_actor() ? self->get_ai_core() : nullptr;

    f32 result = 0.0f;

    if (core) {
        auto* car_inode = (ai_car_inode*)core->get_info_node(info_node_type_ai_car);

        if (car_inode && car_inode->force_exit_car(animate > 0.5f))
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852270
bool slf__entity__get_car_seat__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    i32 seat = -1;

    if (self && self->is_an_actor())
        seat = retail::sub_4CB950((i32)self); // get_car_seat

    f32 result = (f32)seat;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8522D0
bool slf__entity__is_seat_taken__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    // -1 for anything without seats
    if (self) {
        result = -1.0f;

        if (self->is_a_conglomerate() && self->has_occupant_ifc()) {
            auto seat = (i32)stack.local_num(0x04);

            result = self->occupant_ifc()->m_seat_occupants[seat].resolve() ? 1.0f : 0.0f;
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852380
bool slf__entity__is_car_occupied__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    bool occupied = self &&
                    self->is_a_conglomerate() &&
                    self->has_occupant_ifc() &&
                    self->occupant_ifc()->m_vehicle_occupant_count > 0;

    f32 result = (f32)occupied;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852410
bool slf__entity__vehicle_disabled_escape_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_a_conglomerate() && self->has_occupant_ifc())
        self->occupant_ifc()->disabled_escape_enabled = on != 0.0f;

    return true;
}

// sub_852480
bool slf__entity__vehicle_eject_kills_occupants__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_a_conglomerate() && self->has_occupant_ifc())
        self->occupant_ifc()->eject_kills_occupants = on != 0.0f;

    return true;
}

// sub_8524F0
bool slf__entity__set_vehicle_occupant_damage_tolerance_percentage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto percentage = stack.local_num(0x04);

    if (!self)
        return true;

    // anything over 1 counts as out of 100
    if (percentage > 1.0f)
        percentage = (f32)((f64)percentage / 100.0);

    if (self->is_a_conglomerate() && self->has_occupant_ifc())
        self->occupant_ifc()->occupant_damage_tolerance_percentage = percentage;

    return true;
}

// sub_852560
bool slf__entity__get_actor_in_seat__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto seat = stack.local_num(0x04);

    u32 result = 0;

    // the interface is asked for twice
    if (self &&
        self->is_a_conglomerate() &&
        self->has_occupant_ifc() &&
        self->occupant_ifc()->m_seat_occupants[(i32)seat].resolve())

        result = self->occupant_ifc()->m_seat_occupants[(i32)seat].value;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8525E0
bool slf__entity__get_tentacle__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self          = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto tentacle_name = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    if (self && self->is_a_conglomerate()) {
        string_hash id;
        id.initialize(mash::ALLOCATED, tentacle_name);

        if (self->tentacles) {
            ai_tentacle_info* info = ai_tentacle_info::find_tentacle(id, self->tentacles->array, (i32)self->tentacles->size);

            if (info && info->tentacle)
                result = info->tentacle->my_handle;
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852680
bool slf__entity__set_attack_damage_locked__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto locked = stack.local_num(0x04);

    if (!self)
        return true;

    if (locked <= 0.5f)
        self->ext_flags |= entity_ext_flag_attack_damage_unlocked;
    else
        self->ext_flags &= ~entity_ext_flag_attack_damage_unlocked;

    return true;
}

// sub_8526C0
bool slf__entity__is_a_variant__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852740
bool slf__entity__get_variant_name__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    const char* result = "";

    // the name's hash as text, in a new script string
    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant)) {
        string_hash name;
        ((string_hash* (__thiscall*)(variant_interface*, string_hash*, i32))retail::sub_6108F0)(self->variant_ifc(), &name, (i32)index); // variant_interface::get_variant_name

        result = stack.thread->create_string(name.to_string());
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8527D0
bool slf__entity__get_variant_weight__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    f32 result = 0.0f;

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant))
        result = ((f32 (__thiscall*)(variant_interface*, i32))retail::sub_610930)(self->variant_ifc(), (i32)index); // variant_interface::get_weight;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852850
bool slf__entity__get_num_variants__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant))
        result = (f32)(i32)self->variant_ifc()->records.size;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8528C0
bool slf__entity__get_max_variant_texture__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto part = stack.local_num(0x04);

    f32 result = 0.0f;

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant))
        result = (f32)((i32 (__thiscall*)(variant_interface*, i32))retail::sub_610290)(self->variant_ifc(), (i32)part); // variant_interface::get_max_variant_texture;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852940
bool slf__entity__get_min_variant_texture__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto part = stack.local_num(0x04);

    f32 result = 0.0f;

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant))
        result = (f32)((i32 (__thiscall*)(variant_interface*, i32))retail::sub_610260)(self->variant_ifc(), (i32)part); // variant_interface::get_min_variant_texture;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8529C0
bool slf__entity__has_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto variant = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant)) {
        string_hash name;
        name.initialize(mash::ALLOCATED, variant);

        result = ((i32 (__thiscall*)(variant_interface*, string_hash))retail::sub_6101A0)(self->variant_ifc(), name) > -1 ? 1.0f : 0.0f; // variant_interface::find_variant
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852A90
bool slf__entity__find_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto variant = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    // -1 when there's none
    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant)) {
        string_hash name;
        name.initialize(mash::ALLOCATED, variant);

        result = (f32)((i32 (__thiscall*)(variant_interface*, string_hash))retail::sub_6101A0)(self->variant_ifc(), name); // variant_interface::find_variant
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852B50
bool slf__entity__prepare_change_variant__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant))
        ((void (__cdecl*)())retail::sub_9DCC80)(); // ngl::wait_for_rendering

    return true;
}

// sub_852B90
bool slf__entity__set_variant__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto variant = (const char*)stack.local_uint(0x04);

    if (!(self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant)))
        return true;

    string_hash name;
    name.initialize(mash::ALLOCATED, variant);

    ((void (__thiscall*)(variant_interface*, string_hash))retail::sub_65F730)(self->variant_ifc(), name); // variant_interface::set_variant
    ((void (__thiscall*)(variant_interface*))retail::sub_610510)(self->variant_ifc());                    // variant_interface::set_default_variant_textures

    return true;
}

// sub_852C40
bool slf__entity__set_variant_id__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_num(0x04);

    if (!(self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant)))
        return true;

    ((void (__thiscall*)(variant_interface*, i32))retail::sub_65F6F0)(self->variant_ifc(), (i32)id); // variant_interface::set_variant_id
    ((void (__thiscall*)(variant_interface*))retail::sub_610510)(self->variant_ifc());             // variant_interface::set_default_variant_textures

    return true;
}

// sub_852CB0
bool slf__entity__set_variant_textures__num__num__num__num__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x24;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto texture_0 = stack.local_num(0x04);
    auto texture_1 = stack.local_num(0x08);
    auto texture_2 = stack.local_num(0x0C);
    auto texture_3 = stack.local_num(0x10);
    auto texture_4 = stack.local_num(0x14);
    auto texture_5 = stack.local_num(0x18);
    auto texture_6 = stack.local_num(0x1C);
    auto texture_7 = stack.local_num(0x20);

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant))
        ((void (__thiscall*)(variant_interface*, i32, i32, i32, i32, i32, i32, i32, i32))retail::sub_601030) // variant_interface::set_variant_textures
            (self->variant_ifc(), (i32)texture_0, (i32)texture_1, (i32)texture_2, (i32)texture_3, (i32)texture_4, (i32)texture_5, (i32)texture_6, (i32)texture_7);

    return true;
}

// sub_852D30
bool slf__entity__set_variant_and_textures__str__num__num__num__num__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x28;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto variant   = (const char*)stack.local_uint(0x04);
    auto texture_0 = stack.local_num(0x08);
    auto texture_1 = stack.local_num(0x0C);
    auto texture_2 = stack.local_num(0x10);
    auto texture_3 = stack.local_num(0x14);
    auto texture_4 = stack.local_num(0x18);
    auto texture_5 = stack.local_num(0x1C);
    auto texture_6 = stack.local_num(0x20);
    auto texture_7 = stack.local_num(0x24);

    if (!(self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant)))
        return true;

    string_hash name;
    name.initialize(mash::ALLOCATED, variant);

    ((void (__thiscall*)(variant_interface*, string_hash))retail::sub_65F730)(self->variant_ifc(), name); // variant_interface::set_variant

    ((void (__thiscall*)(variant_interface*, i32, i32, i32, i32, i32, i32, i32, i32))retail::sub_601030) // variant_interface::set_variant_textures
        (self->variant_ifc(), (i32)texture_0, (i32)texture_1, (i32)texture_2, (i32)texture_3, (i32)texture_4, (i32)texture_5, (i32)texture_6, (i32)texture_7);

    return true;
}

// sub_852E20
bool slf__entity__set_random_variant__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant))
        ((void (__thiscall*)(variant_interface*))retail::sub_666820)(self->variant_ifc()); // variant_interface::set_random_variant

    return true;
}

// sub_852E70
bool slf__entity__set_random_textures__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_conglomerate() && self->has_ifc(entity_ifc_variant))
        ((void (__thiscall*)(variant_interface*))retail::sub_610520)(self->variant_ifc()); // variant_interface::set_random_textures

    return true;
}

// sub_852EC0
bool slf__entity__set_simple_variant_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto mode = stack.local_num(0x04);

    if (!(self && self->is_an_actor() && self->has_ifc(entity_ifc_simple_variant)))
        return true;

    self->simple_variant_ifc()->simple_variant_mode = (i32)mode;

    // a conglomerate's members follow
    if (!self->is_a_conglomerate())
        return true;

    for (entity* member : ((conglomerate*)self)->members) {
        if (member->has_ifc(entity_ifc_simple_variant))
            member->simple_variant_ifc()->simple_variant_mode = (i32)mode;
    }

    return true;
}

// sub_852F70
bool slf__entity__get_simple_variant_mode__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = -1.0f;

    if (self && self->is_an_actor() && self->has_ifc(entity_ifc_simple_variant))
        result = (f32)self->simple_variant_ifc()->simple_variant_mode;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_852FF0
bool slf__entity__set_simple_variant__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto variant = stack.local_num(0x04);

    if (!(self && self->is_an_actor() && self->has_ifc(entity_ifc_simple_variant)))
        return true;

    self->simple_variant_ifc()->simple_variant = (i32)variant;

    // same
    if (!self->is_a_conglomerate())
        return true;

    for (entity* member : ((conglomerate*)self)->members) {
        if (member->has_ifc(entity_ifc_simple_variant))
            member->simple_variant_ifc()->simple_variant = (i32)variant;
    }

    return true;
}

// sub_8530A0
bool slf__entity__get_simple_variant__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = -1.0f;

    if (self && self->is_an_actor() && self->has_ifc(entity_ifc_simple_variant))
        result = (f32)self->simple_variant_ifc()->simple_variant;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_853120
bool slf__entity__aim_head__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    // the look-at natives don't test the core
    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->set_flag(aim_util_flag_aim_head, enable > 0.0f);

    return true;
}

// sub_8531B0
bool slf__entity__aim_head_direct__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->set_flag(aim_util_flag_aim_head_direct, enable > 0.0f);

    return true;
}

// sub_853240
bool slf__entity__aim_head_and_eyes__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->set_flag(aim_util_flag_aim_head_and_eyes, enable > 0.0f);

    return true;
}

// sub_8532D0
bool slf__entity__aim_head_eyes_and_upper_body__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->set_flag(aim_util_flag_aim_head_eyes_and_upper_body, enable > 0.0f);

    return true;
}

// sub_853360
bool slf__entity__aim_head_and_upper_body__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->set_flag(aim_util_flag_aim_head_and_upper_body, enable > 0.0f);

    return true;
}

// sub_8533F0
bool slf__entity__aim_head_and_upper_body_direct__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->set_flag(aim_util_flag_aim_head_and_upper_body_direct, enable > 0.0f);

    return true;
}

// sub_853480
bool slf__entity__aim_left_arm__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->set_flag(aim_util_flag_aim_left_arm, enable > 0.0f);

    return true;
}

// sub_853510
bool slf__entity__aim_right_arm__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->set_flag(aim_util_flag_aim_right_arm, enable > 0.0f);

    return true;
}

// sub_8535A0
bool slf__entity__update_target_to_look_at__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !self->is_a_conglomerate())
        return true;

    if (auto* aim = (aim_util_inode*)self->get_ai_core()->get_info_node(info_node_type_aim_util))
        aim->target_to_look_at = *target;

    return true;
}

// sub_853620
bool slf__entity__physical_ifc_add_dynamic_pendulum__vector3d__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x20;

    auto self         = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto pivot        = (const vector3d*)&stack.local_uint(0x04);
    auto length       = stack.local_num(0x10);
    auto attach_point = (const vector3d*)&stack.local_uint(0x14);

    // the pendulum's index, -1 without one
    f32 result = -1.0f;

    if (self && self->has_ifc(entity_ifc_physical)) {
        auto* the_pendulum = new (mm::allocate(sizeof(pendulum))) pendulum();

        the_pendulum->set_vector3d_pivot(*pivot);

        // a negative length keeps the current distance to the pivot
        if (length < 0.0f) {
            vector3d position  = self->my_abs_po->get_position();
            vector3d pivot_pos = the_pendulum->get_pivot_abs_pos();

            vector3d rope((f32)((f64)pivot_pos.x - position.x),
                          (f32)((f64)pivot_pos.y - position.y),
                          (f32)((f64)pivot_pos.z - position.z));

            the_pendulum->set_constraint(rope.length());
        } else
            the_pendulum->set_constraint(length);

        the_pendulum->attach_point = *attach_point;
        the_pendulum->enable(true);

        result = (f32)self->physical_ifc()->add_dynamic_pendulum(the_pendulum);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8537F0
bool slf__entity__physical_ifc_add_dynamic_pendulum__entity__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self         = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto pivot        = arch_base_vhandle(stack.local_uint(0x04));
    auto length       = stack.local_num(0x08);
    auto attach_point = (const vector3d*)&stack.local_uint(0x0C);

    // the pendulum's index, -1 without one
    f32 result = -1.0f;

    if (self && pivot.resolve() && self->has_ifc(entity_ifc_physical)) {
        auto* the_pendulum = new (mm::allocate(sizeof(pendulum))) pendulum();

        the_pendulum->set_entity_base_pivot((entity*)pivot.resolve());

        // a negative length keeps the current distance to the pivot
        if (length < 0.0f) {
            vector3d position  = self->my_abs_po->get_position();
            vector3d pivot_pos = the_pendulum->get_pivot_abs_pos();

            math::vector4 rope((f32)((f64)pivot_pos.x - position.x),
                               (f32)((f64)pivot_pos.y - position.y),
                               (f32)((f64)pivot_pos.z - position.z),
                               0.0f - 0.0f);

            the_pendulum->set_constraint((f32)retail::sub_42A9E0((f32*)&rope));
        } else
            the_pendulum->set_constraint(length);

        the_pendulum->attach_point = *attach_point;
        the_pendulum->enable(true);

        result = (f32)self->physical_ifc()->add_dynamic_pendulum(the_pendulum);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8539C0
bool slf__entity__physical_ifc_remove_dynamic_pendulum__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    if (!self || !self->has_ifc(entity_ifc_physical))
        return true;

    if (pendulum* the_pendulum = self->physical_ifc()->remove_dynamic_pendulum((i32)index))
        mm::free(the_pendulum);

    return true;
}

// sub_883960
bool slf__entity__physical_ifc_set_dynamic_pendulum_length__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index  = stack.local_num(0x04);
    auto length = stack.local_num(0x08);

    if (!self || !self->has_ifc(entity_ifc_physical))
        return true;

    if (pendulum* the_pendulum = self->physical_ifc()->get_dynamic_pendulum((i32)index))
        the_pendulum->set_constraint(length);

    return true;
}

// sub_8839B0
bool slf__entity__physical_ifc_get_dynamic_pendulum_length__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_physical)) {
        if (pendulum* the_pendulum = self->physical_ifc()->get_dynamic_pendulum((i32)index))
            result = the_pendulum->constraint;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_883A20
bool slf__entity__physical_ifc_set_dynamic_pendulum_attach_limb__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);
    auto limb  = stack.local_num(0x08);

    if (!self || !self->has_ifc(entity_ifc_physical))
        return true;

    if (pendulum* the_pendulum = self->physical_ifc()->get_dynamic_pendulum((i32)index))
        the_pendulum->set_attach_limb((i32)limb);

    return true;
}

// sub_883A70
bool slf__entity__physical_ifc_get_dynamic_pendulum_attach_limb__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);

    // -1 without an enabled pendulum
    f32 result = -1.0f;

    if (self && self->has_ifc(entity_ifc_physical)) {
        pendulum* the_pendulum = self->physical_ifc()->get_dynamic_pendulum((i32)index);

        if (the_pendulum && the_pendulum->enabled)
            result = (f32)the_pendulum->attach_limb;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_883AF0
bool slf__entity__physical_ifc_set_dynamic_pendulum_maintain_rel_orient__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);
    auto on    = stack.local_num(0x08);

    if (!self || !self->has_ifc(entity_ifc_physical))
        return true;

    if (pendulum* the_pendulum = self->physical_ifc()->get_dynamic_pendulum((i32)index))
        the_pendulum->set_maintain_rel_orient(on > 0.0001f);

    return true;
}

// sub_883B70
bool slf__entity__physical_ifc_set_dynamic_pendulum_maintain_abs_orient__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto index = stack.local_num(0x04);
    auto on    = stack.local_num(0x08);

    if (!self || !self->has_ifc(entity_ifc_physical))
        return true;

    if (pendulum* the_pendulum = self->physical_ifc()->get_dynamic_pendulum((i32)index))
        the_pendulum->set_maintain_abs_orient(on > 0.0001f);

    return true;
}

// sub_879DA0
bool slf__entity__set_puppet_slave_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // the receiver resolves twice, the second answer is used
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto slaves = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);

    ai_core* core = self ? self->get_ai_core() : nullptr;

    if (!core)
        return true;

    auto* puppet = (std_puppet_inode*)core->get_info_node(info_node_type_std_puppet);

    if (!puppet)
        return true;

    puppet->slaves.clear();

    for (i32 index = 0; index < (i32)slaves->size(); ++index)
        puppet->slaves.push_back(arch_base_vhandle(*(u32*)(*slaves)[index].stuff));

    return true;
}

// sub_853A20
bool slf__entity__get_floor_offset__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self           = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto allow_override = stack.local_num(0x04);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_standing)) {
        if (allow_override > 0.0001f && !(self->standing_ifc()->floor_offset_override <= 0.0f))
            result = self->standing_ifc()->floor_offset_override;
        else
            result = self->standing_ifc()->get_pure_anim_floor_offset();
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_853AB0
bool slf__entity__set_floor_offset_override__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self         = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto new_override = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_standing))
        self->standing_ifc()->set_floor_offset_override(new_override);

    return true;
}

// sub_853B00
bool slf__entity__can_be_web_yanked__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_actor() && self->get_ai_core()) {
        string_hash allow_web_yank;
        allow_web_yank.initialize(mash::ALLOCATED, "allow_web_yank");

        if (self->get_ai_core()->my_param_block.get_optional_pb_int(allow_web_yank, 1, nullptr))
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_853BD0
bool slf__entity__get_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result(0.0f, 0.0f, 0.0f);

    // the physics' while they move the entity, else the body its prop keeps, a player's own or the actor's
    if (self) {
        if (self->has_ifc(entity_ifc_physical)) {
            if (self->physical_ifc()->is_prop_physics_running() ||
                self->physical_ifc()->is_flagged(physical_interface_flag_biped_running) ||
                (self->physical_ifc()->is_enabled() && !self->physical_ifc()->is_suspended() &&
                 (!retail::sub_4024E0((u32*)self) || // has_standing_ifc
                  !self->standing_ifc()->is_enabled() ||
                  !self->standing_ifc()->is_effectively_standing())))

                result = self->physical_ifc()->get_velocity();
            else if (retail::sub_402500((u32*)self)) // has_player_ifc
                result = *self->player_ifc()->unk_095();
            else
                retail::sub_614190((u32*)self, (f32*)&result);
        } else if (self->is_an_actor()) {
            if (rigid_body_prop_head* head = prop_system::find_rigid_body_prop_head(self)) {
                if (head->m_first_rbpi) {
                    const vector4d &t_vel = head->m_first_rbpi->m_rb->m_t_vel;

                    result = vector3d(t_vel.x, t_vel.y, t_vel.z);
                }
            } else if (self->has_ifc(entity_ifc_player))
                result = *self->player_ifc()->unk_095();
            else
                retail::sub_614190((u32*)self, (f32*)&result);
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_853DC0
bool slf__entity__set_crawlable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self)
        self->set_ext_flag_recursive(entity_ext_flag_noncrawlable, on == 0.0f);

    return true;
}

// sub_853E10
bool slf__entity__in_combat_state__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    ai_core* core = self && self->is_an_actor() ? self->get_ai_core() : nullptr;

    f32 result = -1.0f;

    if (core && core->my_base_machine) {
        base_state* state = core->my_base_machine->my_curr_state;

        result = state && state->is_combat_state() ? 1.0f : 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_853EC0
bool slf__entity__set_tentacle_end_target__uint__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto which  = stack.local_uint(0x04);
    auto target = arch_base_vhandle(stack.local_uint(0x08));

    // no tentacle interface test
    if (self)
        self->tentacle_ifc()->set_tentacle_end_target(which, target);

    return true;
}

// sub_853F00
bool slf__entity__set_tentacle_default_pct__uint__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto which = stack.local_uint(0x04);
    auto pct   = stack.local_num(0x08);

    if (self)
        self->tentacle_ifc()->set_tentacle_default_pct(which, pct);

    return true;
}

// sub_853F40
bool slf__entity__set_tentacle_effect__uint__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self        = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto which       = stack.local_uint(0x04);
    auto effect_name = (const char*)stack.local_uint(0x08);

    if (!self)
        return true;

    string_hash effect;
    effect.initialize(mash::ALLOCATED, effect_name);

    self->tentacle_ifc()->set_tentacle_effect(which, effect);

    return true;
}

// sub_853F90
bool slf__entity__play_tentacle_effect__uint__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self        = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto which       = stack.local_uint(0x04);
    auto effect_name = (const char*)stack.local_uint(0x08);

    if (!self)
        return true;

    string_hash effect;
    effect.initialize(mash::ALLOCATED, effect_name);

    self->tentacle_ifc()->play_tentacle_effect(which, effect);

    return true;
}

// sub_853FE0
bool slf__entity__spawn_effect__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self        = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto effect_name = (const char*)stack.local_uint(0x04);

    if (!self)
        return true;

    string_hash effect;
    effect.initialize(mash::ALLOCATED, effect_name);

    self->spawn_effect(effect, 0);

    return true;
}

// sub_854030
bool slf__entity__spawn_effect_at_entity_position__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self        = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto effect_name = (const char*)stack.local_uint(0x04);
    auto position    = (const vector3d*)&stack.local_uint(0x08);

    if (!self)
        return true;

    vector3d local = self->my_abs_po->inverse_xform(*position);

    string_hash effect;
    effect.initialize(mash::ALLOCATED, effect_name);

    self->spawn_effect_at_entity_position(effect, vector4d(local.x, local.y, local.z, 0.0f), 0);

    return true;
}

// sub_8542E0
bool slf__entity__spawn_effect_at_world_position__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self        = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto effect_name = (const char*)stack.local_uint(0x04);
    auto position    = (const vector3d*)&stack.local_uint(0x08);

    if (!self)
        return true;

    vector4d position_4(position->x, position->y, position->z, 0.0f);

    string_hash effect;
    effect.initialize(mash::ALLOCATED, effect_name);

    self->spawn_effect_at_world_position(effect, position_4, 0);

    return true;
}

// sub_854360
bool slf__entity__spawn_looping_effect__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self        = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto effect_name = (const char*)stack.local_uint(0x04);

    u32 result = 0;

    if (self) {
        string_hash effect;
        effect.initialize(mash::ALLOCATED, effect_name);

        result = self->spawn_looping_effect(effect, 0);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8543E0
bool slf__entity__stop_looping_effect__uint__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto id   = stack.local_uint(0x04);

    if (self)
        self->stop_looping_effect(id);

    return true;
}

// sub_854410
bool slf__entity__stop_all_effects__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->has_ifc(entity_ifc_ise))
        self->ise_ifc()->stop_all_effects();

    return true;
}

// sub_854450
bool slf__entity__get_teleport_target_position__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result(0.0f, 0.0f, 0.0f);

    // the core isn't tested
    if (self && self->is_an_actor()) {
        if (auto* teleport = (teleporting_layer_inode*)self->get_ai_core()->get_info_node(info_node_type_teleporting_layer))
            result = teleport->get_teleport_target_pos();
    }

    stack.push(&result, sizeof(result));

    return true;
}
