#include <cmath>
#include <cstring>

#include "retail.hh"
#include "game/amalga/resource_manager.hh"
#include "game/amalga/resource_types.hh"
#include "game/arch_base.hh"
#include "game/colgeom.hh"
#include "game/physics/physics_system.hh"
#include "game/physics/rigid_body_prop_system.hh"
#include "game/script/chuck_lib_entity.hh"
#include "game/script/chuck_lib_entity2.hh"
#include "game/wds/ai/ai_core.hh"
#include "game/wds/ai/ai_weapon_inode.hh"
#include "game/wds/ai/info_node.hh"
#include "game/wds/camera/camera.hh"
#include "game/wds/entity/actor.hh"
#include "game/wds/entity/conglomerate.hh"
#include "game/wds/entity/entity.hh"
#include "game/wds/entity/pendulum.hh"
#include "game/wds/entity/vehicle.hh"
#include "game/wds/references.hh"
#include "game/wds/world_dynamics_system.hh"
#include "game/math/references.hh"
#include "treyarch/chuck/vm/vm_dynamic_array_manager.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/math/trig.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mm/mm.hh"
#include "vc8/vector.hh"

using namespace treyarch;

// sub_83DAE0
u32 slc_player_entity_t::find_instance(const mash::string &name) const {
    if (!std::strncmp(name.c_str(), "NULL", 0xFFFF))
        return 0;

    string_hash hash;

    hash.initialize(mash::ALLOCATED, name.c_str());

    auto* entity = (arch_base*)retail::sub_640AE0(&hash.source_hash_code, 28, true); // entity_handle_manager::find_entity

    if (!entity)
        return 0;

    return entity->my_handle.value;
}

// sub_882BD0
bool slf__rvb_find_entities_in_radius__dynamic_array__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto position = (const vector3d*)&stack.local_uint(0x04);

    if (!(references::rtree_search_name_guard.read() & 1)) {
        references::rtree_search_name_guard.write(references::rtree_search_name_guard.read() | 1);
        references::rtree_search_name.get().initialize(mash::ALLOCATED, "rtree_search");
    }

    auto flags = (i32)stack.local_num(0x14);

    if (!flags)
        flags = 1;

    auto results = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x00);
    results->clear();

    // the radius argument itself is clamped
    if (stack.local_num(0x10) > 200.0)
        stack.local_num(0x10) = 200.0f;

    entity::lock_found_entities();

    auto count = (f32)((i32 (__cdecl*)(const vector3d*, f32, i32))retail::sub_645150)(position, stack.local_num(0x10), flags); // entity search through the rtree

    if (count > 0.0f) {
        entity::entity_search_list* found = references::found_entities.read();

        for (auto* node = found->begin(); node != found->end(); node = node->next) {
            arch_base_vhandle handle = node->value->my_handle;

            chuck::vm::chuck_dynamic_array_element_t cell;
            results->push_back(cell);

            *(arch_base_vhandle*)results->back().stuff = handle;
        }
    }

    entity::unlock_found_entities();

    stack.push(&count, sizeof(count));

    return true;
}

// sub_87A650
bool slf__rvb_find_cop_patrol_dude__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto position = (const vector3d*)&stack.local_uint(0x00);

    if (!(references::rvb_find_cop_patrol_dude_name_guard.read() & 1)) {
        references::rvb_find_cop_patrol_dude_name_guard.write(references::rvb_find_cop_patrol_dude_name_guard.read() | 1);
        references::rvb_find_cop_patrol_dude_name.get().initialize(mash::ALLOCATED, "rvb_find_cop_patrol_dude");
    }

    entity* result = nullptr;

    entity::lock_found_entities();

    if (((i32 (__cdecl*)(const vector3d*, f32, i32))retail::sub_645150)(position, stack.local_num(0x0C), 0x81010) > 0) { // entity search through the rtree
        entity::entity_search_list* found = references::found_entities.read();

        for (auto* node = found->begin(); node != found->end(); node = node->next) {
            entity* candidate = node->value;

            if (!candidate)
                continue;

            u32  unk_choice = stack.local_num(0x14) != 0.0f;
            bool qualifies;

            // weighs the candidate's Combat_ID
            __asm {
                push unk_choice
                mov  ecx, candidate
                call retail::sub_8401F0
                add  esp, 4
                mov  qualifies, al
            }

            if (!qualifies)
                continue;

            bool    in_vehicle = false;
            entity* vehicle;

            if (candidate->is_an_actor() && retail::sub_4CB950((i32)candidate) != -1) { // get_car_seat
                in_vehicle = true;

                __asm {
                    mov  eax, candidate
                    call retail::sub_8404F0
                    mov  vehicle, eax
                }
            } else {
                __asm {
                    mov  eax, candidate
                    call retail::sub_840570
                    mov  vehicle, eax
                }

                if (vehicle)
                    in_vehicle = true;
            }

            if (stack.local_num(0x10) == 0.0f) {
                if (!in_vehicle) {
                    result = candidate;

                    break;
                }
            } else if (in_vehicle) {
                f32 height = (f32)((f64)candidate->my_abs_po->get_position().y - position->y);

                if (std::fabs(height) < 10.0f) {
                    result = vehicle;

                    break;
                }
            }
        }
    }

    entity::unlock_found_entities();

    arch_base_vhandle handle = result ? result->my_handle : arch_base_vhandle();

    stack.push(&handle, sizeof(handle));

    return true;
}

// sub_882D10
bool slf__find_entities_in_radius_LEGACY__dynamic_array__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto results  = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x00);
    auto position = (const vector3d*)&stack.local_uint(0x04);

    if (!results) {
        f32 none = 0.0f;
        stack.push(&none, sizeof(none));

        return true;
    }

    auto flags = (u32)(i32)stack.local_num(0x14);

    if (!flags)
        flags = 1;

    results->clear();

    entity::lock_found_entities();

    i32 count = ((i32 (__cdecl*)(u32, const vector3d*, f32, region*))retail::sub_6452A0)(flags, position, stack.local_num(0x10), nullptr); // entity::find_entities

    if (count <= 0)
        count = 0;
    else {
        entity::entity_search_list* found = references::found_entities.read();

        for (auto* node = found->begin(); node != found->end(); node = node->next) {
            arch_base_vhandle handle = node->value->my_handle;

            chuck::vm::chuck_dynamic_array_element_t cell;
            results->push_back(cell);

            *(arch_base_vhandle*)results->back().stuff = handle;
        }
    }

    entity::unlock_found_entities();

    f32 result = (f32)count;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86FFA0
bool slf__get_entities_in_pack__str__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    amalga::e_resource_partition partitions[3] = { amalga::resource_partition_common,
                                                   amalga::resource_partition_hero,
                                                   amalga::resource_partition_game };

    // looks the partitions up and stops there; nothing is listed or pushed
    for (i32 i = 0; i < 3; ++i)
        amalga::resource_manager::get_partition_pointer(partitions[i]);

    return true;
}

// sub_870840
bool slf__find_best_prop_entity__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x24;

    f32 min_dot = math::cos((f32)(stack.local_num(0x20) * 0.01745329300562541));

    auto find_best_prop_phys_target = (entity* (__cdecl*)(const vector3d*, const vector3d*, const vector3d*, f32, f32, f32, f32, f32, f32, void*, void*, void*, bool, i32))retail::sub_470ED0;

    entity* target = find_best_prop_phys_target((const vector3d*)&stack.local_uint(0x00),
                                                &references::unk_00f2213c.get(),
                                                (const vector3d*)&stack.local_uint(0x0C),
                                                stack.local_num(0x18),
                                                stack.local_num(0x1C),
                                                -100.0f,
                                                100.0f,
                                                min_dot,
                                                min_dot,
                                                nullptr,
                                                nullptr,
                                                nullptr,
                                                false,
                                                0);

    arch_base_vhandle result = target ? target->my_handle : arch_base_vhandle();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_870940
bool slf__find_best_damageable_entity__entity__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x28;

    auto whos_asking = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result;

    if (whos_asking) {
        f32 min_dot = math::cos((f32)(stack.local_num(0x24) * 0.01745329300562541));

        auto find_best_damageable_target = (entity* (__cdecl*)(entity*, const vector3d*, const vector3d*, f32, f32, f32, f32, f32, f32, void*, void*, vc8::vector<arch_base_vhandle>*, bool, i32))retail::sub_5484B0;

        entity* target = find_best_damageable_target(whos_asking,
                                                     (const vector3d*)&stack.local_uint(0x04),
                                                     (const vector3d*)&stack.local_uint(0x10),
                                                     stack.local_num(0x1C),
                                                     stack.local_num(0x20),
                                                     -100.0f,
                                                     100.0f,
                                                     min_dot,
                                                     min_dot,
                                                     nullptr,
                                                     nullptr,
                                                     nullptr,
                                                     true,
                                                     0);

        if (target)
            result = target->my_handle;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_883000
bool slf__find_best_damageable_entity__entity__vector3d__vector3d__num__num__num__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x2C;

    auto whos_asking = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result;

    if (!whos_asking) {
        stack.push(&result, sizeof(result));

        return true;
    }

    f32 min_dot = math::cos((f32)(stack.local_num(0x24) * 0.01745329300562541));

    vc8::vector<arch_base_vhandle> listed;

    chuck::vm::script_dynamic_array<arch_base_vhandle> entities((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x28));

    for (auto* cell = entities.array->begin(); cell != entities.array->end(); ++cell)
        listed.push_back(*(arch_base_vhandle*)cell->stuff);

    auto find_best_damageable_target = (entity* (__cdecl*)(entity*, const vector3d*, const vector3d*, f32, f32, f32, f32, f32, f32, void*, void*, vc8::vector<arch_base_vhandle>*, bool, i32))retail::sub_5484B0;

    entity* target = find_best_damageable_target(whos_asking,
                                                 (const vector3d*)&stack.local_uint(0x04),
                                                 (const vector3d*)&stack.local_uint(0x10),
                                                 stack.local_num(0x1C),
                                                 stack.local_num(0x20),
                                                 -100.0f,
                                                 100.0f,
                                                 min_dot,
                                                 min_dot,
                                                 nullptr,
                                                 nullptr,
                                                 &listed,
                                                 true,
                                                 0);

    if (target)
        result = target->my_handle;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_870A90
bool slf__find_best_manip_obj__vector3d__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x24;

    f32 min_dot = math::cos((f32)(stack.local_num(0x20) * 0.01745329300562541));

    auto find_best_ready_manip_obj = (manip_obj_interface* (__cdecl*)(actor*, const vector3d*, const vector3d*, f32, f32, f32, f32, f32, f32, void*, void*))retail::sub_63FAA0;

    manip_obj_interface* target = find_best_ready_manip_obj(nullptr,
                                                            (const vector3d*)&stack.local_uint(0x00),
                                                            (const vector3d*)&stack.local_uint(0x0C),
                                                            stack.local_num(0x18),
                                                            stack.local_num(0x1C),
                                                            -100.0f,
                                                            100.0f,
                                                            min_dot,
                                                            min_dot,
                                                            nullptr,
                                                            nullptr);

    arch_base_vhandle result = target ? ((entity*)target->my_actor)->my_handle : arch_base_vhandle();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_869FC0
bool slf__entity__set_path_graph__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    if (!self)
        return true;

    // an empty name clears the param, one no loaded path graph has changes nothing
    if (!name || !std::strlen(name))
        self->get_ai_core()->my_param_block.set_pb_fixedstring(path_graph_key, "");
    else {
        resource_key key;
        retail::sub_739B10((u32*)&key, name, (i32)amalga::e_resource_type::path);

        if (!references::g_world_ptr.read()->ai_mgr.path_graph_exists(key))
            return true;

        self->get_ai_core()->my_param_block.set_pb_fixedstring(path_graph_key, name);
    }

    return true;
}

