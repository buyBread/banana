#include <cassert>
#include <cstdlib>

#include "treyarch/shared/hash/algo.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/gimmie/fn.hh"
#include "util/memory_reference.hh"

using namespace treyarch;

namespace treyarch { namespace mash { namespace detail {
    // overlays for the two old Dinkumware hash maps the executable builds during static initialization
    struct vtable_registry_node {
        vtable_registry_node* next;
        vtable_registry_node* previous;
        vtable_key_t          key;
        vtable_value_t        value;
    };

    struct vtable_registry {
        u32                    unknown_00;
        u32                    unknown_04;
        vtable_registry_node*  head;
        u32                    unknown_0c;
        u32                    unknown_10;
        vtable_registry_node** bucket_bounds;
        u32                    unknown_18;
        u32                    unknown_1c;
        u32                    bucket_mask;
        u32                    bucket_count;
    };

    ASSERT_SIZEOF  (vtable_registry_node,           0x10);
    ASSERT_OFFSETOF(vtable_registry_node, next,     0x00);
    ASSERT_OFFSETOF(vtable_registry_node, previous, 0x04);
    ASSERT_OFFSETOF(vtable_registry_node, key,      0x08);
    ASSERT_OFFSETOF(vtable_registry_node, value,    0x0C);

    ASSERT_SIZEOF  (vtable_registry,                0x28);
    ASSERT_OFFSETOF(vtable_registry, head,          0x08);
    ASSERT_OFFSETOF(vtable_registry, bucket_bounds, 0x14);
    ASSERT_OFFSETOF(vtable_registry, bucket_mask,   0x20);
    ASSERT_OFFSETOF(vtable_registry, bucket_count,  0x24);

    static util::memory_reference<vtable_registry> vtable_registry_reference { 0x01126CB4 };

    static vtable_registry_node* find_vtable_entry(vtable_key_t key) {
        vtable_registry &registry = vtable_registry_reference.get();

        // this ugly little park-miller mix is exactly what retail uses before applying the bucket mask
        div_t division = std::div((i32)(key ^ 0xDEADBEEF), 127773);
        i32 mixed_key = 16807 * division.rem - 2836 * division.quot;

        if (mixed_key < 0)
            mixed_key += I32_MAX;

        u32 bucket = (u32)mixed_key & registry.bucket_mask;

        if (registry.bucket_count <= bucket)
            bucket += -1 - (registry.bucket_mask >> 1);

        vtable_registry_node* node = registry.bucket_bounds[bucket];
        vtable_registry_node* end  = registry.bucket_bounds[bucket + 1];

        while (node != end && node->key < key)
            node = node->next;

        if (node == end || key < node->key)
            return registry.head;

        return node;
    }
}}} // treyarch::mash::detail

mash::virtual_types_key mash::mash_virtual_base::get_virtual_type_key() const {
    return generate_virtual_types_key_from_string("mash_virtual_base");
}

void mash::mash_virtual_base::copy_values(const mash_virtual_base* other) {
    assert(other != nullptr);
    assert(get_virtual_type_key() == other->get_virtual_type_key());
}

bool mash::mash_virtual_base::verify_valid_vtable_map(void* what_class) {
    detail::vtable_registry &registry = detail::vtable_registry_reference.get();

    vtable_value_t vtable = *(vtable_value_t*)what_class;

    for (detail::vtable_registry_node* node = registry.head->next; node != registry.head; node = node->next) {
        if (node->value == vtable)
            return true;
    }

    return false;
}

bool mash::mash_virtual_base::verify_valid_vtable(void* what_class) {
    return verify_valid_vtable_map(what_class);
}

mash::virtual_types_key mash::mash_virtual_base::generate_virtual_types_key_from_string(const char* string) {
    return hash::djb2(string);
}

bool mash::mash_virtual_base::is_or_is_subclass_of(virtual_types_key parent_class) const {
    return get_virtual_type_key() == parent_class || is_subclass_of(parent_class);
}

mash::mash_virtual_base* mash::mash_virtual_base::create_subclass_in_place(virtual_types_key  class_to_make,
                                                                           mash_virtual_base* memory_buffer) {

    *(virtual_types_key*)memory_buffer = class_to_make;

    return construct_class_helper(memory_buffer);
}

void mash::mash_virtual_base::fixup_vtable(void* what_class_to_fixup) {
    // the first image word is a type key until this replaces it with an executable vtable
    vtable_key_t   key   = *(vtable_key_t*)what_class_to_fixup;
    vtable_value_t value = lookup_vtable_entry(key);

    if (value != invalid_vtable_value)
        *(vtable_value_t*)what_class_to_fixup = value;
}

mash::mash_virtual_base* mash::mash_virtual_base::construct_class_helper(void* class_ptr) {
    fixup_vtable(class_ptr);

    mash_virtual_base* mashed_class = (mash_virtual_base*)class_ptr;
    mashed_class->construct_mashed_class();

    return mashed_class;
}

bool mash::mash_virtual_base::register_vtable_entry(const vtable_key_t   &key,
                                                    const vtable_value_t &value) {

    using register_function = bool (__cdecl*)(const vtable_key_t*, const vtable_value_t*);

    // insertion mutates two process-owned STL maps; keep that allocator-sensitive mess on the retail side (for now)
    return util::gimmie::fn<register_function>(0x00A6C760)(&key, &value);
}

mash::vtable_value_t mash::mash_virtual_base::lookup_vtable_entry(const vtable_key_t &key) {
    detail::vtable_registry_node*  node     = detail::find_vtable_entry(key);
    detail::vtable_registry       &registry = detail::vtable_registry_reference.get();

    return node == registry.head ? invalid_vtable_value : node->value;
}
