#pragma once

#include "treyarch/shared/mash/types.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace mash {
    class mash_info_struct;

    class mash_virtual_base {

    public:
        // order matters here; retail calls these by the offsets documented in the mash notes
        virtual void construct_mashed_class() {}
        virtual void destruct_mashed_class() {}

                 mash_virtual_base() = default;
        explicit mash_virtual_base(generating_vtable_version) {}
        virtual ~mash_virtual_base() = default;

        virtual void unmash(mash_info_struct*, void*, buffer_type) {}
        virtual virtual_types_key get_virtual_type_key() const;
        virtual void copy_values(const mash_virtual_base* other);

        static bool verify_valid_vtable_map(void* what_class);
        static bool verify_valid_vtable(void* what_class);

        static virtual_types_key generate_virtual_types_key_from_string(const char* string);

        virtual bool is_subclass_of(virtual_types_key) const { return false; }
        virtual bool is_or_is_subclass_of(virtual_types_key parent_class) const;

        virtual i32 get_mash_sizeof() const { return sizeof(*this); }

        static mash_virtual_base* create_subclass_in_place(virtual_types_key  class_to_make,
                                                           mash_virtual_base* memory_buffer);

        static void fixup_vtable(void* what_class_to_fixup);

        static mash_virtual_base* construct_class_helper(void* class_ptr);

        static bool register_vtable_entry(const vtable_key_t   &key,
                                          const vtable_value_t &value);

        static vtable_value_t lookup_vtable_entry(const vtable_key_t &key);
    };

    ASSERT_SIZEOF(mash_virtual_base, 0x04);
}} // treyarch::mash