// sub_86A0A0
bool slf__entity__ai_can_carry__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (actor*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    f32 result = 0.0f;

    if (self && target && self->is_an_actor() && self->get_ai_core() && target->is_an_actor() && target->get_ai_core()) {
        auto* carrier = (std_carry_inode*)self  ->get_ai_core()->get_info_node(info_node_type_std_carry);
        auto* slave   = (std_carry_inode*)target->get_ai_core()->get_info_node(info_node_type_std_carry);

        // neither side may carry or be carried already
        if (carrier && slave &&
            !carrier->is_being_carried() && !carrier->has_carried_objects() &&
            !slave  ->is_being_carried() && !slave  ->has_carried_objects())

            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86A1B0
bool slf__entity__ai_carry_pickup__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (actor*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    if (!self || !target)
        return true;

    // the actor tests look the core up and drop it
    if (self->is_an_actor())
        self->get_ai_core();

    if (target->is_an_actor())
        target->get_ai_core();

    auto* carrier = (std_carry_inode*)self  ->get_ai_core()->get_info_node(info_node_type_std_carry);
    auto* slave   = (std_carry_inode*)target->get_ai_core()->get_info_node(info_node_type_std_carry);

    carrier->add_slave(slave, true);

    return true;
}

// sub_86A2A0
bool slf__entity__ai_carry_putdown__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    arch_base_vhandle target_handle(stack.local_uint(0x04));

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (actor*)target_handle.resolve();

    // a null handle puts down every slave, a stale one nothing
    if (!self || (!target && target_handle.value))
        return true;

    // the actor tests look the core up and drop it
    if (self->is_an_actor())
        self->get_ai_core();

    auto* carrier = (std_carry_inode*)self->get_ai_core()->get_info_node(info_node_type_std_carry);

    std_carry_inode* slave = nullptr;

    if (target) {
        if (target->is_an_actor())
            target->get_ai_core();

        slave = (std_carry_inode*)target->get_ai_core()->get_info_node(info_node_type_std_carry);
    }

    carrier->remove_slave(slave, true);

    return true;
}

// sub_86A3B0
bool slf__entity__ai_is_carrying__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self) {
        // the actor test looks the core up and drops it
        if (self->is_an_actor())
            self->get_ai_core();

        auto* carry = (std_carry_inode*)self->get_ai_core()->get_info_node(info_node_type_std_carry);

        if (carry && carry->has_carried_objects())
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86A470
bool slf__entity__ai_is_carried__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self) {
        // the actor test looks the core up and drops it
        if (self->is_an_actor())
            self->get_ai_core();

        auto* carry = (std_carry_inode*)self->get_ai_core()->get_info_node(info_node_type_std_carry);

        if (carry && carry->is_being_carried())
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86A520
bool slf__entity__set_ai_subclass__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    // the receiver resolves twice, the core is looked up and dropped and the name is never read
    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve())
        self->get_ai_core();

    return true;
}

// sub_86A560
bool slf__entity__set_render_scale__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto scale = (const vector3d*)&stack.local_uint(0x04);

    if (self && self->is_an_entity())
        self->set_render_scale(*scale);

    return true;
}

// sub_86A5A0
bool slf__entity__wait_change_render_scale__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    struct wait_state {
        vector3d start;
        f32      elapsed;
    };

    auto* state = pop_wait_arguments<wait_state>(stack, 0x14);

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target   = (const vector3d*)&stack.local_uint(0x04);
    auto duration = stack.local_num(0x10);

    if (!self)
        return true;

    if (!stack.thread->entry) {
        state->elapsed = 0.0f;
        state->start   = self->is_an_entity() ? self->get_render_scale() : vector3d(1.0f, 1.0f, 1.0f);

        return false;
    }

    state->elapsed += get_wait_time_step(self);

    if (state->elapsed > duration)
        state->elapsed = duration;

    f32 t = state->elapsed / duration;

    vector3d scale(state->start.x + (target->x - state->start.x) * t,
                   state->start.y + (target->y - state->start.y) * t,
                   state->start.z + (target->z - state->start.z) * t);

    if (self->is_an_entity())
        self->set_render_scale(scale);

    if (duration > state->elapsed)
        return false;

    return true;
}

// sub_86A840
bool slf__entity__set_render_color__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto rgb  = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !self->is_an_entity())
        return true;

    color32 color = self->get_render_color();

    color.set_red_float  (rgb->x);
    color.set_green_float(rgb->y);
    color.set_blue_float (rgb->z);

    self->set_render_color(color);

    return true;
}

// sub_86A930
bool slf__entity__set_render_alpha__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto  self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto &alpha = stack.local_num(0x04);

    if (!self || !self->is_an_entity())
        return true;

    // clamped on the stack first
    if (0.0f > alpha)
        alpha = 0.0f;
    else if (alpha > 1.0f)
        alpha = 1.0f;

    color32 color = self->get_render_color();

    color.set_alpha_float(alpha);

    self->set_render_color(color);

    return true;
}

// sub_86A9E0
bool slf__entity__get_render_color__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result(1.0f, 1.0f, 1.0f);

    if (self && self->is_an_entity()) {
        color32 color = self->get_render_color();

        result = vector3d((f32)(color.r / 255.0), (f32)(color.g / 255.0), (f32)(color.b / 255.0));
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86AAD0
bool slf__entity__get_render_scale__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    vector3d result = self && self->is_an_entity() ? self->get_render_scale() : vector3d(1.0f, 1.0f, 1.0f);

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86AB60
bool slf__entity__get_render_alpha__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() ? (f32)(self->get_render_color().a / 255.0) : 1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86ABF0
bool slf__entity__wait_change_render_color__vector3d__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    struct wait_state {
        color start;
        f32   elapsed;
    };

    auto* state = pop_wait_arguments<wait_state>(stack, 0x18);

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target   = (const color*)&stack.local_uint(0x04);
    auto duration = stack.local_num(0x14);

    if (!self || !self->is_an_entity())
        return true;

    if (!stack.thread->entry) {
        state->elapsed = 0.0f;

        color32 current = self->get_render_color();

        state->start.r = current.r * (1.0f / 255.0f);
        state->start.g = current.g * (1.0f / 255.0f);
        state->start.b = current.b * (1.0f / 255.0f);
        state->start.a = current.a * (1.0f / 255.0f);

        return false;
    }

    state->elapsed += get_wait_time_step(self);

    if (state->elapsed > duration)
        state->elapsed = duration;

    color current = *target;

    // the alpha takes the unrounded fraction
    if (duration > 0.0f) {
        f64 fraction = (f64)state->elapsed / (f64)duration;
        f32 t        = (f32)fraction;

        current.r = state->start.r + (target->r - state->start.r) * t;
        current.g = state->start.g + (target->g - state->start.g) * t;
        current.b = state->start.b + (target->b - state->start.b) * t;
        current.a = (f32)((f64)(target->a - state->start.a) * fraction) + state->start.a;
    }

    color32 packed;

    packed.set_clamped((i32)((f64)current.r * 255.0),
                       (i32)((f64)current.g * 255.0),
                       (i32)((f64)current.b * 255.0),
                       (i32)((f64)current.a * 255.0));

    self->set_render_color(packed);

    if (duration > state->elapsed)
        return false;

    return true;
}

// sub_86AF70
bool slf__entity__set_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto scale = stack.local_num(0x04);

    if (!self || !(scale > 0.0f))
        return true;

    f32 factor = scale / self->my_rel_po->get_x_facing().length();

    po       scaling = references::unk_010f76b0.read();
    vector4d factors(factor, factor, factor, 0.0f);

    retail::sub_48E170((u64*)&scaling, (u32*)&factors); // the scale rows

    // scaled about the origin, then put back
    po       rel_po   = *self->my_rel_po;
    vector3d position = rel_po.get_position();
    vector3d origin   = references::unk_010f7140.read();

    rel_po.get_matrix().w = vector4d(origin.x, origin.y, origin.z, 1.0f);
    retail::sub_44E230((u64*)&rel_po, (f32*)&rel_po, (f32*)&scaling);
    rel_po.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

    retail::sub_626BE0((i32)self, (u64*)&rel_po); // entity_base::set_rel_po

    return true;
}

