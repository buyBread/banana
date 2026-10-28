#pragma once

#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "game/script/script_instance_shadow.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/string.hh"
#include "game/resource_key.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class entity;

    // an argument for script::call_function; a zero type ends the list
    struct script_arg {
        u32         type = 0; // 1 is a num
        const void* value;
        u32         unk_008;
    };

    namespace references {
        // the master script's global object and instance; set when a level's world starts (sub_824EF0), cleared by sub_824F10
        inline util::memory_reference<chuck::vm::script_object*>   master_global_object   { 0x010F7080 };
        inline util::memory_reference<chuck::vm::script_instance*> master_global_instance { 0x010F7084 };

        // bytes pushed per script_arg type, nine of them
        inline util::memory_reference<u32> script_arg_sizes { 0x00BDF340 };
    } // references

    class script {

    public:
        static void update_script_instance_shadow(chuck::vm::script_instance* inst);

        // the handle is the instance shadow's; no instance once it's gone
        static chuck::vm::script_instance* get_instance       (arch_base_vhandle           handle);
        static arch_base_vhandle           get_instance_handle(chuck::vm::script_instance* inst);

        // the exec is a script resource in the owner's pack slot
        static bool load_script_exec  (const char*                filename, resource_key* key, actor* owner);
        static bool unload_script_exec(const char*                filename,                    actor* owner);
        static bool unload_script_exec(chuck::vm::script_object*  object,                      actor* owner);

        // nothing for a null object
        static chuck::vm::script_object*   find_object  (string_hash obj_name,  const mash::string        &filename, const resource_key* key);
        static chuck::vm::script_instance* find_instance(string_hash inst_name,       chuck::vm::script_object* object);

        static void reconstruct_instance(chuck::vm::script_instance* inst, chuck::vm::vm_thread** constructor_thread);
        static void destroy_instance    (chuck::vm::script_instance* inst, bool                   auto_destruct);

        // inlined @ sub_825A30
        static script_instance_shadow* get_script_instance_shadow(chuck::vm::script_instance* inst) {
            return (script_instance_shadow*)inst->client_space;
        }

        // flattened function indices; -1 when missing
        static i32 find_function         (string_hash fullname, chuck::vm::script_object*   object);
        static i32 find_instance_function(string_hash fullname, chuck::vm::script_instance* inst);

        // a thread on inst, ready to be run; the instance goes on its stack first unless it's the master global instance
        static chuck::vm::vm_thread* create_thread(i32         function_index, chuck::vm::script_instance* inst, u32 resource_context);
        static chuck::vm::vm_thread* create_thread(string_hash fullname,       chuck::vm::script_instance* inst, u32 resource_context);
        static chuck::vm::vm_thread* create_thread(i32         function_index, chuck::vm::script_instance* inst, actor* owner);

        // looks on the owner's "__enx" instance, then its executable's global instance, then the master global instance
        static chuck::vm::vm_thread* create_thread(actor* owner, string_hash fullname);

        // creates the thread and pushes the arguments; it runs on the next scheduler pass
        static bool call_function(      actor*       owner,
                                        string_hash  fullname,
                                  const script_arg  &arg0,
                                  const script_arg  &arg1,
                                  const script_arg  &arg2,
                                  const script_arg  &arg3,
                                  const script_arg  &arg4,
                                  const script_arg  &arg5,
                                  const script_arg  &arg6,
                                  const script_arg  &arg7);

        // pushes the entity's handle or 0; false only for a null thread
        static bool push_entity(chuck::vm::vm_thread* thread, entity* ent);

        // false only for a null thread
        static bool push_uint(chuck::vm::vm_thread* thread, u32 value);

        // sub_842D00
        template<typename T>
        static bool push_value(chuck::vm::vm_thread* thread, T value) {
            if (thread)
                thread->dstack.push(&value, sizeof(value));

            return thread != nullptr;
        }

        // nothing for a null object or one without an executable
        static chuck::vm::script_instance* add_instance(const mash::string                            &name,
                                                              chuck::vm::script_object*                object,
                                                              chuck::vm::vm_thread**                   constructor_thread,
                                                              u32                                      resource_context,
                                                              chuck::vm::e_script_instance_stack_size  stack_size);

        static chuck::vm::script_instance* add_instance(const mash::string                            &name,
                                                              chuck::vm::script_object*                object,
                                                              chuck::vm::vm_thread**                   constructor_thread,
                                                              actor*                                   owner,
                                                              chuck::vm::e_script_instance_stack_size  stack_size);

        // false only for a null thread
        static bool start_thread(chuck::vm::vm_thread* thread, bool run_now);
        static bool run_thread  (chuck::vm::vm_thread* thread);

        // false only for a null thread
        static bool run_thread_until_finished(chuck::vm::vm_thread* thread);

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
