#pragma once

#include "treyarch/shared/mutex.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/chuck/vm/script_executable.hh"
#include "treyarch/chuck/vm/script_executable_tree.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    struct script_manager;

    namespace fn {
        inline auto runtime_find_thread =
            (vm_thread*(__thiscall*)
            (script_manager*, u32))
            0x00A1B030;
    }

    namespace callback {
        // game_variables selects game-variable resource type 10 or shared-variable resource type 11
        using get_script_var_resource =
            script_var_container* (__cdecl*)
            (const string_hash* name, i32* out_mash_data_size, bool game_variables); 
    } // callback

    struct script_manager {
        u32                               unk_00;                           // something about "chuck old fashioned"; dead in retail
        f32                               latest_tick_argument;
        script_executable_tree*           loaded_executables;
        script_executable*                master_script;
        script_var_container*             game_variable_container;
        script_var_container*             shared_variable_container;
        void*                             executables_pending_first_run;
        void*                             fixed_callbacks[15];
        void*                             lifecycle_callback;
        void*                             resource_acquisition_callback;
        callback::get_script_var_resource script_var_resource_callback;
        void*                             unk_services_64[5];               // cross-platform debugging callbacks; dead in retail
        engine_recursive_lock             loaded_executables_lock;
        engine_recursive_lock             callback_lock;

        vm_thread* find_thread(u32 id) {
            return fn::runtime_find_thread(this, id);
        }
    };
}}} // treyarch::chuck