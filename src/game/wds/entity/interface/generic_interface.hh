#pragma once

#include "game/wds/entity/interface_storage.hh"
#include "game/boolx.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor; // impl?
    class conglomerate;
    class entity;
    class generic_interface;

    namespace references {
        // each e_entity_ifc's name ("damage", "physical", ...) hashed, by id; filled by a crt initializer
        inline util::memory_reference<string_hash> ifc_name_hashes { 0x01003508 };
    } // references

    /* the retail vtable prefix shared by every entity interface. it follows SM3's declaration order:
       mash_virtual_base (0-8), generic_interface (9-17), then actor_/entity_interface::set_owner (18). */
    struct generic_interface_vtable {
        void*        mash_virtual_base_slots[9];
        bool         (__thiscall* get_ifc_num)(generic_interface* self, const string_hash &name, f32* value, bool document);
        bool         (__thiscall* set_ifc_num)(generic_interface* self, const string_hash &name, f32 value, bool document);
        bool         (__thiscall* get_ifc_vec)(generic_interface* self, const string_hash &name, vector3d* value, bool document);
        bool         (__thiscall* set_ifc_vec)(generic_interface* self, const string_hash &name, const vector3d* value, bool document);
        bool         (__thiscall* get_ifc_str)(generic_interface* self, const string_hash &name, const char** value, bool document);
        bool         (__thiscall* set_ifc_str)(generic_interface* self, const string_hash &name, const char* value, bool document);
        const char*  (__thiscall* get_ifc_type_str)(const generic_interface* self);
        e_entity_ifc (__thiscall* get_ifc_id)(const generic_interface* self); // SM3 returned a generic_id
        i32          (__thiscall* get_base_type)(generic_interface* self);
        void*        set_owner;
    };

    ASSERT_SIZEOF(generic_interface_vtable, 19 * 4);

    class generic_interface {

    public:
        generic_interface_vtable* vtable;
        boolx                     dynamic;
        u8                        pad_005[0x03];

        e_entity_ifc get_ifc_id() const {
            return vtable->get_ifc_id(this);
        }

        // sub_5FCA90
        string_hash get_ifc_name() const {
            return (&references::ifc_name_hashes.get())[get_ifc_id()];
        }

        // false when the interface doesn't have the parameter
        bool get_ifc_num(const string_hash &name, f32* value, bool document) {
            return vtable->get_ifc_num(this, name, value, document);
        }

        bool set_ifc_num(const string_hash &name, f32 value, bool document) {
            return vtable->set_ifc_num(this, name, value, document);
        }

        bool get_ifc_vec(const string_hash &name, vector3d* value, bool document) {
            return vtable->get_ifc_vec(this, name, value, document);
        }

        bool set_ifc_vec(const string_hash &name, const vector3d &value, bool document) {
            return vtable->set_ifc_vec(this, name, &value, document);
        }

        bool get_ifc_str(const string_hash &name, const char** value, bool document) {
            return vtable->get_ifc_str(this, name, value, document);
        }

        bool set_ifc_str(const string_hash &name, const char* value, bool document) {
            return vtable->set_ifc_str(this, name, value, document);
        }
    };

    class actor_interface : public generic_interface {

    public:
        actor* my_actor;
    };

    class entity_interface : public generic_interface {

    public:
        entity* my_entity;
    };

    class conglomerate_interface : public generic_interface {

    public:
        conglomerate* my_conglomerate;
    };

    ASSERT_OFFSETOF(generic_interface,      dynamic,         0x04);
    ASSERT_OFFSETOF(actor_interface,        my_actor,        0x08);
    ASSERT_OFFSETOF(entity_interface,       my_entity,       0x08);
    ASSERT_OFFSETOF(conglomerate_interface, my_conglomerate, 0x08);
} // treyarch
