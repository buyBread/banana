#pragma once

#include <new>

#include "treyarch/mash/types.hh"
#include "treyarch/mm/mm.hh"
#include "vc8/hash_map.hh"
#include "vc8/hash_set.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch { namespace mash {
    class mash_info_struct;

    class mash_virtual_base {

    public:
                 mash_virtual_base() = default;
        explicit mash_virtual_base(generating_vtable_version) {}

        // sub_42AAF0
        virtual ~mash_virtual_base() = default;

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        virtual void construct_mashed_class() {}
        virtual void destruct_mashed_class() {}
        virtual void unmash(mash_info_struct*, void*, buffer_type) {}
        virtual virtual_types_key get_virtual_type_key() const;
        virtual void copy_values(const mash_virtual_base* other);

        static bool verify_valid_vtable(void*) { return true; }

        static virtual_types_key generate_virtual_types_key_from_string(const char* string);

        virtual bool is_subclass_of(virtual_types_key) const { return false; }
        virtual bool is_or_is_subclass_of(virtual_types_key parent_class) const;

        virtual i32 get_mash_sizeof() const { return sizeof(*this); }

        static void fixup_vtable(void* what_class_to_fixup);

        static mash_virtual_base* construct_class_helper(void* class_ptr);

        static bool register_vtable_entry(const vtable_key_t   &key,
                                          const vtable_value_t &value);

        // what the generated registration list does per class: build a generating-version one in scratch memory (never destroyed) and register its vtable
        template<typename T>
        static bool register_class() {
            alignas(T) u8 scratch[sizeof(T)];

            T* object = ::new (scratch) T(GENERATING_VTABLES_VERSION);

            return register_vtable_entry(T::my_type_key.source_hash_code, *(vtable_value_t*)object);
        }

        static vtable_value_t lookup_vtable_entry(const vtable_key_t &key);

    private:
        static virtual_types_key type_key;
    };

    ASSERT_SIZEOF(mash_virtual_base, 0x04);

    namespace references {
        inline util::memory_reference<vc8::hash_map<vtable_key_t, vtable_value_t>> vtable_map { 0x01126CB4 };

        // every vtable ever registered, nothing reads it back
        inline util::memory_reference<vc8::hash_set<vtable_value_t>> vtable_set { 0x01126C8C };
    } // references
}} // treyarch::mash
