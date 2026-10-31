#pragma once

#include "game/wds/entity/interface_storage.hh"
#include "game/boolx.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor; // impl?
    class conglomerate;
    class entity;

    namespace references {
        // each e_entity_ifc's name ("damage", "physical", ...) hashed, by id; filled by a crt initializer
        inline util::memory_reference<string_hash> ifc_name_hashes { 0x01003508 };
    } // references

    // SM3 eEntityInterfaceBase
    enum e_entity_interface_base : i32 {
        entity_ifc_searcher,
        entity_ifc_entity_base,
        entity_ifc_entity,
        entity_ifc_actor,
        entity_ifc_conglomerate
    };

    /* every entity interface's vtable starts the same way, in SM3's declaration order:
       mash_virtual_base (0-8), generic_interface (9-17), then the owner's set_owner (18) */
    class generic_interface : public mash::mash_virtual_base {

    public:
        boolx dynamic;
        u8    pad_005[0x03];

        explicit generic_interface(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing) {}

        // false when the interface doesn't have the parameter
        virtual bool get_ifc_num(const string_hash &name, f32* value, bool document);
        virtual bool set_ifc_num(const string_hash &name, f32 value, bool document);
        virtual bool get_ifc_vec(const string_hash &name, vector3d* value, bool document);
        virtual bool set_ifc_vec(const string_hash &name, const vector3d &value, bool document);
        virtual bool get_ifc_str(const string_hash &name, const char** value, bool document);
        virtual bool set_ifc_str(const string_hash &name, const char* value, bool document);

        virtual const char*             get_ifc_type_str() const = 0;
        virtual e_entity_ifc            get_ifc_id() const = 0; // SM3 returned a generic_id
        virtual e_entity_interface_base get_base_type() = 0;

        // sub_5FCA90
        string_hash get_ifc_name() const {
            return (&references::ifc_name_hashes.get())[get_ifc_id()];
        }
    };

    class actor_interface : public generic_interface {

    public:
        actor* my_actor;

        explicit actor_interface(mash::generating_vtable_version so_do_nothing) : generic_interface(so_do_nothing) {}

        const char*             get_ifc_type_str() const override;
        e_entity_interface_base get_base_type() override;

        virtual void set_owner(actor* owner);
    };

    class entity_interface : public generic_interface {

    public:
        entity* my_entity;

        explicit entity_interface(mash::generating_vtable_version so_do_nothing) : generic_interface(so_do_nothing) {}

        const char*             get_ifc_type_str() const override;
        e_entity_interface_base get_base_type() override;

        virtual void set_owner(entity* owner);
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