// sub_86B180
bool slf__player_entity__get_player_num__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self && self->has_ifc(entity_ifc_player)) {
        auto ifc = self->player_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        f32 value;
        result = ifc->get_ifc_num(hash, &value, false) ? value : 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86B240
bool slf__player_entity__set_player_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = stack.local_num(0x08);

    if (self && self->has_ifc(entity_ifc_player)) {
        auto ifc = self->player_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        ifc->set_ifc_num(hash, value, false);
    }

    return true;
}

// sub_86B2A0
bool slf__player_entity__get_player_vec__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    vector3d result;

    if (self && self->has_ifc(entity_ifc_player)) {
        auto ifc = self->player_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        vector3d value;
        result = ifc->get_ifc_vec(hash, &value, false) ? value : references::unk_010f7140.get();
    } else
        result = references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86B360
bool slf__player_entity__is_button_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto button = (i32)stack.local_num(0x04);

    f32 result = 0.0f;

    // the range test passes every button
    if (self && self->has_ifc(entity_ifc_player) && (button > 0 || button < 25))
        result = self->player_ifc()->get_button(button)->is_enabled() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86B400
bool slf__player_entity__set_button_enabled__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto button = (i32)stack.local_num(0x04);
    auto on     = stack.local_num(0x08);

    if (self && self->has_ifc(entity_ifc_player) && button > 0 && button < 25 && button < 13)
        self->player_ifc()->enable_button(button, on != 0.0f);

    return true;
}

// sub_86B470
bool slf__player_entity__set_buttons_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self || !self->has_ifc(entity_ifc_player))
        return true;

    for (i32 button = 1; button < 25; ++button)
        self->player_ifc()->enable_button(button, on != 0.0f);

    return true;
}

// sub_86B4F0
bool slf__player_entity__set_swing_override_anchor__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto anchor = arch_base_vhandle(stack.local_uint(0x04));

    if (self && self->has_ifc(entity_ifc_player))
        self->player_ifc()->set_swing_override_anchor(anchor);

    return true;
}

// sub_86B540
bool slf__player_entity__set_zip_override_anchor__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto anchor = arch_base_vhandle(stack.local_uint(0x04));
    auto num    = stack.local_num(0x08);

    if (!self || !self->has_ifc(entity_ifc_player))
        return true;

    self->player_ifc()->set_zip_override_anchor(anchor);
    self->player_ifc()->set_zip_override_anchor_num(num);

    return true;
}

// sub_86B5B0
bool slf__player_entity__clear_buttons__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->has_ifc(entity_ifc_player))
        return true;

    // the whole controller gets cleared 24 times over
    for (i32 count = 24; count; --count)
        self->player_ifc()->clear_buttons();

    return true;
}

// sub_86B600
bool slf__player_entity__clear_controller__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->has_ifc(entity_ifc_player))
        self->player_ifc()->clear_controller(false);

    return true;
}

// sub_86B640
bool slf__player_entity__set_override_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self || !self->has_ifc(entity_ifc_player))
        return true;

    auto ifc = self->player_ifc();

    ifc->clear_buttons();
    ifc->override_mode = on != 0.0f;

    return true;
}

// sub_86B6B0
bool slf__player_entity__override_button__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto button = (i32)stack.local_num(0x04);
    auto value  = stack.local_num(0x08);
    auto t      = stack.local_num(0x0C);

    if (self && self->has_ifc(entity_ifc_player) && button > 0 && button < 25 && button < 13)
        self->player_ifc()->override_button(button, value, t);

    return true;
}

// sub_86B710
bool slf__player_entity__override_command__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self    = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto command = (i32)stack.local_num(0x04);
    auto value   = stack.local_num(0x08);
    auto t       = stack.local_num(0x0C);

    if (self && self->has_ifc(entity_ifc_player) && command >= 0 && command < 89)
        self->player_ifc()->override_command(command, value, t);

    return true;
}

// sub_86B770
bool slf__player_entity__set_button__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto button = (i32)stack.local_num(0x04);
    auto value  = stack.local_num(0x08);

    if (self && self->has_ifc(entity_ifc_player) && button > 0 && button < 25 && button < 13)
        self->player_ifc()->set_button(button, value);

    return true;
}

// sub_86B7D0
bool slf__player_entity__set_axis__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto axis  = (i32)stack.local_num(0x04);
    auto ud    = stack.local_num(0x08);
    auto value = stack.local_num(0x0C);

    if (self && self->has_ifc(entity_ifc_player) && axis >= 0 && axis < 4)
        self->player_ifc()->set_axis(axis, ud != 0.0f, value);

    return true;
}

// sub_86B840
bool slf__player_entity__set_command__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self    = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto command = (i32)stack.local_num(0x04);
    auto value   = stack.local_num(0x08);

    if (!self || !self->has_ifc(entity_ifc_player) || command < 0 || command >= 89)
        return true;

    game_button* button = self->player_ifc()->get_command_button(command);

    button->state  = value;
    button->unk_1c = value;

    return true;
}

// sub_86B8B0
bool slf__player_entity__set_axis_enabled__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto axis = (i32)stack.local_num(0x04);
    auto on   = stack.local_num(0x08);

    if (self && self->has_ifc(entity_ifc_player) && axis >= 0 && axis < 4)
        self->player_ifc()->enable_axis(axis, on != 0.0f);

    return true;
}

// sub_86B920
bool slf__player_entity__set_axis_positive_enabled__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto axis = (i32)stack.local_num(0x04);
    auto lr   = stack.local_num(0x08);
    auto ud   = stack.local_num(0x0C);

    if (!self || !self->has_ifc(entity_ifc_player) || axis < 0 || axis >= 4)
        return true;

    game_axis* control = self->player_ifc()->control_axis[axis];

    control->axis_lr->set_flag(game_button::gbflag_disallow_pos_state, !(lr > 0.0001));
    control->axis_ud->set_flag(game_button::gbflag_disallow_pos_state, !(ud > 0.0001));

    return true;
}

// sub_86B9C0
bool slf__player_entity__set_axis_negative_enabled__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto axis = (i32)stack.local_num(0x04);
    auto lr   = stack.local_num(0x08);
    auto ud   = stack.local_num(0x0C);

    if (!self || !self->has_ifc(entity_ifc_player) || axis < 0 || axis >= 4)
        return true;

    game_axis* control = self->player_ifc()->control_axis[axis];

    control->axis_lr->set_flag(game_button::gbflag_allow_neg_state, lr > 0.0001);
    control->axis_ud->set_flag(game_button::gbflag_allow_neg_state, ud > 0.0001);

    return true;
}

// sub_86BA60
bool slf__player_entity__set_axis_direction_enabled__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto axis = (i32)stack.local_num(0x04);
    auto lr   = stack.local_num(0x08);
    auto ud   = stack.local_num(0x0C);

    if (!self || !self->has_ifc(entity_ifc_player) || axis < 0 || axis >= 4)
        return true;

    game_axis* control = self->player_ifc()->control_axis[axis];

    control->axis_lr->enable(lr > 0.0001);
    control->axis_ud->enable(ud > 0.0001);

    return true;
}

// sub_86BB00
bool slf__entity__get_custom_ent__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        arch_base_vhandle value;

        if (ifc->get_custom_ent(hash, &value, false))
            result = value;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86BBF0
bool slf__entity__set_custom_ent__str__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = arch_base_vhandle(stack.local_uint(0x08));

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        ifc->set_custom_ent(hash, value, false);
    }

    return true;
}

// sub_86BC60
bool slf__entity__has_entity_in_custom_list__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data) ? (f32)self->script_data_ifc()->has_entity_in_custom_list() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86BCF0
bool slf__entity__add_entity_to_custom_entity_list__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto ent  = arch_base_vhandle(stack.local_uint(0x04));

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data))
        self->script_data_ifc()->add_entity_to_custom_entity_list(ent);

    return true;
}

// sub_86BD40
bool slf__entity__remove_entity_from_custom_entity_list__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto ent  = arch_base_vhandle(stack.local_uint(0x04));

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data))
        self->script_data_ifc()->remove_entity_from_custom_entity_list(ent);

    return true;
}

// sub_86BD90
bool slf__entity__get_first_entity_from_custom_entity_list__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data))
        result = self->script_data_ifc()->get_first_entity_from_custom_entity_list();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86BE20
bool slf__entity__get_custom_entity_list_count__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data) ? (f32)self->script_data_ifc()->get_custom_entity_list_count() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86BEB0
bool slf__entity__get_custom_num__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        f32 value;
        result = ifc->get_ifc_num(hash, &value, false) ? value : 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86BF60
bool slf__entity__get_optional_custom_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name     = (const char*)stack.local_uint(0x04);
    auto fallback = stack.local_num(0x08);

    f32 result = fallback;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        f32 value;
        result = ifc->get_ifc_num(hash, &value, false) ? value : fallback;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C020
bool slf__entity__set_custom_num__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = stack.local_num(0x08);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        ifc->set_ifc_num(hash, value, false);
    }

    return true;
}

// sub_86C090
bool slf__entity__get_custom_vec__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    vector3d result;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        vector3d value;
        result = ifc->get_ifc_vec(hash, &value, false) ? value : references::unk_010f7140.get();
    } else
        result = references::unk_010f7140.get();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C170
bool slf__entity__get_optional_custom_vec__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name     = (const char*)stack.local_uint(0x04);
    auto fallback = (const vector3d*)&stack.local_uint(0x08);

    vector3d result = *fallback;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        vector3d value;
        result = ifc->get_ifc_vec(hash, &value, false) ? value : *fallback;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C230
bool slf__entity__set_custom_vec__str__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = (const vector3d*)&stack.local_uint(0x08);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        ifc->set_ifc_vec(hash, *value, false);
    }

    return true;
}

