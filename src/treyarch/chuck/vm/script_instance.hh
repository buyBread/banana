#pragma once

#include "treyarch/shared/mutex.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/chuck/vm/script_var_container.hh"
#include "treyarch/chuck/vm/script_function.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct vm_thread;
    struct script_instance;

    namespace fn {
        inline auto instance_add_thread_with_arguments =
            (vm_thread*(__thiscall*)
            (script_instance*, script_function*, const void*, i32, void*, i32))
            0x00A1E0F0;
    }

    namespace callback {
        // reason 0: instance teardown; reason 1: thread destroy
        using script_instance_lifecycle =
            void(__cdecl*)
            (i32 reason, script_instance* instance, vm_thread* thread, void* user_data);

        // game_variables selects game-variable resource type 10 or shared-variable resource type 11
        using get_script_var_resource =
            script_var_container* (__cdecl*)
            (const string_hash* name, i32* out_mash_data_size, bool game_variables); 
    } // callback

    struct script_instance_callback_node {
        void*                          user_data;
        script_instance_callback_node* next;
    };

    struct script_instance {
        u32                                 flags;
        u32                                 name_hash;
        script_variable_block               script_variables;
        vm_thread*                          first_thread;
        vm_thread*                          last_thread;
        u32                                 thread_count;
        script_object*                      object;
        callback::script_instance_lifecycle lifecycle_callback;
        script_instance_callback_node*      callback_list_head;
        u8                                  unk_30[0x10];
        engine_recursive_lock               thread_lock;
        u8                                  unk_50[0x10];
        script_instance**                   instance_list_owner;
        script_instance*                    previous;
        script_instance*                    next;
        u32                                 unk_6c;

        vm_thread* add_thread(script_function* function,
                              const void*      arguments     = nullptr,
                              i32              argument_size = 0,
                              void*            context       = nullptr,
                              i32              stack_size    = 0) {

            if (!function || argument_size < 0 || (argument_size && !arguments))
                return nullptr;

            u32 maximum_initial_push = 0;

            switch (stack_size) {
                case 0:
                case 128:
                    maximum_initial_push = 284;
                    break;

                case 284:
                    maximum_initial_push = 512;
                    break;

                case 512:
                case 1024:
                    maximum_initial_push = 1024;
                    break;

                default:
                    return nullptr;
            }

            if ((u32)argument_size > maximum_initial_push)
                return nullptr;

            return fn::instance_add_thread_with_arguments(
                this,
                function,
                arguments,
                argument_size,
                context,
                stack_size);
        }
    };

}}} // treyarch::chuck::vm