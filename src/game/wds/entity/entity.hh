#pragma once

#include "game/arch_base.hh"
#include "game/wds/entity/interface/combo_interface.hh"
#include "game/wds/entity/interface/damage_interface.hh"
#include "game/wds/entity/interface/physical_interface.hh"
#include "game/wds/entity/interface/player_interface.hh"
#include "game/wds/entity/interface/switch_obj_interface.hh"
#include "game/wds/entity/interface/time_interface.hh"
#include "game/wds/entity/interface/towing_interface.hh"
#include "game/wds/entity/interface_storage.hh"
#include "game/container/fixed_vector.hh"
#include "treyarch/mash/string_hash.hh"
#include "game/math/po.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct region;

    class entity : public arch_base {

    public:
        string_hash                         id;
        po*                                 my_rel_po;
        po*                                 my_abs_po;
        u32                                 flags;
        u32                                 ext_flags;
        interface_storage*                  my_ifc_storage;
        u8                                  reserved_020[0x1A];
        u8                                  unk_03a; // cleared as scene entities are loaded
        u8                                  reserved_03b[0x15];
        container::fixed_vector<region*, 8> regions;

        // inlined; the milestone's has_<name>_ifc()
        bool has_ifc(e_entity_ifc index) const {
            return my_ifc_storage && my_ifc_storage->has_ifc(index);
        }

        // inlined; the milestone's entity_base::<name>_ifc()
        damage_interface* damage_ifc() const {
            return (damage_interface*)my_ifc_storage->get_ifc(entity_ifc_damage);
        }

        physical_interface* physical_ifc() const {
            return (physical_interface*)my_ifc_storage->get_ifc(entity_ifc_physical);
        }

        time_interface* time_ifc() const {
            return (time_interface*)my_ifc_storage->get_ifc(entity_ifc_time);
        }

        player_interface* player_ifc() const {
            return (player_interface*)my_ifc_storage->get_ifc(entity_ifc_player);
        }

        combo_interface* combo_ifc() const {
            return (combo_interface*)my_ifc_storage->get_ifc(entity_ifc_combo);
        }

        towing_interface* towing_ifc() const {
            return (towing_interface*)my_ifc_storage->get_ifc(entity_ifc_towing);
        }

        switch_obj_interface* switch_obj_ifc() const {
            return (switch_obj_interface*)my_ifc_storage->get_ifc(entity_ifc_switch_obj);
        }

        // SM3's EFLAG_GRAPHICS_VISIBLE, unless ext flag 0x1000 vetoes it
        bool is_visible() const {
            return (flags & 0x200) && !(ext_flags & 0x1000);
        }

        // slot 67
        void set_visible(bool visible, bool unk) {
            ((void (__thiscall*)(entity*, bool, bool))vtable[67])(this, visible, unk);
        }

        void invoke_render_phase();

        generic_interface* find_ifc_by_name(string_hash name);

        region* get_primary_region();
    };

    ASSERT_OFFSETOF(entity, id,             0x08);
    ASSERT_OFFSETOF(entity, my_rel_po,      0x0C);
    ASSERT_OFFSETOF(entity, my_abs_po,      0x10);
    ASSERT_OFFSETOF(entity, flags,          0x14);
    ASSERT_OFFSETOF(entity, ext_flags,      0x18);
    ASSERT_OFFSETOF(entity, my_ifc_storage, 0x1C);
    ASSERT_OFFSETOF(entity, unk_03a,        0x3A);
    ASSERT_OFFSETOF(entity, regions,        0x50);
} // treyarch
