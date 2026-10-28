#pragma once

#include "vc8/list.hh"
#include "game/wds/ai/param_block.hh"
#include "game/wds/entity/interface/generic_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class script_data_interface;

    struct script_data_interface_vtable : generic_interface_vtable {
        bool (__thiscall* get_custom_ent)(script_data_interface* self, const string_hash &name, arch_base_vhandle* value, bool unk); // sub_610150
        bool (__thiscall* set_custom_ent)(script_data_interface* self, const string_hash &name, arch_base_vhandle value, bool unk);  // sub_600EC0
    };

    // the get/set_ifc_* slots read and write data
    class script_data_interface : public entity_interface {

    public:
        u8                            pad_00c[0x04];
        param_block                   data;
        vc8::list<arch_base_vhandle>* custom_entity_list; // made by the first add
        u8                            pad_034[0x04];

        script_data_interface_vtable* get_vtable() const {
            return (script_data_interface_vtable*)vtable;
        }

        // false when there's no such entity parameter
        bool get_custom_ent(const string_hash &name, arch_base_vhandle* value, bool unk) {
            return get_vtable()->get_custom_ent(this, name, value, unk);
        }

        bool set_custom_ent(const string_hash &name, arch_base_vhandle value, bool unk) {
            return get_vtable()->set_custom_ent(this, name, value, unk);
        }

        bool has_entity_in_custom_list(); // the list isn't empty

        // both skip handles that don't resolve
        void add_entity_to_custom_entity_list     (arch_base_vhandle ent);
        void remove_entity_from_custom_entity_list(arch_base_vhandle ent);

        arch_base_vhandle get_first_entity_from_custom_entity_list();
        i32               get_custom_entity_list_count();
    };

    ASSERT_SIZEOF  (script_data_interface_vtable, 21 * 4);

    ASSERT_SIZEOF  (script_data_interface,                     0x38);
    ASSERT_OFFSETOF(script_data_interface, data,               0x10);
    ASSERT_OFFSETOF(script_data_interface, custom_entity_list, 0x30);
} // treyarch