// sub_86C2A0
bool slf__entity__get_custom_str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    const char* result = "";

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        const char* value;
        result = ifc->get_ifc_str(hash, &value, false) ? value : "";
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C380
bool slf__entity__get_optional_custom_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name     = (const char*)stack.local_uint(0x04);
    auto fallback = (const char*)stack.local_uint(0x08);

    const char* result = fallback;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        const char* value;
        result = ifc->get_ifc_str(hash, &value, false) ? value : fallback;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C450
bool slf__entity__set_custom_str__str__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name  = (const char*)stack.local_uint(0x04);
    auto value = (const char*)stack.local_uint(0x08);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_script_data)) {
        auto ifc = self->script_data_ifc();

        string_hash hash;
        hash.initialize(mash::ALLOCATED, name);

        ifc->set_ifc_str(hash, value, false);
    }

    return true;
}

// sub_86C4C0
bool slf__entity__get_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (f32)self->damage_ifc()->hit_points.val : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C550
bool slf__entity__set_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_hit_points((i32)value);

    return true;
}

// sub_86C5B0
bool slf__entity__set_hit_points_with_signal__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_hit_points((i32)value, true);

    return true;
}

// sub_86C610
bool slf__entity__get_max_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (f32)self->damage_ifc()->hit_points.max_val : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C6A0
bool slf__entity__get_damage_dealt__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (f32)self->damage_ifc()->damage_dealt : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C730
bool slf__entity__set_damage_dealt__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    damage_interface* damage = self->damage_ifc();

    damage->flags        |= damage_ifc_flag_dealt_damage_this_frame;
    damage->damage_dealt  = (i32)value;

    return true;
}

// sub_86C790
bool slf__entity__set_web_subdue_recover_duration__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (auto web = self->damage_ifc()->web_subdue)
        web->recover_duration = value;

    return true;
}

// sub_86C7E0
bool slf__entity__cancel_web_subdue__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->cancel_web_subdue();

    return true;
}

// sub_86C830
bool slf__entity__get_web_subdued__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)) {
        if (auto web = self->damage_ifc()->web_subdue)
            result = web->web_hit_points <= 0.0f ? 1.0f : 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C8F0
bool slf__entity__get_web_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)) {
        if (auto web = self->damage_ifc()->web_subdue)
            result = web->web_hit_points;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86C9A0
bool slf__entity__set_web_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (auto web = self->damage_ifc()->web_subdue)
        web->web_hit_points = value;

    return true;
}

// sub_86C9F0
bool slf__entity__get_max_web_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)) {
        if (auto web = self->damage_ifc()->web_subdue)
            result = web->max_web_hit_points;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86CAA0
bool slf__entity__set_armor_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_armor_points((i32)value);

    return true;
}

// sub_87A5B0
bool slf__entity__set_max_armor_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_max_armor_points((i32)value);

    return true;
}

// sub_86CAF0
bool slf__entity__get_armor_points__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (f32)self->damage_ifc()->armor_points.val : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86CB80
bool slf__entity__get_max_armor_points__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (f32)self->damage_ifc()->armor_points.max_val : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_87A600
bool slf__entity__set_max_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_max_hit_points((i32)value);

    return true;
}

// sub_86CC10
bool slf__entity__set_damage_modifier__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->damage_mod = value;

    return true;
}

// sub_86CC60
bool slf__entity__set_damage_lock_threshold__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->damage_lock_threshold = (i32)value;

    return true;
}

// sub_86CCB0
bool slf__entity__get_damage_lock_threshold__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (f32)self->damage_ifc()->damage_lock_threshold : -1.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86CD40
bool slf__entity__set_immune_to_death_from_damage__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto immune = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (immune > 0.5f)
        self->damage_ifc()->flags |= damage_ifc_flag_immune_to_death_from_damage;
    else
        self->damage_ifc()->flags &= ~damage_ifc_flag_immune_to_death_from_damage;

    return true;
}

// sub_86CDC0
bool slf__entity__get_immune_to_death_from_damage__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (self->damage_ifc()->flags & damage_ifc_flag_immune_to_death_from_damage ? 1.0f : 0.0f) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86CE90
bool slf__entity__get_stun_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)) {
        if (auto stun = self->damage_ifc()->stun)
            result = (f32)stun->stun_hp;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86CF40
bool slf__entity__set_stun_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (auto stun = self->damage_ifc()->stun)
        stun->stun_hp = (i32)value;

    return true;
}

// sub_86CFA0
bool slf__entity__get_stun_restore_hit_points__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)) {
        if (auto stun = self->damage_ifc()->stun)
            result = (f32)stun->stun_restore_hp;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D050
bool slf__entity__set_stun_restore_hit_points__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (auto stun = self->damage_ifc()->stun)
        stun->stun_restore_hp = (i32)value;

    return true;
}

// sub_86D0B0
bool slf__entity__get_stun_recover_duration__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)) {
        if (auto stun = self->damage_ifc()->stun)
            result = stun->recover_duration;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D160
bool slf__entity__set_stun_recover_duration__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (auto stun = self->damage_ifc()->stun)
        stun->recover_duration = value;

    return true;
}

// sub_86D1B0
bool slf__entity__get_stun_recover_remaining__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)) {
        if (auto stun = self->damage_ifc()->stun)
            result = stun->recover_remaining;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D260
bool slf__entity__set_stun_recover_remaining__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (auto stun = self->damage_ifc()->stun)
        stun->recover_remaining = value;

    return true;
}

// sub_86D2B0
bool slf__entity__is_stunned__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (self->damage_ifc()->is_stunned() ? 1.0f : 0.0f) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D360
bool slf__entity__is_life_bar_hidden__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? (self->damage_ifc()->is_life_bar_hidden() ? 1.0f : 0.0f) : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D410
bool slf__entity__set_life_bar_hidden__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_life_bar_hidden(value != 0.0f);

    return true;
}

// sub_86D470
bool slf__entity__set_use_distance_based_invulnerability__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (value != 0.0f)
        self->damage_ifc()->ext_flags |= damage_ifc_ext_flag_use_distance_based_invulnerability;
    else
        self->damage_ifc()->ext_flags &= ~damage_ifc_ext_flag_use_distance_based_invulnerability;

    return true;
}

// sub_86D4F0
bool slf__entity__set_webcast_level__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_webcast_level(value);

    return true;
}

// sub_86D540
bool slf__entity__set_webcast_style__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_webcast_style((i32)value);

    return true;
}

// sub_86D590
bool slf__entity__get_manip_thrower__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result = self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj) ? self->manip_obj_ifc()->thrower : arch_base_vhandle();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D610
bool slf__entity__get_manip_throw_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj) ? self->manip_obj_ifc()->throw_velocity : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D6A0
bool slf__entity__set_webcast_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (!(self && self->is_an_entity() && self->has_ifc(entity_ifc_damage)))
        return true;

    if (value != 0.0f)
        self->damage_ifc()->ext_flags |= damage_ifc_ext_flag_webcast_enabled;
    else
        self->damage_ifc()->ext_flags &= ~damage_ifc_ext_flag_webcast_enabled;

    return true;
}

// sub_86D720
bool slf__entity__set_webcast_sync_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_webcast_sync_enabled(value != 0.0f);

    return true;
}

// sub_86D780
bool slf__entity__set_webcast_counter_chance__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self        = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto chance      = stack.local_num(0x04);
    auto is_override = stack.local_num(0x08);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_webcast_counter_chance(chance, is_override > 0.0f);

    return true;
}

// sub_86D7F0
bool slf__entity__set_webcast_counter_scale__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->set_webcast_counter_scale(value);

    return true;
}

// sub_86D840
bool slf__entity__get_entity_force_attack_multiplier__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? self->damage_ifc()->force_attack_multiplier : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D8D0
bool slf__entity__set_entity_force_attack_multiplier__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->force_attack_multiplier = value;

    return true;
}

// sub_86D920
bool slf__entity__get_entity_force_react_multiplier__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_damage) ? self->damage_ifc()->force_react_multiplier : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86D9B0
bool slf__entity__set_entity_force_react_multiplier__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto value = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_damage))
        self->damage_ifc()->force_react_multiplier = value;

    return true;
}

// sub_86DA00
bool slf__entity__get_manip_damage__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj) ? (f32)self->manip_obj_ifc()->damage : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86DA90
bool slf__entity__manip_update_for_grab__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    arch_base_vhandle result;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj)) {
        auto grabber = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

        result = grabber ? self->manip_obj_ifc()->update_for_grab(grabber) : self->my_handle;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86DB30
bool slf__entity__set_throwable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto throwable = stack.local_num(0x04);

    if (!self || !self->is_an_entity())
        return true;

    if (self->is_a_vehicle())
        ((vehicle*)self)->throwable = throwable != 0.0f;

    if (self->has_ifc(entity_ifc_manip_obj))
        self->manip_obj_ifc()->set_flag(manip_obj_flag_throwable, throwable != 0.0f);

    return true;
}

// sub_86DBD0
bool slf__entity__is_throwable__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity()) {
        if (self->is_a_vehicle())
            result = (f32)((vehicle*)self)->throwable;

        if (result == 0.0f && self->has_ifc(entity_ifc_manip_obj))
            result = (f32)((self->manip_obj_ifc()->flags & manip_obj_flag_throwable) != 0);
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86DCB0
bool slf__entity__set_always_upright__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self    = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto upright = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->is_a_vehicle())
        ((vehicle*)self)->always_upright = upright != 0.0f;

    return true;
}

// sub_86DD20
bool slf__entity__is_always_upright__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->is_a_vehicle())
        result = (f32)((vehicle*)self)->always_upright;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86DDD0
bool slf__entity__set_manip_allow_pickup__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto allow = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj))
        self->manip_obj_ifc()->set_flag(manip_obj_flag_allow_pickup, allow != 0.0f);

    return true;
}

// sub_86DE50
bool slf__entity__set_manip_allow_putdown__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto allow = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj))
        self->manip_obj_ifc()->set_flag(manip_obj_flag_allow_putdown, allow != 0.0f);

    return true;
}

