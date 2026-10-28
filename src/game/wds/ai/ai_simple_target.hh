#pragma once

#include "game/arch_base_vhandle.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    enum e_simple_target_type : i32 {
        simple_target_type_none     = 0, // dropped wherever an inode finds it
        simple_target_type_entity   = 1,
        simple_target_type_location = 2
    };

    namespace references {
        inline util::memory_reference<void*> ai_simple_target_entity_vtable   { 0x00B992D4 };
        inline util::memory_reference<void*> ai_simple_target_location_vtable { 0x00B992E4 };
    } // references

    // what an ai inode keeps per target slot
    class ai_simple_target {

    public:
        void** vtable;

        // slot 0
        void destroy() {
            ((void (__thiscall*)(ai_simple_target*, u32))vtable[0])(this, 1);
        }

        // slot 1; the key is a target collection's, neither leaf reads it
        vector3d get_position(string_hash key) {
            vector3d result;

            ((vector3d* (__thiscall*)(ai_simple_target*, vector3d*, string_hash))vtable[1])(this, &result, key);

            return result;
        }

        // slot 2
        e_simple_target_type get_type() {
            return ((e_simple_target_type (__thiscall*)(ai_simple_target*))vtable[2])(this);
        }
    };

    class ai_simple_target_entity : public ai_simple_target {

    public:
        arch_base_vhandle target;

        // inlined @ sub_4E57E0
        explicit ai_simple_target_entity(arch_base_vhandle handle) : target(handle) {
            vtable = (void**)&references::ai_simple_target_entity_vtable.get();
        }

        // sub_44F9D0
        arch_base* resolve() const { // asks the handle twice
            return target.resolve() ? target.resolve() : nullptr;
        }
    };

    class ai_simple_target_location : public ai_simple_target {

    public:
        vector3d point;

        // sub_4E2230
        explicit ai_simple_target_location(const vector3d &location) : point(location) {
            vtable = (void**)&references::ai_simple_target_location_vtable.get();
        }
    };

    ASSERT_SIZEOF  (ai_simple_target_entity,           0x08);
    ASSERT_OFFSETOF(ai_simple_target_entity, target,   0x04);
    ASSERT_SIZEOF  (ai_simple_target_location,         0x10);
    ASSERT_OFFSETOF(ai_simple_target_location, point,  0x04);
} // treyarch
