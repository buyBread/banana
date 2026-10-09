#pragma once

#include "treyarch/chuck/vm/script_manager.hh"
#include "treyarch/chuck/vm/vm_thread.hh"

namespace treyarch { namespace chuck_callbacks {
    void install();

    void script_manager_notification_callback(chuck::vm::e_script_manager_callback_reason reason,
                                              chuck::vm::script_executable*               se,
                                              void*                                       user_data);
    chuck::vm::script_executable* get_script_executable_resource_callback(const string_hash* filename, i32* resource_size);
    chuck::vm::script_var_container* get_script_var_container_resource_callback(const string_hash* filename,
                                                                                      i32*         resource_size,
                                                                                      bool         is_game_var_container);
    bool unk_predicate_callback();
    e_platform get_platform_callback();

    void resolve_signal_callback(const char* signal_name, u32* signal_id);
    u32 resolve_extern_callback(const char* script_object_name, const char* instance_name);
    u32 get_chuck_client_library_key_callback();
    mash::string get_script_executable_folder_callback();

    void script_instance_created_callback(chuck::vm::script_instance* inst);
    void script_instance_destroyed_callback(chuck::vm::script_instance* inst);

    void vm_thread_raise_global_signal_callback(chuck::vm::vm_thread* source_thread,
                                                string_hash           signal,
                                                void*                 raise_args,
                                                u32                   args_stack_size);
    void vm_thread_raise_instance_signal_callback(chuck::vm::vm_thread*       source_thread,
                                                  string_hash                 signal,
                                                  chuck::vm::script_instance* inst,
                                                  void*                       raise_args,
                                                  u32                         args_stack_size);
    void vm_thread_raise_library_signal_callback(chuck::vm::vm_thread* source_thread,
                                                 string_hash           signal,
                                                 arch_base_vhandle     signaller,
                                                 void*                 raise_args,
                                                 u32                   args_stack_size);

    void vm_thread_clear_global_callback_by_name_callback(chuck::vm::vm_thread* source_thread, string_hash name);
    void vm_thread_clear_global_callback_by_id_callback(chuck::vm::vm_thread* source_thread, u32 id);
    void vm_thread_clear_instance_callback_by_name_callback(chuck::vm::vm_thread*       source_thread,
                                                            string_hash                 name,
                                                            chuck::vm::script_instance* inst);
    void vm_thread_clear_instance_callback_by_id_callback(chuck::vm::vm_thread*       source_thread,
                                                          u32                         id,
                                                          chuck::vm::script_instance* inst);

    u32 vm_thread_add_global_callback_callback(      chuck::vm::vm_thread*       source_thread,
                                                     string_hash                 signal,
                                                     chuck::vm::script_instance* inst,
                                                     chuck::vm::script_function* sfr,
                                               const void*                       parms,
                                                     bool                        one_shot);
    u32 vm_thread_add_instance_callback_callback(      chuck::vm::vm_thread*       source_thread,
                                                       string_hash                 signal,
                                                       chuck::vm::script_instance* me,
                                                       chuck::vm::script_instance* inst,
                                                       chuck::vm::script_function* sfr,
                                                 const void*                       parms,
                                                       bool                        one_shot);
    u32 vm_thread_add_library_callback_callback(      chuck::vm::vm_thread*       source_thread,
                                                      string_hash                 signal,
                                                      arch_base_vhandle           signaller,
                                                      chuck::vm::script_instance* inst,
                                                      chuck::vm::script_function* sfr,
                                                const void*                       parms,
                                                      bool                        one_shot);
}} // treyarch::chuck_callbacks