// sub_86DED0
bool slf__entity__get_manip_allow_pickup__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj) && self->manip_obj_ifc()->get_allow_pickup() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86DF80
bool slf__entity__get_manip_allow_putdown__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj) && self->manip_obj_ifc()->get_allow_putdown() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86E030
bool slf__entity__set_manip_allow_auto_pickup__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto allow = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj))
        self->manip_obj_ifc()->set_allow_auto_pickup(allow != 0.0f);

    return true;
}

// sub_86E0A0
bool slf__entity__set_manip_allow_auto_putdown__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto allow = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj))
        self->manip_obj_ifc()->set_allow_auto_putdown(allow != 0.0f);

    return true;
}

// sub_86E110
bool slf__entity__get_manip_allow_auto_pickup__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj)) {
        manip_obj_interface* manip = self->manip_obj_ifc();

        if (manip->get_allow_pickup() && manip->get_allow_auto_pickup())
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86E1D0
bool slf__entity__get_manip_allow_auto_putdown__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_manip_obj)) {
        manip_obj_interface* manip = self->manip_obj_ifc();

        if (manip->get_allow_putdown() && manip->get_allow_auto_putdown())
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86E290
bool slf__entity__set_allow_manage_standing__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto allow = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_standing))
        self->standing_ifc()->enable(allow != 0.0f);

    return true;
}

// sub_86E310
bool slf__entity__set_allow_swimming__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto allow = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_standing))
        self->standing_ifc()->set_swimming(allow != 0.0f);

    return true;
}

// sub_86E390
bool slf__entity__set_mass__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto mass = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_mass(mass);

    return true;
}

// sub_86E3E0
bool slf__entity__set_slide_factor__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto factor = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_slide_factor(factor);

    return true;
}

// sub_86E430
bool slf__entity__set_bounce_factor__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto factor = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_bounce_factor(factor);

    return true;
}

// sub_86E480
bool slf__entity__set_bouncy__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_bouncy(on != 0.0f);

    return true;
}

// sub_86E500
bool slf__entity__set_sticky__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_sticky(on != 0.0f);

    return true;
}

// sub_86E580
bool slf__entity__is_physics_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_enabled() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86E600
bool slf__entity__set_physics_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->enable(on != 0.0f);

    return true;
}

// sub_86E670
bool slf__entity__is_physics_suspended__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_suspended() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86E700
bool slf__entity__set_physics_suspended__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->suspend(on != 0.0f);

    return true;
}

// sub_86E770
bool slf__entity__set_gravity_enabled__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_gravity(on != 0.0f);

    return true;
}

// sub_86E7E0
bool slf__entity__get_gravity_multiplier__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_entity() && self->has_ifc(entity_ifc_physical) ? self->physical_ifc()->get_gravity_multiplier() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86E870
bool slf__entity__set_gravity_multiplier__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self       = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto multiplier = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_gravity_multiplier(multiplier);

    return true;
}

// sub_86E8C0
bool slf__entity__set_allow_biped_physics__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_allow_biped_physics(on != 0.0f);

    return true;
}

// sub_86E940
bool slf__entity__set_angular_velocity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto velocity = (const vector3d*)&stack.local_uint(0x04);

    if (self && self->is_an_entity() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_angular_velocity(*velocity);

    return true;
}

// sub_86E9A0
bool slf__entity__set_time_dilation__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto dilation = stack.local_num(0x04);

    if (!self || !self->is_an_entity())
        return true;

    if (!self->has_ifc(entity_ifc_time))
        self->create_time_ifc();

    self->time_ifc()->set_time_dilation(dilation);

    return true;
}

// sub_86EA20
bool slf__entity__get_time_dilation__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result;

    if (self && self->is_an_entity()) {
        if (!self->has_ifc(entity_ifc_time))
            self->create_time_ifc();

        result = self->time_ifc()->get_time_dilation();
    } else
        result = references::g_world_ptr.read()->time_mgr.global_time_dilation;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86EAB0
bool slf__entity__set_time_mode__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto mode = stack.local_num(0x04);

    if (!self || !self->is_an_entity() || (u32)(i64)mode > time_mode_relative)
        return true;

    // relative time needs an interface already
    if (!self->has_ifc(entity_ifc_time)) {
        if ((u32)(i64)mode == time_mode_relative)
            return true;

        self->create_time_ifc();
    }

    self->time_ifc()->set_mode((e_time_mode)(i64)mode);

    return true;
}

// sub_86EB80
bool slf__entity__create_script_data_interface__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_actor() && !self->has_ifc(entity_ifc_script_data))
        self->create_script_data_ifc();

    return true;
}

// sub_86EBC0
bool slf__entity__has_script_data_interface__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->has_ifc(entity_ifc_script_data) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86EC10
bool slf__entity__create_physical_interface__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_actor() && !self->has_ifc(entity_ifc_physical)) {
        self->create_physical_ifc();
        self->flags |= entity_flag_moving;
    }

    return true;
}

// sub_86EC60
bool slf__entity__create_damage_interface__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_actor() && !self->has_ifc(entity_ifc_damage))
        self->create_damage_ifc();

    return true;
}

// sub_86ECA0
bool slf__entity__has_damage_interface__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->has_ifc(entity_ifc_damage) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86ECF0
bool slf__entity__has_player_interface__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->has_ifc(entity_ifc_player) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86ED40
bool slf__entity__physical_ifc_apply_force__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto direction = (const vector3d*)&stack.local_uint(0x04);
    auto magnitude = stack.local_num(0x10);

    if (!self)
        return true;

    math::vector4 direction_4(direction->x, direction->y, direction->z, 0.0f);
    math::vector4 unit;

    retail::sub_401AA0((u64*)&unit, (f32*)&direction_4);

    vector3d force((f32)((f64)unit[0] * magnitude),
                   (f32)((f64)unit[1] * magnitude),
                   (f32)((f64)unit[2] * magnitude));

    if (self->has_ifc(entity_ifc_physical) && !self->is_a_vehicle()) {
        self->physical_ifc()->add_velocity_increment(force);

        return true;
    }

    if (!self->is_a_vehicle()) {
        if (!self->is_an_actor())
            return true;

        if (!(self->ext_flags & entity_ext_flag_spawned) && !(self->ext_flags & entity_ext_flag_is_auto_activate_prop))
            return true;
    }

    // vehicles and spawned props already have one
    rigid_body_prop_head* head = !self->is_a_vehicle() && !(self->ext_flags & entity_ext_flag_spawned) ?
        prop_system::create_prop(self,
                                 rigid_body_prop_flag_auto_remove_stable_prop_pieces |
                                 rigid_body_prop_flag_auto_remove_empty_prop_head    |
                                 rigid_body_prop_flag_do_not_traverse_children,
                                 references::unk_010f7140.get(),
                                 0.0f,
                                 false,
                                 nullptr) :
        prop_system::find_rigid_body_prop_head(self);

    if (!head)
        return true;

    rigid_body* body = head->m_first_rbpi->m_rb;

    if (!body)
        return true;

    body->add_force(vector4d(force.x, force.y, force.z, 0.0f));

    return true;
}

// sub_86EF80
bool slf__entity__physical_ifc_apply_force_at_pos__vector3d__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self, each vector3d once
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 5));

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto direction = (const vector3d*)&stack.local_uint(0x04);
    auto magnitude = stack.local_num(0x10);
    auto position  = (const vector3d*)&stack.local_uint(0x14);

    if (!self)
        return true;

    math::vector4 direction_4(direction->x, direction->y, direction->z, 0.0f);
    math::vector4 unit;

    retail::sub_401AA0((u64*)&unit, (f32*)&direction_4);

    vector3d force((f32)((f64)unit[0] * magnitude),
                   (f32)((f64)unit[1] * magnitude),
                   (f32)((f64)unit[2] * magnitude));

    f32 torque_mult = count - 3 > 0 ? stack.local_num(0x20) : 1.0f;

    if (self->has_ifc(entity_ifc_physical) && !self->is_a_vehicle()) {
        self->physical_ifc()->add_velocity_increment_at_pos(force, *position, torque_mult);

        return true;
    }

    if (!self->is_a_vehicle()) {
        if (!self->is_an_actor())
            return true;

        if (!(self->ext_flags & entity_ext_flag_spawned) && !(self->ext_flags & entity_ext_flag_is_auto_activate_prop))
            return true;
    }

    // vehicles and spawned props already have one
    rigid_body_prop_head* head = !self->is_a_vehicle() && !(self->ext_flags & entity_ext_flag_spawned) ?
        prop_system::create_prop(self,
                                 rigid_body_prop_flag_auto_remove_stable_prop_pieces |
                                 rigid_body_prop_flag_auto_remove_empty_prop_head    |
                                 rigid_body_prop_flag_do_not_traverse_children,
                                 references::unk_010f7140.get(),
                                 0.0f,
                                 false,
                                 nullptr) :
        prop_system::find_rigid_body_prop_head(self);

    if (!head)
        return true;

    rigid_body* body = head->m_first_rbpi->m_rb;

    if (!body)
        return true;

    body->add_force(vector4d(force.x,     force.y,     force.z,     0.0f),
                    vector4d(position->x, position->y, position->z, 0.0f),
                    torque_mult);

    return true;
}

// sub_86F1F0
bool slf__entity__physical_ifc_is_effectively_standing__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->has_ifc(entity_ifc_standing) && self->standing_ifc()->is_effectively_standing() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86F280
bool slf__entity__physical_ifc_manage_standing__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto force = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_standing))
        self->standing_ifc()->manage_standing(0.01f, force != 0.0f, -1.0f);

    return true;
}

// sub_86F2F0
bool slf__entity__physical_ifc_cancel_all_velocity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->cancel_all_velocity(references::unk_010f7140.get(), true);

    return true;
}

