#pragma once

#include "treyarch/game/wds/entity/interface_storage.hh"
#include "treyarch/shared/boolx.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class actor; // impl?
    class entity;
    class generic_interface;

    /* the retail vtable prefix shared by every entity interface. it follows SM3's declaration order:
       mash_virtual_base (0-8), generic_interface (9-17), then actor_/entity_interface::set_owner (18).
       retail confirms the order through time_interface, which overrides only get/set_ifc_num of slots 9-14 */
    struct generic_interface_vtable {
        void*        mash_virtual_base_slots[9];
        void*        get_ifc_num;
        void*        set_ifc_num;
        void*        get_ifc_vec;
        void*        set_ifc_vec;
        void*        get_ifc_str;
        void*        set_ifc_str;
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
    };

    class actor_interface : public generic_interface {

    public:
        actor* my_actor;
    };

    class entity_interface : public generic_interface {

    public:
        entity* my_entity;
    };

    ASSERT_OFFSETOF(generic_interface, dynamic,   0x04);
    ASSERT_OFFSETOF(actor_interface,   my_actor,  0x08);
    ASSERT_OFFSETOF(entity_interface,  my_entity, 0x08);
} // treyarch
