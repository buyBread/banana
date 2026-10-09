#include <cstring>
#include <new>

#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_types.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/game/arch_base.hh"
#include "treyarch/game/event/chuck_parameter_event.hh"
#include "treyarch/game/event/event_manager.hh"
#include "treyarch/game/game.hh"
#include "treyarch/game/mission/mission_manager.hh"
#include "treyarch/game/script/chuck_callbacks.hh"
#include "treyarch/game/script/script_access.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace references {
    util::memory_reference<u32> client_library_key { 0x010F77F4 };
}} // treyarch::references

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_825E00
void chuck_callbacks::script_manager_notification_callback(e_script_manager_callback_reason reason,
                                                           script_executable*               se,
                                                           void*                            user_data) {

    switch (reason) {
        case script_manager_callback_reason_about_to_unload: {
            arch_base* controller = references::script_controller.read();

            if (controller)
                controller->clear_script_callbacks(se);

            break;
        }

        // the context scope opened here is closed by just_ran, both living in the reason slot
        case script_manager_callback_reason_about_to_run:
            amalga::resource_manager::references::unk_010300a8.get().acquire();
            new (&reason) amalga::resource_context_stack_object((amalga::resource_pack_slot*)user_data);
            break;

        case script_manager_callback_reason_just_ran:
            ((amalga::resource_context_stack_object*)&reason)->~resource_context_stack_object();
            amalga::resource_manager::references::unk_010300a8.get().release();
            break;

        default:
            break;
    }
}

// sub_8258F0
script_executable* chuck_callbacks::get_script_executable_resource_callback(const string_hash* filename,
                                                                                  i32*         resource_size) {

    resource_key key;
    key.set(*filename, amalga::script);

    return (script_executable*)retail::sub_7629D0((u32*)&key, (u32*)resource_size, nullptr);
}

// sub_825930
script_var_container* chuck_callbacks::get_script_var_container_resource_callback(const string_hash* filename,
                                                                                        i32*         resource_size,
                                                                                        bool         is_game_var_container) {

    resource_key key;
    key.set(*filename, is_game_var_container ? amalga::script_gv : amalga::script_sv);

    return (script_var_container*)retail::sub_7629D0((u32*)&key, (u32*)resource_size, nullptr);
}

// sub_71F870
bool chuck_callbacks::unk_predicate_callback() {
    return false;
}

// sub_ABD4A0
e_platform chuck_callbacks::get_platform_callback() {
    return platform_pc;
}

// sub_825B70
void chuck_callbacks::resolve_signal_callback(const char* signal_name, u32* signal_id) {
    *signal_id = event_manager::register_script_event_type(signal_name).source_hash_code;
}

// sub_825B90
u32 chuck_callbacks::resolve_extern_callback(const char* script_object_name, const char* instance_name) {
    return (u32)retail::sub_AD2860(references::mission_manager.read(), (i32)script_object_name, (i32)instance_name);
}

// sub_825BB0
u32 chuck_callbacks::get_chuck_client_library_key_callback() {
    u32 key = references::client_library_key.read();

    if (key)
        return key;

    constexpr u32 name_hashes[] = { 0xC11CDA95, 0xBF6A7C9A, 0x6155AECA, 0x38F6ACC8, 0x870296FD,
                                    0x9D8C9688, 0xC114BC8B, 0x8ADC7DE0, 0x246E450D, 0x5A65C921,
                                    0xDEE65F23, 0x019B84A6, 0x689AE69E, 0x80B6467D, 0xFE620C03,
                                    0xA9328838, 0x9F3AB014, 0xC556BFF1, 0xE7F03FE9, 0xFD685609,
                                    0x4D153CEB, 0x31B685F1, 0x548BD884, 0x64F68DFB, 0x7C0DC25A,
                                    0xC4917CD0, 0x8CCC7315, 0x62338546, 0x544D6C08, 0x6424FC3D,
                                    0xBFCB5A14, 0x53F89A33, 0x8032B5DF, 0xB77652B7, 0x59A51543 };

    key = 0xFFFFFFFF;

    for (u32 name_hash : name_hashes)
        retail::sub_A6D790(&key, name_hash);

    key |= 1;
    references::client_library_key.write(key);

    return key;
}

// sub_825DC0
mash::string chuck_callbacks::get_script_executable_folder_callback() {
    return mash::string("");
}

// sub_5B4A70
void chuck_callbacks::script_instance_created_callback(script_instance*) {}

// sub_825DE0
void chuck_callbacks::script_instance_destroyed_callback(script_instance* inst) {
    script_instance_shadow* inst_shadow = script::get_script_instance_shadow(inst);

    // slot 0, deleting destructor
    if (inst_shadow)
        ((void (__thiscall*)(script_instance_shadow*, u32))inst_shadow->vtable[0])(inst_shadow, 1);
}

// sub_843140
void chuck_callbacks::vm_thread_raise_global_signal_callback(vm_thread*,
                                                             string_hash signal,
                                                             void*       raise_args,
                                                             u32         args_stack_size) {

    if (!args_stack_size) {
        event_manager::raise_event(signal, arch_base_vhandle());

        return;
    }

    chuck_parameter_event* raised_event = new chuck_parameter_event(signal);
    std::memcpy(raised_event->parameters, raise_args, args_stack_size);
    raised_event->args_stack_size = (i32)args_stack_size;

    event_manager::raise_event(raised_event, arch_base_vhandle());
    delete raised_event;
}