// sub_86F330
bool slf__entity__physical_ifc_is_biped_physics_running__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_flagged(physical_interface_flag_biped_running) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86F3D0
bool slf__entity__physical_ifc_is_prop_physics_running__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    // without a physical interface, whether it's a spawned prop
    if (self) {
        if (self->has_ifc(entity_ifc_physical))
            result = self->physical_ifc()->is_prop_physics_running() ? 1.0f : 0.0f;
        else if (self->ext_flags & entity_ext_flag_spawned)
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86F440
bool slf__entity__physical_ifc_is_prop_physics_stable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self        = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto full_stable = stack.local_num(0x04);

    f32 result = self && self->has_ifc(entity_ifc_physical) && self->physical_ifc()->is_prop_physics_stable((f64)full_stable > 0.0001) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86F500
bool slf__entity__physical_ifc_stop_biped_physics__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self &&
        self->has_ifc(entity_ifc_physical) &&
        self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))

        self->physical_ifc()->stop_biped_physics(false);

    return true;
}

// sub_86F550
bool slf__entity__physical_ifc_set_biped_physics_is_weapon__num__t::operator()(chuck::vm::vm_stack &stack) {
    // the count word covers every argument after self
    i32 count = (i32)stack.pop_num();

    stack.move_sp(-4 * (count + 1));

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self ||
        !self->has_ifc(entity_ifc_physical) ||
        !self->physical_ifc()->is_flagged(physical_interface_flag_biped_running))

        return true;

    i32 optional_count = count - 1;

    f32 unk_1 = optional_count > 0 ? stack.local_num(0x08) : 2.5f;
    f32 unk_2 = optional_count > 1 ? stack.local_num(0x0C) : 5.0f;

    self->physical_ifc()->set_biped_physics_is_weapon(stack.local_num(0x04) != 0.0f, unk_1, unk_2);

    return true;
}

// sub_87C980
bool slf__entity__physical_ifc_set_pendulum__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto pivot  = (const vector3d*)&stack.local_uint(0x04);
    auto length = stack.local_num(0x10);

    if (!self || !self->has_ifc(entity_ifc_physical))
        return true;

    pendulum* the_pendulum = self->physical_ifc()->get_static_pendulum(0);

    if (!the_pendulum)
        the_pendulum = new (mm::allocate(sizeof(pendulum))) pendulum();

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

    the_pendulum->enable(true);

    self->physical_ifc()->set_static_pendulum(0, the_pendulum);

    return true;
}

// sub_87CB40
bool slf__entity__physical_ifc_set_pendulum__entity__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto pivot  = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();
    auto length = stack.local_num(0x08);

    if (!self || !pivot || !self->has_ifc(entity_ifc_physical))
        return true;

    pendulum* the_pendulum = self->physical_ifc()->get_static_pendulum(0);

    if (!the_pendulum)
        the_pendulum = new (mm::allocate(sizeof(pendulum))) pendulum();

    the_pendulum->set_entity_base_pivot(pivot);

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

    the_pendulum->enable(true);

    self->physical_ifc()->set_static_pendulum(0, the_pendulum);

    return true;
}

// sub_87CCF0
bool slf__entity__physical_ifc_clear_pendulum__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->has_ifc(entity_ifc_physical))
        return true;

    pendulum* the_pendulum = self->physical_ifc()->get_static_pendulum(0);

    self->physical_ifc()->set_static_pendulum(0, nullptr);

    if (the_pendulum)
        mm::free(the_pendulum);

    return true;
}

// sub_87CD60
bool slf__entity__set_pendulum_attach_limb__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto limb = stack.local_num(0x04);

    if (!self || !self->has_ifc(entity_ifc_physical))
        return true;

    if (pendulum* the_pendulum = self->physical_ifc()->get_static_pendulum(0))
        the_pendulum->set_attach_limb((i32)limb);

    return true;
}

// sub_86F640
bool slf__entity__get_num_of_active_pendulums__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->has_ifc(entity_ifc_physical) ? (f32)self->physical_ifc()->get_num_active_pendulums() : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86F6B0
bool slf__entity__enable_collisions__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_actor())
        self->set_collisions_active(on != 0.0f, true);

    return true;
}

// sub_86F710
bool slf__entity__collisions_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() && (self->flags & entity_flag_collisions_active) ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86F7A0
bool slf__entity__enable_character_collisions__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->is_an_actor())
        self->set_character_collisions_active(on != 0.0f);

    return true;
}

// sub_86F810
bool slf__entity__character_collisions_enabled__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_an_actor() && self->are_character_collisions_active() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86F8B0
bool slf__entity__get_name__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    string_hash result = self ? self->id : string_hash();

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86F900
bool slf__entity__set_special_target__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on == 0.0f)
        self->flags &= ~entity_flag_special_target;
    else
        self->flags |= entity_flag_special_target;

    return true;
}

// sub_86F950
bool slf__entity__set_allow_lockon__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on == 0.0f)
        self->flags &= ~entity_flag_allow_lockon;
    else
        self->flags |= entity_flag_allow_lockon;

    return true;
}

// sub_86F9A0
bool slf__entity__is_valid__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    u32 &handle = stack.local_uint(0x00);

    f32 result = handle && retail::sub_605320(&handle) && arch_base_vhandle(handle).resolve() ? 1.0f : 0.0f; // arch_base_vhandle::is_possibly_valid

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86FA10
bool slf__entity__is_alive__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = self && self->is_alive() ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_86FA60
bool slf__entity__set_immobile__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (self && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_immobile(on != 0.0f);

    return true;
}

// sub_86FAD0
bool slf__entity__set_really_immobile__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self || !self->is_an_actor() || !self->adv_ptrs)
        return true;

    self->adv_ptrs->unk_028 = on > 0.0001f ? 10 : 0;

    return true;
}

// sub_86FB30
bool slf__entity__set_auto_prop_physics_activate__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self || !self->is_an_actor())
        return true;

    // only a standalone mesh-like colgeom can become a prop
    if ((f64)on > 0.0001 &&
        !(self->flags & entity_flag_member) &&
        !self->is_a_conglomerate() &&
        self->get_colgeom() &&
        (self->get_colgeom()->get_type() == collision_geometry_type_mesh || self->get_colgeom()->get_type() == 3))

        self->ext_flags |= entity_ext_flag_is_auto_activate_prop;
    else
        self->ext_flags &= ~entity_ext_flag_is_auto_activate_prop;

    if (self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_auto_prop_physics((self->ext_flags & entity_ext_flag_is_auto_activate_prop) != 0);

    return true;
}

// sub_86FC10
bool slf__entity__set_actor_debug_text__str__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self  = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto text  = (const char*)stack.local_uint(0x04);
    auto time  = stack.local_num(0x08);
    auto color = (const vector3d*)&stack.local_uint(0x0C);

    if (!self || !self->is_an_actor())
        return true;

    u8 red   = color32::adjust(color->x);
    u8 green = color32::adjust(color->y);
    u8 blue  = color32::adjust(color->z);

    u32 packed = (u32)blue << 24 | (u32)green << 16 | (u32)red << 8 | 0xFF;

    ((void (__thiscall*)(arch_base*, mash::string, i32, u32))retail::sub_601CE0)(self, mash::string(text), (i32)time, packed);

    return true;
}

// sub_86FD40
bool slf__entity__physical_ifc_start_prop_physics__vector3d__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto force  = (const vector3d*)&stack.local_uint(0x04);
    auto random = stack.local_num(0x10);

    if (!self || !self->is_an_actor())
        return true;

    // a new interface starts disabled and suspended, with gravity doubled
    if (!self->has_ifc(entity_ifc_physical)) {
        self->create_physical_ifc();
        self->physical_ifc()->enable(false);
        self->physical_ifc()->suspend(true);
        self->physical_ifc()->set_gravity(true);
        self->physical_ifc()->set_gravity_multiplier(2.0f);
    }

    self->physical_ifc()->start_prop_physics(*force, random);

    return true;
}

// sub_86FE10
bool slf__entity__physical_ifc_start_prop_physics__vector3d__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x20;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto force  = (const vector3d*)&stack.local_uint(0x04);
    auto random = stack.local_num(0x10);

    if (!self || !self->is_an_actor())
        return true;

    // a new interface starts disabled and suspended, with gravity doubled
    if (!self->has_ifc(entity_ifc_physical)) {
        self->create_physical_ifc();
        self->physical_ifc()->enable(false);
        self->physical_ifc()->suspend(true);
        self->physical_ifc()->set_gravity(true);
        self->physical_ifc()->set_gravity_multiplier(2.0f);
    }

    bool traverse_children = (f64)stack.local_num(0x1C) > 0.0001;
    bool kill_when_stable  = (f64)stack.local_num(0x18) > 0.0001;
    bool lightweight       = (f64)stack.local_num(0x14) > 0.0001;

    self->physical_ifc()->start_prop_physics(*force, random, lightweight, kill_when_stable, traverse_children);

    return true;
}

// sub_86FF30
bool slf__entity__physical_ifc_stop_prop_physics__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_an_actor() && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->stop_prop_physics(false);

    return true;
}

// sub_86FF80
bool slf__entity__debug_only_disable_weapon_interface__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    return true;
}

// sub_86FF80
bool slf__entity__get_animation_names__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    arch_base_vhandle(stack.local_uint(0x00)).resolve();

    return true;
}

// sub_882E00
bool slf__entity__get_weapon_names__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon);

    if (!weapons)
        return true;

    // every weapon of both hands goes in as an empty string
    for (i32 hand = 0; hand < 2; ++hand) {
        for (mash::vector<ai_weapon> &list : weapons->weapon_sets[hand]->lists) {
            for (ai_weapon** it = list.begin(); it != list.end(); ++it) {
                auto names = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);

                chuck::vm::chuck_dynamic_array_element_t element;

                names->push_back(element);
                *(const char**)names->back().stuff = "";
            }
        }
    }

    return true;
}

