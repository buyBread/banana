#pragma once

#include "vc8/vector.hh"
#include "treyarch/chuck/vm/vm_simple_list.hh"
#include "game/mutex.hh"
#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace chuck { namespace vm {
    class  script_instance;
    struct garbage_collection_element;

    // SM3: one VM value (`argument_t`-sized)
    struct chuck_dynamic_array_element_t {
        char stuff[12];
    };

    // a VM `str` handle points at the first element of one of these
    using chuck_dynamic_array_t = vc8::vector<chuck_dynamic_array_element_t>;

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

    /* built by setup during slc_manager::init, torn down with the slc_manager.
       one bucket per 16 slots; a set bit marks a free slot and 0xFFFF a bucket with none in use.
       lookups compare every slot of a bucket that has any slot in use, free ones included. */
    class vm_dynamic_array_manager : public singleton_instance<vm_dynamic_array_manager, 0x0112475C> {

    public:
        vc8::vector<vm_dynamic_array_slot>* array_pool;
        vc8::vector<u16>*                   slot_buckets;
        bool                                unk_08;    // when set, dying arrays aren't unlisted from their owner
        u8                                  pad_09[3];

        // also creates dynamic_array_lock
        vm_dynamic_array_manager();
       ~vm_dynamic_array_manager();

        static void setup();
        static void destroy();

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

        // the manager's default for script_garbage_collection_dynamic_array
        static void garbage_collect(script_instance*                             si,
                                    vm_simple_list<garbage_collection_element*> &stuff_to_delete);

    private:
        // 512 slots of arrays reserved for 10 elements, 32 buckets
        void setup_array_pool();
        void destroy_array_pool();

        void                   expand_array_pool();
        i32                    take_free_slot();
        chuck_dynamic_array_t* alloc_array_from_pool(script_instance* si, bool for_string);

        i32  find_slot(const chuck_dynamic_array_t* array);
        void release_reference(chuck_dynamic_array_t*          array,
                               script_instance*                context,
                               e_vm_dynamic_array_release_mode mode);
    };

    // SM3: a native's array argument; one it allocated itself goes back with it
    template<typename T>
    class script_dynamic_array {

    public:
        chuck_dynamic_array_t* array;
        bool                   allocated;

        script_dynamic_array(chuck_dynamic_array_t* arr) : array(arr),
                                                           allocated(false) {}

        // sub_8433A0
        ~script_dynamic_array() {
            destroy_array();
        }

        void destroy_array() {
            if (allocated)
                vm_dynamic_array_manager::inst()->remove_reference(array);

            array     = nullptr;
            allocated = false;
        }

        // sub_83D760
        i32 size() const {
            return (i32)array->size();
        }

        T &operator[](u32 index) {
            return *(T*)&(*array)[index].stuff[0];
        }
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

    ASSERT_SIZEOF  (script_dynamic_array<f32>,            0x08);
    ASSERT_OFFSETOF(script_dynamic_array<f32>, allocated, 0x04);
}}} // treyarch::chuck::vm