// sub_843220
void chuck_callbacks::vm_thread_raise_instance_signal_callback(vm_thread*,
                                                               string_hash      signal,
                                                               script_instance* inst,
                                                               void*            raise_args,
                                                               u32              args_stack_size) {

    chuck_parameter_event* raised_event = nullptr;

    if (args_stack_size) {
        raised_event = new chuck_parameter_event(signal);
        std::memcpy(raised_event->parameters, raise_args, args_stack_size);
        raised_event->args_stack_size = (i32)args_stack_size;
    }

    // a null instance leaves a parameter event allocated
    if (!inst)
        return;

    script::update_script_instance_shadow(inst);
    script_instance_shadow* inst_shadow = script::get_script_instance_shadow(inst);

    if (raised_event) {
        inst_shadow->raise_event(raised_event);
        delete raised_event;
    } else
        inst_shadow->raise_event(signal);
}

// sub_8259B0
void chuck_callbacks::vm_thread_raise_library_signal_callback(vm_thread*,
                                                              string_hash       signal,
                                                              arch_base_vhandle signaller,
                                                              void*,
                                                              u32) {

    arch_base* recipient = signaller.resolve();

    if (recipient)
        recipient->raise_event(signal);
}

// sub_8259E0
void chuck_callbacks::vm_thread_clear_global_callback_by_name_callback(vm_thread*, string_hash name) {
    event_manager::clear_script_callback(arch_base_vhandle(), name);
}

// sub_825A10
void chuck_callbacks::vm_thread_clear_global_callback_by_id_callback(vm_thread*, u32 id) {
    event_manager::clear_script_callback(arch_base_vhandle(), id);
}

// sub_825A30
void chuck_callbacks::vm_thread_clear_instance_callback_by_name_callback(vm_thread*,
                                                                         string_hash      name,
                                                                         script_instance* inst) {

    if (!inst)
        return;

    script::update_script_instance_shadow(inst);
    script::get_script_instance_shadow(inst)->clear_script_callback(name);
}

// sub_825A60
void chuck_callbacks::vm_thread_clear_instance_callback_by_id_callback(vm_thread*,
                                                                       u32              id,
                                                                       script_instance* inst) {

    if (!inst)
        return;

    script::update_script_instance_shadow(inst);
    script::get_script_instance_shadow(inst)->clear_script_callback(id);
}

// sub_825A90
u32 chuck_callbacks::vm_thread_add_global_callback_callback(      vm_thread*,
                                                                  string_hash      signal,
                                                                  script_instance* inst,
                                                                  script_function* sfr,
                                                            const void*            parms,
                                                                  bool             one_shot) {

    return event_manager::add_callback(signal, arch_base_vhandle(), inst, sfr, parms, one_shot);
}

// sub_825AD0
u32 chuck_callbacks::vm_thread_add_instance_callback_callback(      vm_thread*,
                                                                    string_hash      signal,
                                                                    script_instance* me,
                                                                    script_instance* inst,
                                                                    script_function* sfr,
                                                              const void*            parms,
                                                                    bool             one_shot) {

    if (!me)
        return 0;

    script::update_script_instance_shadow(me);

    return script::get_script_instance_shadow(me)->add_callback(signal, inst, sfr, parms, one_shot);
}

// sub_825B20
u32 chuck_callbacks::vm_thread_add_library_callback_callback(      vm_thread*,
                                                                   string_hash       signal,
                                                                   arch_base_vhandle signaller,
                                                                   script_instance*  inst,
                                                                   script_function*  sfr,
                                                             const void*             parms,
                                                                   bool              one_shot) {

    arch_base* recipient = signaller.resolve();

    return recipient ? recipient->add_callback(signal, inst, sfr, parms, one_shot) : 0;
}

// sub_843300
void chuck_callbacks::install() {
    script_manager::get().register_callbacks(script_manager_notification_callback,
                                             get_script_executable_resource_callback,
                                             get_script_var_container_resource_callback,
                                             unk_predicate_callback,
                                             unk_predicate_callback,
                                             unk_predicate_callback,
                                             unk_predicate_callback,
                                             get_platform_callback);

    script_executable::register_callbacks(resolve_signal_callback,
                                          resolve_extern_callback,
                                          get_chuck_client_library_key_callback,
                                          get_script_executable_folder_callback);

    script_instance::set_script_instance_callbacks(script_instance_created_callback,
                                                   script_instance_destroyed_callback);

    vm_thread::register_callbacks(vm_thread_raise_global_signal_callback,
                                  vm_thread_raise_instance_signal_callback,
                                  vm_thread_raise_library_signal_callback,
                                  vm_thread_clear_global_callback_by_name_callback,
                                  vm_thread_clear_global_callback_by_id_callback,
                                  vm_thread_clear_instance_callback_by_name_callback,
                                  vm_thread_clear_instance_callback_by_id_callback,
                                  vm_thread_add_global_callback_callback,
                                  vm_thread_add_instance_callback_callback,
                                  vm_thread_add_library_callback_callback);
}