// sub_882F40
bool slf__entity__get_marker_names__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (conglomerate*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto names = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);

    if (!self || !self->is_an_actor() || !self->is_a_conglomerate())
        return true;

    i32 count = (i32)self->members.size;

    for (i32 i = 0; i < count; ++i) {
        entity* member = self->members.array[i];

        if (!member || !member->is_a_marker())
            continue;

        // the name is looked up and dropped; every entry is an empty string
        retail::sub_62F970((i32)self, (i16)i); // conglomerate::get_member_id

        chuck::vm::chuck_dynamic_array_element_t element;

        names->push_back(element);
        *(const char**)names->back().stuff = "";
    }

    return true;
}

// sub_86FFE0
bool slf__entity__ai_enable_weapon__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name   = (const char*)stack.local_uint(0x04);
    auto enable = stack.local_num(0x08);

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    if (auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon)) {
        string_hash weapon;
        weapon.initialize(mash::ALLOCATED, name);

        weapons->enable_weapon(weapon, enable != 0.0f);
    }

    return true;
}

// sub_870090
bool slf__entity__ai_weapon_available__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = 0.0f;

    if (self && self->is_an_actor() && self->get_ai_core()) {
        if (auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon)) {
            string_hash weapon;
            weapon.initialize(mash::ALLOCATED, name);

            if (weapons->is_weapon_available(weapon))
                result = 1.0f;
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_870170
bool slf__entity__ai_enable_all_weapons__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto enable = stack.local_num(0x04);

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    if (auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon))
        weapons->enable_all_weapons(enable != 0.0f);

    return true;
}

// sub_870220
bool slf__entity__holster_weapon__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name       = (const char*)stack.local_uint(0x04);
    auto other_name = (const char*)stack.local_uint(0x08);
    auto immediate  = stack.local_num(0x0C);

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    if (auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon)) {
        string_hash other_weapon;
        other_weapon.initialize(mash::ALLOCATED, other_name);

        string_hash weapon;
        weapon.initialize(mash::ALLOCATED, name);

        weapons->holster_weapon(weapon, other_weapon, immediate > 0.5f);
    }

    return true;
}

// sub_8702E0
bool slf__entity__draw_weapon__str__str__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self       = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name       = (const char*)stack.local_uint(0x04);
    auto other_name = (const char*)stack.local_uint(0x08);
    auto immediate  = stack.local_num(0x0C);

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    if (auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon)) {
        string_hash other_weapon;
        other_weapon.initialize(mash::ALLOCATED, other_name);

        string_hash weapon;
        weapon.initialize(mash::ALLOCATED, name);

        weapons->draw_weapon(weapon, other_weapon, immediate > 0.5f);
    }

    return true;
}

// sub_8703A0
bool slf__entity__ai_weapon_sys_busy__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 0.0f;

    if (self && self->is_an_actor() && self->get_ai_core()) {
        auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon);

        if (weapons && weapons->weapon_sys_busy)
            result = 1.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_870460
bool slf__entity__get_ai_weapon_state__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    f32 result = (f32)ai_num_weapon_states;

    if (self && self->is_an_actor() && self->get_ai_core()) {
        if (auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon)) {
            string_hash weapon_name;
            weapon_name.initialize(mash::ALLOCATED, name);

            if (ai_weapon* weapon = weapons->find_weapon(weapon_name))
                result = (f32)weapon->get_state();
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_870550
bool slf__entity__fire_weapon__str__num__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self     = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name     = (const char*)stack.local_uint(0x04);
    auto kind     = (u32)(i32)stack.local_num(0x08);
    auto target   = arch_base_vhandle(stack.local_uint(0x0C));
    auto position = (const vector3d*)&stack.local_uint(0x10);

    if (!self || !self->is_an_actor() || !self->get_ai_core() || kind > weapon_fire_kind_rocket)
        return true;

    auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon);

    if (!weapons)
        return true;

    string_hash weapon_name;
    weapon_name.initialize(mash::ALLOCATED, name);

    ai_weapon* weapon = weapons->find_weapon(weapon_name);

    if (weapon && weapon->has_weapon_entity())
        ai_weapon::fire(self, weapon, (e_weapon_fire_kind)kind, true, target, *position);

    return true;
}

// sub_870610
bool slf__entity__get_weapon_entity__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto name = (const char*)stack.local_uint(0x04);

    arch_base_vhandle result;

    if (self && self->is_an_actor() && self->get_ai_core()) {
        if (auto* weapons = (ai_weapon_inode*)self->get_ai_core()->get_info_node(info_node_type_ai_weapon)) {
            string_hash weapon_name;
            weapon_name.initialize(mash::ALLOCATED, name);

            ai_weapon* weapon = weapons->find_weapon(weapon_name);

            if (weapon && weapon->has_weapon_entity()) {
                if (arch_base* entity = weapon->weapon_entity.resolve())
                    result = entity->my_handle;
            }
        }
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_870700
bool slf__entity__ai_reset_morale__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    if (auto* fear = (std_fear_inode*)self->get_ai_core()->get_info_node(info_node_type_std_fear))
        fear->reset_morale();

    return true;
}

// sub_870770
bool slf__entity__ai_set_ignore_limbo__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto ignore = stack.local_num(0x04);

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    if (ignore != 0.0f)
        self->get_ai_core()->flags |= ai_core_flag_ignore_limbo;
    else
        self->get_ai_core()->flags &= ~ai_core_flag_ignore_limbo;

    return true;
}

// sub_8707F0
bool slf__entity__set_walkable__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    if (on == 0.0f)
        self->flags &= ~entity_flag_walkable;
    else
        self->flags |= entity_flag_walkable;

    return true;
}

// sub_870B80
bool slf__entity__physical_ifc_set_ballistic_target__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target = (const vector3d*)&stack.local_uint(0x04);

    if (self && self->has_ifc(entity_ifc_physical))
        self->physical_ifc()->set_physics_ballistic_target(*target);

    return true;
}

// sub_870BC0
bool slf__entity__manip_obj_ifc_throw_it__entity__vector3d__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self       = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto target_pos = (const vector3d*)&stack.local_uint(0x08);

    if (self && self->has_ifc(entity_ifc_manip_obj)) {
        manip_obj_interface* manip = self->manip_obj_ifc();

        auto target  = (entity*)arch_base_vhandle(stack.local_uint(0x14)).resolve();
        auto thrower = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

        manip->throw_it(thrower, *target_pos, target, false, references::unk_00f22130.get());
    }

    return true;
}

// sub_870C30
bool slf__entity__manip_obj_ifc_throw_it_velocity__entity__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x14;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto velocity = (const vector3d*)&stack.local_uint(0x08);

    if (self && self->has_ifc(entity_ifc_manip_obj)) {
        manip_obj_interface* manip = self->manip_obj_ifc();

        auto thrower = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

        manip->throw_it_velocity(thrower, *velocity, references::unk_010f7140.get(), true);
    }

    return true;
}

// sub_870C90
bool slf__entity__manip_obj_ifc_throw_it_velocity_and_rotation__entity__vector3d__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x20;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto velocity = (const vector3d*)&stack.local_uint(0x08);
    auto rotation = (vector3d*)&stack.local_uint(0x14);

    if (!self || !self->has_ifc(entity_ifc_manip_obj))
        return true;

    // the rotation is in the thrower's frame, whose w column gets reset in place first
    po* frame = ((entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve())->my_abs_po;

    frame->get_matrix().x.w = 0.0f;
    frame->get_matrix().y.w = 0.0f;
    frame->get_matrix().z.w = 0.0f;
    frame->get_matrix().w.w = 1.0f;

    *rotation = frame->non_affine_slow_xform(*rotation);

    manip_obj_interface* manip = self->manip_obj_ifc();

    auto thrower = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

    manip->throw_it_velocity(thrower, *velocity, *rotation, true);

    return true;
}

// sub_870E30
bool slf__entity__manip_obj_ifc_throw_it_with_speed__entity__entity__num__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self     = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto rotation = (const vector3d*)&stack.local_uint(0x10);

    if (self && self->has_ifc(entity_ifc_manip_obj)) {
        manip_obj_interface* manip = self->manip_obj_ifc();

        auto target  = (entity*)arch_base_vhandle(stack.local_uint(0x08)).resolve();
        auto speed   = stack.local_num(0x0C);
        auto thrower = (entity*)arch_base_vhandle(stack.local_uint(0x04)).resolve();

        manip->throw_it_with_speed(thrower, speed, target, *rotation, -1.0f, 0.0f);
    }

    return true;
}

// sub_870EB0
bool slf__entity__manip_obj_ifc_disarm__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->has_ifc(entity_ifc_manip_obj))
        self->manip_obj_ifc()->disarm();

    return true;
}

// sub_870EF0
bool slf__entity__drop_manip_obj__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_an_actor() || !self->has_base_ai_data())
        return true;

    if (self->has_ifc(entity_ifc_player)) {
        self->player_ifc()->unk_282(self->get_ai_core());

        return true;
    }

    auto* carry = (std_carry_inode*)self->get_ai_core()->get_info_node(info_node_type_std_carry);

    if (carry)
        retail::sub_4BD960((u32*)carry); // std_carry_inode: lets go of every manip obj

    return true;
}

// sub_870FB0
bool slf__entity__orientation_floor__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    // the up axis decides; a missing entity counts as all three
    f32 result = !self || self->my_abs_po->get_y_facing().y > 0.7f ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_871040
bool slf__entity__orientation_wall__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = 1.0f;

    if (self) {
        const f32 up = self->my_abs_po->get_y_facing().y;

        if (up > 0.7f || -0.7f > up)
            result = 0.0f;
    }

    stack.push(&result, sizeof(result));

    return true;
}

// sub_8710F0
bool slf__entity__orientation_ceiling__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    f32 result = !self || self->my_abs_po->get_y_facing().y < -0.7f ? 1.0f : 0.0f;

    stack.push(&result, sizeof(result));

    return true;
}

