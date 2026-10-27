#include "game/arch_base.hh"
#include "game/event/event_manager.hh"
#include "game/wds/entity/entity.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace references {
    util::memory_reference<string_hash> collide_entity { 0x0102C788 };
}} // treyarch::references

using namespace treyarch;

// sub_601750
void arch_base::raise_event(string_hash event_type_id) {
    event_manager::raise_event(event_type_id, my_handle);
}

// sub_601770
void arch_base::raise_event(event* raised_event) {
    event_manager::raise_event(raised_event, my_handle);
}

// sub_601790
u32 arch_base::add_callback(string_hash event_type_id, code_event_callback_function function, void* parameters, bool one_shot) {
    if (event_type_id == references::collide_entity.read() && is_an_entity_base())
        ((entity*)this)->ext_flags |= 0x10000000;

    return event_manager::add_callback(event_type_id, my_handle, function, parameters, one_shot);
}

// sub_601850
void arch_base::remove_callback(u32 callback_id, string_hash event_type_id) {
    event_manager::remove_callback(callback_id, event_type_id, my_handle);
}

// sub_6017E0
u32 arch_base::add_callback(      string_hash                 event_type_id,
                                  chuck::vm::script_instance* instance,
                                  chuck::vm::script_function* function,
                            const void*                       parameters,
                                  bool                        one_shot) {

    if (event_type_id == references::collide_entity.read() && is_an_entity_base())
        ((entity*)this)->ext_flags |= 0x10000000;

    return event_manager::add_callback(event_type_id, my_handle, instance, function, parameters, one_shot);
}

// sub_601880
void arch_base::clear_script_callbacks(chuck::vm::script_executable* script_exe) {
    event_manager::clear_script_callbacks(my_handle, script_exe);
}

// sub_6018A0
void arch_base::clear_script_callback(string_hash function_name) {
    event_manager::clear_script_callback(my_handle, function_name);
}

// sub_6018C0
void arch_base::clear_script_callback(u32 id) {
    event_manager::clear_script_callback(my_handle, id);
}
