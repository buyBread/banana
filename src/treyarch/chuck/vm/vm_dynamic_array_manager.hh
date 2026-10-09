#pragma once

#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/mutex.hh"
#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class script_instance;

    // SM3: one VM value (`argument_t`-sized)
    struct chuck_dynamic_array_element_t {
        char stuff[12];
    };

    // a VM `str` handle points at the first element of one of these
    using chuck_dynamic_array_t = dinkumware::vector<chuck_dynamic_array_element_t>;

    // retail addition: SM3's pool held bare array pointers
    struct vm_dynamic_array_slot {
        chuck_dynamic_array_t* array;
        u32                    ref_count;
        script_instance*       owner;     // the array is listed in its garbage-collection stuff
        u32                    unk_0c;    // bit 0 set for string storage
    };

    enum e_vm_dynamic_array_release_mode : i32 {
        vm_dynamic_array_release_strings = 0, // release each element as a `str` first
        vm_dynamic_array_release_erase   = 1, // erase the elements first
        vm_dynamic_array_release_plain   = 2
    };

    /* built by sub_A20E10 during slc_manager::setup.
       one bucket per 16 slots; a set bit marks a free slot and 0xFFFF a bucket with none in use.
       lookups compare every slot of a bucket that has any slot in use, free ones included. */
    class vm_dynamic_array_manager : public singleton_instance<vm_dynamic_array_manager, 0x0112475C> {

    public:
        dinkumware::vector<vm_dynamic_array_slot>* array_pool;
        dinkumware::vector<u16>*                   slot_buckets;
        bool                                       unk_08;    // when set, dying arrays aren't unlisted from their owner
        u8                                         pad_09[3];

        // reserve_size is never read
        chuck_dynamic_array_t* create_new_dynamic_array(script_instance* si, i32 reserve_size);

        // a `str` is the data of a 22-element array; reserve_size is never read
        char* create_string_storage(script_instance* si, i32 reserve_size);
        char* create_new_string(script_instance* si, i32 reserve_size, const char* first, const char* second);
        char* append_string(char* string, const char* source);

        void add_string_reference(const void* string);

        void add_reference(chuck_dynamic_array_t* array);
        bool find_by_data(const void* data, chuck_dynamic_array_t** found);

        void remove_reference(chuck_dynamic_array_t* array);
        void remove_string_reference(const void* string);
        void remove_string_array_reference(chuck_dynamic_array_t* array);
        void remove_instance_array_reference(script_instance* context, chuck_dynamic_array_t* array);

        void release_string_elements(chuck_dynamic_array_t* array);

    private:
        void                   grow_pool();
        i32                    take_free_slot();
        chuck_dynamic_array_t* alloc_array_from_pool(script_instance* si, bool for_string);

        i32  find_slot(const chuck_dynamic_array_t* array);
        void release_reference(chuck_dynamic_array_t*          array,
                               script_instance*                context,
                               e_vm_dynamic_array_release_mode mode);
    };

    namespace references {
        inline util::memory_reference<engine_recursive_lock*> dynamic_array_lock { 0x01124758 };
    } // references

    ASSERT_SIZEOF  (chuck_dynamic_array_element_t, 0x0C);
    ASSERT_SIZEOF  (chuck_dynamic_array_t,         0x10);

    ASSERT_SIZEOF  (vm_dynamic_array_slot,            0x10);
    ASSERT_OFFSETOF(vm_dynamic_array_slot, ref_count, 0x04);
    ASSERT_OFFSETOF(vm_dynamic_array_slot, owner,     0x08);

    ASSERT_SIZEOF  (vm_dynamic_array_manager,               0x0C);
    ASSERT_OFFSETOF(vm_dynamic_array_manager, slot_buckets, 0x04);
    ASSERT_OFFSETOF(vm_dynamic_array_manager, unk_08,       0x08);
}}} // treyarch::chuck::vm
