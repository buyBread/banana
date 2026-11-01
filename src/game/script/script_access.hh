#pragma once

#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/script_object.hh"
#include "treyarch/chuck/vm/vm_thread.hh"
#include "game/script/script_instance_shadow.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/string.hh"
#include "game/resource_key.hh"
#include "game/mutex.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/container/fixed_vector.hh"
#include "treyarch/mm/mm.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class entity;

    // an argument for script::call_function; a zero type ends the list
    struct script_arg {
        u32         type = 0; // 1 is a num, 3 an entity
        const void* value;
        u32         unk_008;
    };

    namespace references {
        // the master script's global object and instance; set when a level's world starts (sub_824EF0), cleared by sub_824F10
        inline util::memory_reference<chuck::vm::script_object*>   master_global_object   { 0x010F7080 };
        inline util::memory_reference<chuck::vm::script_instance*> master_global_instance { 0x010F7084 };

        // bytes pushed per script_arg type, nine of them
        inline util::memory_reference<u32> script_arg_sizes { 0x00BDF340 };

        // numbers the instances script_object_instance::init names without an owner
        inline util::memory_reference<u32> ownerless_instance_count { 0x010F7088 };
    } // references

    namespace amalga {
        struct resource_pack_slot;
    } // amalga

    // the running script's pack as the resource context around a native's loading; a waiting thread pushes nothing but still pops
    class native_resource_context {

        amalga::resource_pack_slot* m_context;

    public:
        explicit native_resource_context(const chuck::vm::vm_stack &stack);
       ~native_resource_context();

        native_resource_context           (const native_resource_context&) = delete;
        native_resource_context &operator=(const native_resource_context&) = delete;
    };

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

        static bool load_script_exec  (const char*                filename, resource_key* key, amalga::resource_pack_slot* slot);
        static bool unload_script_exec(const char*                filename,                    amalga::resource_pack_slot* slot);
        static bool unload_script_exec(chuck::vm::script_object*  object,                      amalga::resource_pack_slot* slot);

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

        // sub_842D30
        static bool push_value(chuck::vm::vm_thread* thread, const vector3d &value) {
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

    // a function of an instance's script object, known by its declaration ("frame_advance(num)")
    struct script_object_function {
        string_hash function_name;  // the whole declaration hashed
        i32         function_index; // -1 when the object doesn't have it
        u32         argument_types; // 4 bits per argument

        // false when the object doesn't have the function
        bool set(const mash::string &func_declaration, chuck::vm::script_object* so, u32 unk);

        static void parse_arguments(const mash::string &func_declaration,
                                          u32*          argument_types,
                                          u32           unk);

        // one argument per 4 bits of argument_types, each pushed by its own type's size
        static void push_arguments(      chuck::vm::vm_thread*                   thread,
                                         u32                                     argument_types,
                                   const container::fixed_vector<script_arg, 8> &args);
    };

    // an instance of a script object driven from code: its executable, the instance and the functions registered on it
    class script_object_instance {

    public:
        engine_recursive_lock               lock;
        i32                                 cached_function_index;
        amalga::resource_pack_slot*         pack_slot;
        resource_key                        script_exec_prefix;
        arch_base_vhandle                   inst_handle;           // the instance's shadow
        vc8::vector<script_object_function> functions;             // sorted by function_name
        u8                                  pad_034[0x04];

        script_object_instance();
       ~script_object_instance();

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        // lets the instance destruct itself and unloads the executable
        void destroy();

        // the executable loads into the slot, else the owner's; the constructor's arguments follow script_obj's declaration
        bool init(const mash::string                           &script_exec,
                  const mash::string                           &script_obj,
                        amalga::resource_pack_slot*             slot,
                        actor*                                  owner,
                  const char*                                   inst_name,
                        bool                                    reuse,
                  const container::fixed_vector<script_arg, 8> &args);

        // the arguments up to the first one with a zero type
        bool init(const mash::string                &script_exec,
                  const mash::string                &script_obj,
                        amalga::resource_pack_slot*  slot,
                        actor*                       owner,
                  const char*                        inst_name,
                        bool                         reuse,
                  const script_arg                  &arg0,
                  const script_arg                  &arg1,
                  const script_arg                  &arg2,
                  const script_arg                  &arg3,
                  const script_arg                  &arg4,
                  const script_arg                  &arg5,
                  const script_arg                  &arg6,
                  const script_arg                  &arg7);

        bool register_function(const mash::string &function_name, u32 unk);
    };

    ASSERT_SIZEOF  (script_object_function,                0x0C);
    ASSERT_SIZEOF  (script_object_instance,                0x38);
    ASSERT_OFFSETOF(script_object_instance, pack_slot,     0x14);
    ASSERT_OFFSETOF(script_object_instance, inst_handle,   0x20);
    ASSERT_OFFSETOF(script_object_instance, functions,     0x24);
} // treyarch