// sub_871190
bool slf__entity__orbit__vector3d__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x1C;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto center = (const vector3d*)&stack.local_uint(0x04);
    auto range  = stack.local_num(0x10);
    auto theta  = stack.local_num(0x14);
    auto psi    = stack.local_num(0x18);

    if (!self)
        return true;

    po rel_po;
    retail::sub_4015B0((u64*)&rel_po);

    vector3d position = marky_camera::get_orbit_pos(*center, range, theta, psi);

    rel_po.get_matrix().w = vector4d(position.x, position.y, position.z, 1.0f);

    vector4d center_4(center->x, center->y, center->z, 0.0f);
    retail::sub_44DE20((f32*)&rel_po, (f32*)&center_4);

    retail::sub_626BE0((i32)self, (u64*)&rel_po); // entity_base::set_rel_po

    update_als_facing(self);

    return true;
}

// sub_871300
bool slf__entity__set_puppet_goto_destination__vector3d__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x10;

    auto self        = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto destination = (const vector3d*)&stack.local_uint(0x04);

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    if (auto* puppet = (std_puppet_inode*)self->get_ai_core()->get_info_node(info_node_type_std_puppet))
        puppet->set_puppet_goto_destination(*destination);

    return true;
}

// sub_87A810
bool slf__entity__set_puppet_goto_destinations__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto points = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);

    mash::vector_basic<vector3d> destinations;
    destinations.array    = nullptr;
    destinations.max_size = 0;

    // the script's points go in back to front
    for (i32 index = (i32)points->size() - 1; index >= 0; --index)
        destinations.push_back(*(const vector3d*)(*points)[index].stuff);

    if (self && self->is_an_actor() && self->get_ai_core()) {
        if (auto* puppet = (std_puppet_inode*)self->get_ai_core()->get_info_node(info_node_type_std_puppet))
            puppet->set_puppet_goto_destinations(&destinations);
    }

    return true;
}

// sub_871390
bool slf__entity__puppet_set_movement_info__dynamic_array__num__num__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x18;

    auto self              = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto points            = (chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04);
    auto goto_radius       = stack.local_num(0x08);
    auto goto_speed        = stack.local_num(0x0C);
    auto force_non_pathed  = stack.local_num(0x10);
    auto disable_avoidance = stack.local_num(0x14);

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    auto* puppet = (std_puppet_inode*)self->get_ai_core()->get_info_node(info_node_type_std_puppet);

    if (!puppet)
        return true;

    chuck::vm::script_dynamic_array<vector3d> destinations(points);

    vector3d fill;

    puppet->goto_destinations.resize(destinations.size(), fill);

    u32 count = destinations.size();

    for (u32 index = 0; index < count; ++index)
        puppet->goto_destinations.array[index] = destinations[index];

    param_block &params = puppet->my_param_block;

    params.set_pb_float(goto_radius_key,            goto_radius);
    params.set_pb_float(goto_speed_key,             goto_speed);
    params.set_pb_int  (goto_allow_facing_key,      0);
    params.set_pb_int  (goto_allow_nonpathed_key,   0);
    params.set_pb_float(turn_speed_deg_key,         -1.0f);
    params.set_pb_int  (goto_force_non_pathed_key,  force_non_pathed > 0.0001f);
    params.set_pb_int  (goto_disable_avoidance_key, disable_avoidance > 0.0001f);
    params.set_pb_int  (movement_info_updated_key,  1);

    return true;
}

// sub_871890
bool slf__entity__puppet_setup_for_goto_complex__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    auto* puppet = (std_puppet_inode*)self->get_ai_core()->get_info_node(info_node_type_std_puppet);

    if (!puppet)
        return true;

    param_block &params = puppet->my_param_block;

    params.set_pb_entity(interact_slave_key,           arch_base_vhandle());
    params.set_pb_entity(interact_master_key,          arch_base_vhandle());
    params.set_pb_int   (ignore_same_state_switch_key, 1);
    params.set_pb_hash  (switch_state_key,             generic_goto_key);

    return true;
}

// sub_871AF0
bool slf__entity__puppet_clear_callbacks__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self)
        return true;

    self->clear_script_callback(puppet_success_stop_key);
    self->clear_script_callback(puppet_failure_stop_key);
    self->clear_script_callback(puppet_dead_stop_key);
    self->clear_script_callback(puppet_damage_stop_key);
    self->clear_script_callback(puppet_threat_stop_key);
    self->clear_script_callback(puppet_stop_key);
    self->clear_script_callback(puppet_external_stop_key);

    return true;
}

// sub_871D90
bool slf__entity__enable_zlock__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto on   = stack.local_num(0x04);

    if (!self)
        return true;

    // any kind of record takes it, unlike entity_tracker::enable_zlock
    if (on > 0.0f) {
        if (auto* record = (u32*)retail::sub_94D720(self->my_handle.value, 5, true)) // tracker record lookup, adding one when missing
            retail::sub_90D110(record, 0.0f);
    } else if (auto* record = (u32*)retail::sub_92BB10(self->my_handle.value)) // tracker record lookup
        retail::sub_90D110(record, -1.0f);

    return true;
}

// sub_871E10
bool slf__entity__set_zlock_ranges__num__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x0C;

    auto self      = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto near_clip = stack.local_num(0x04);
    auto far_clip  = stack.local_num(0x08);

    if (!self)
        return true;

    if (auto* record = (u32*)retail::sub_94D720(self->my_handle.value, 5, true)) // tracker record lookup, adding one when missing
        retail::sub_905FE0(record, near_clip, far_clip); // set_clip_range

    return true;
}

// sub_871E60
bool slf__entity__reset_zlock_ranges__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x04;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self)
        return true;

    if (auto* record = (u32*)retail::sub_94D720(self->my_handle.value, 5, true)) // tracker record lookup, adding one when missing
        retail::sub_905ED0(record, -1.0f);

    return true;
}

// sub_871EA0
bool slf__entity__zlock_ignore_los__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto ignore = stack.local_num(0x04);

    if (!self)
        return true;

    if (auto* record = (u8*)retail::sub_94D720(self->my_handle.value, 5, true)) // tracker record lookup, adding one when missing
        record[0x1C] = ignore != 0.0f;

    return true;
}

// sub_871F00
bool slf__entity__set_camera_priority_boost__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self  = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto boost = stack.local_num(0x04);

    if (!self)
        return true;

    if (auto* record = retail::sub_94D720(self->my_handle.value, 5, true)) // tracker record lookup, adding one when missing
        retail::sub_905EA0(record, boost);

    return true;
}

// sub_871F40
bool slf__entity__set_entity_targeting_weight__num__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self   = (actor*)arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto weight = stack.local_num(0x04);

    if (!self || !self->is_an_actor() || !self->get_ai_core())
        return true;

    if (auto* target = (combat_target_inode*)self->get_ai_core()->get_info_node(info_node_type_combat_target))
        target->set_targeting_weight((i32)weight);

    return true;
}

// sub_871FC0
bool slf__entity__add_fakir_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_fakir())
        retail::sub_632AF0((u32*)self, stack.local_uint(0x04)); // fakir::add_entity

    return true;
}

// sub_872000
bool slf__entity__add_fakir_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_a_fakir())
        return true;

    chuck::vm::script_dynamic_array<u32> entities((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04));

    // the count is read again before every element
    for (i32 i = 0; i < entities.size(); ++i)
        retail::sub_632AF0((u32*)self, entities[i]); // fakir::add_entity

    return true;
}

// sub_8720B0
bool slf__entity__remove_fakir_entity__entity__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (self && self->is_a_fakir())
        retail::sub_61A620((u32*)self, stack.local_uint(0x04)); // fakir::remove_entity

    return true;
}

// sub_8720F0
bool slf__entity__remove_fakir_array__dynamic_array__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x08;

    auto self = (entity*)arch_base_vhandle(stack.local_uint(0x00)).resolve();

    if (!self || !self->is_a_fakir())
        return true;

    chuck::vm::script_dynamic_array<u32> entities((chuck::vm::chuck_dynamic_array_t*)stack.local_uint(0x04));

    for (i32 i = 0; i < entities.size(); ++i)
        retail::sub_61A620((u32*)self, entities[i]); // fakir::remove_entity

    return true;
}

// sub_87CDB0
bool slf__entity__get_best_turret_target__num__num__num__num__vector3d__str__t::operator()(chuck::vm::vm_stack &stack) {
    stack.sp -= 0x24;

    auto self      = arch_base_vhandle(stack.local_uint(0x00)).resolve();
    auto unk_04    = stack.local_num(0x04);
    auto spread    = stack.local_num(0x08);
    auto unk_0c    = stack.local_num(0x0C);
    auto unk_10    = stack.local_num(0x10);
    auto forward   = (const vector3d*)&stack.local_uint(0x14);
    auto team_name = (const char*)stack.local_uint(0x20);

    string_hash team_hash;
    team_hash.initialize(mash::ALLOCATED, team_name);

    ai_team::e_team team = ai_team::get_enum_by_hash(team_hash);

    engine_lock_scope lock(&references::ai_core_list_lock.get());

    using find_best_turret_target_t = ai_core_list_node* (__cdecl*)(arch_base*, ai_core_list*,
                                                                    f32, f32, f32, f32, f32, f32,
                                                                    u32, u32, u32, u32,
                                                                    const vector3d*,
                                                                    u32, u32,
                                                                    ai_team::e_team);

    ai_core_list_node* best = ((find_best_turret_target_t)retail::sub_87BD40)(self,
                                                                             &references::ai_core_list_high.get(),
                                                                             unk_04,
                                                                             spread,
                                                                             -spread,
                                                                             spread,
                                                                             unk_0c,
                                                                             unk_10,
                                                                             0x10011,
                                                                             0,
                                                                             0,
                                                                             0,
                                                                             forward,
                                                                             0,
                                                                             0,
                                                                             team);

    arch_base_vhandle result;

    if (best && best->core)
        result = best->core->get_actor()->my_handle;

    stack.push(&result, sizeof(result));

    return true;
}
