#pragma once

#include "treyarch/chuck/vm/script_function.hh"
#include "treyarch/chuck/vm/script_instance.hh"
#include "treyarch/chuck/vm/vm_simple_list.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class script_executable;
    class vm_thread;

    struct script_object_function_cache_element {
        const script_object* parent;
              string_hash    function_fullname;
              i32            function_index;    // -1 marks a free slot
              i32            usage;
    };

    namespace references {
        // find_func remembers its last 20 hits; the least recently used slot is replaced
        inline util::memory_reference<script_object_function_cache_element> function_cache       { 0x01124A60 };
        inline util::memory_reference<engine_recursive_lock*>               function_cache_lock  { 0x01124A5C };
        inline util::memory_reference<i32>                                  function_cache_usage { 0x01124A58 };
    } // references

    enum e_script_object_flags : u32 {
        script_object_flag_global_object = 0x01,
        script_object_flag_from_mash     = 0x02,
        script_object_flag_needs_run     = 0x10, // set by thread creation and AUTODEST, recomputed by each run (sub_A1D590)
        script_object_flag_sorted_funcs  = 0x08, // find_func binary-searches the functions past the constructor and destructor
        script_object_flag_singleton     = 0x20  // first run creates and runs one "__singleton" instance
    };

    // funcs only holds this level;
    // a serialized function index is flattened across the parent chain and resolved by sub_A1CFE0.
    class script_object {

    public:
        string_hash                      name;
        script_object*                   parent_object;
        script_executable*               parent;
        script_instance*                 global_instance;
        i32                              data_blocksize;
        mash::vector<script_function>    funcs;
        i32                              constructor_index;
        i32                              destructor_index; // negative when absent
        u8                               unk_30[0x08];
        mash::vector_basic
            <vm_reference_descriptor>    reference_descriptors; // only kind 0 is checked, and it's released as a `str` when an instance dies
        vm_simple_list<script_instance*> instances;
        e_script_object_flags            flags;
        ref_counted_simple_mutex*        instance_lock;    // pooled

        script_instance* add_instance(const char* inst_name, e_script_instance_stack_size stack_size);
        script_instance* add_instance(const mash::string                 &inst_name,
                                      const void*                         constructor_parms_buffer,
                                            vm_thread**                   constructor_thread,
                                            e_script_instance_stack_size  stack_size);
        void             construct_instance(script_instance* inst, const void* constructor_parms_buffer, vm_thread** constructor_thread);
        void             remove_instance(script_instance* delete_me, bool run_destructor_if_present);

        // only for a constructor taking exactly one 4-byte argument; null otherwise
        script_instance* create_auto_instance(f32 argument);

        void destruct_instances(bool call_all_destructors);
        void delete_all_instances();

        // parent_object is serialized as an index into the executable's objects, -1 for none
        void post_un_mash_fixup(script_executable* requested_parent);
        void quick_post_un_mash_fixup();

        vm_thread* add_thread(script_instance* inst, i32 fidx);

        bool has_threads() const;
        void run(bool ignore_suspended);

        // a flattened index counts this object's functions first, then its parents'
        i32              find_func(string_hash func_fullname) const;
        script_function* get_function_ptr(u32 index) const;

        script_instance* first_instance() const;

    private:
        void add(script_instance* inst);
    };

    ASSERT_SIZEOF  (script_object,                        0x5C);
    ASSERT_OFFSETOF(script_object, name,                  0x00);
    ASSERT_OFFSETOF(script_object, parent_object,         0x04);
    ASSERT_OFFSETOF(script_object, parent,                0x08);
    ASSERT_OFFSETOF(script_object, global_instance,       0x0C);
    ASSERT_OFFSETOF(script_object, data_blocksize,        0x10);
    ASSERT_OFFSETOF(script_object, funcs,                 0x14);
    ASSERT_OFFSETOF(script_object, constructor_index,     0x28);
    ASSERT_OFFSETOF(script_object, destructor_index,      0x2C);
    ASSERT_OFFSETOF(script_object, reference_descriptors, 0x38);
    ASSERT_OFFSETOF(script_object, instances,             0x48);
    ASSERT_OFFSETOF(script_object, flags,                 0x54);
    ASSERT_OFFSETOF(script_object, instance_lock,         0x58);
}}} // treyarch::chuck::vm
