#pragma once

#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "treyarch/game/script/script_instance_shadow.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace references {
        // the master script's global object and instance; set when a level's world starts (sub_824EF0), cleared by sub_824F10
        inline util::memory_reference<chuck::vm::script_object*>   master_global_object   { 0x010F7080 };
        inline util::memory_reference<chuck::vm::script_instance*> master_global_instance { 0x010F7084 };
    } // references

    class script {

    public:
        static void update_script_instance_shadow(chuck::vm::script_instance* inst);

        // inlined @ sub_825A30
        static script_instance_shadow* get_script_instance_shadow(chuck::vm::script_instance* inst) {
            return (script_instance_shadow*)inst->client_space;
        }

        // flattened function indices; -1 when missing
        static i32 find_function         (string_hash fullname, chuck::vm::script_object*   object);
        static i32 find_instance_function(string_hash fullname, chuck::vm::script_instance* inst);

        // a thread on inst, ready to be run; the instance goes on its stack first unless it's the master global instance
        static chuck::vm::vm_thread* create_thread(i32         function_index, chuck::vm::script_instance* inst, u32 unk_60);
        static chuck::vm::vm_thread* create_thread(string_hash fullname,       chuck::vm::script_instance* inst, u32 unk_60);

        // false only for a null thread
        static bool start_thread(chuck::vm::vm_thread* thread, bool run_now);
        static bool run_thread  (chuck::vm::vm_thread* thread);

        // sub_825100
        // sub_825200
        // sub_825280
        // sub_825180
        template<typename T>
        static bool run_thread_with_return(chuck::vm::vm_thread* thread, T* result) {
            if (!thread)
                return false;

            string_hash key_prefix;
            key_prefix.initialize(mash::ALLOCATED);

            thread->inst->run_single_thread_with_return(thread, false, result, sizeof(T), key_prefix);

            return true;
        }
    };
} // treyarch
