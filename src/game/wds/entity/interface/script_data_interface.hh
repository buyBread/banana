#pragma once

#include "vc8/list.hh"
#include "game/wds/ai/param_block.hh"
#include "game/wds/entity/interface/generic_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the get/set_ifc_* slots read and write data
    class script_data_interface : public actor_interface {

    public:
        u8                            pad_00c[0x04];
        param_block                   data;
        vc8::list<arch_base_vhandle>* custom_entity_list; // made by the first add
        u8                            pad_034[0x04];

        // false when there's no such entity parameter
        virtual bool get_custom_ent(const string_hash &name, arch_base_vhandle* value, bool unk);
        virtual bool set_custom_ent(const string_hash &name, arch_base_vhandle value, bool unk);

        bool has_entity_in_custom_list(); // the list isn't empty

        // both skip handles that don't resolve
        void add_entity_to_custom_entity_list     (arch_base_vhandle ent);
        void remove_entity_from_custom_entity_list(arch_base_vhandle ent);

        arch_base_vhandle get_first_entity_from_custom_entity_list();
        i32               get_custom_entity_list_count();

        void set_als_anim_filename_use_prefix(bool use_prefix);
    };

    ASSERT_SIZEOF  (script_data_interface,                     0x38);
    ASSERT_OFFSETOF(script_data_interface, data,               0x10);
    ASSERT_OFFSETOF(script_data_interface, custom_entity_list, 0x30);
} // treyarch
