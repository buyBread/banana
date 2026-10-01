#pragma once

#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class generic_interface;

    enum e_entity_ifc : u8 {
        entity_ifc_damage,
        entity_ifc_physical,
        entity_ifc_standing,
        entity_ifc_script_data,
        entity_ifc_manip_obj,
        entity_ifc_switch_obj,
        entity_ifc_weapon,
        entity_ifc_thrown_weapon,
        entity_ifc_gun_weapon,
        entity_ifc_melee_weapon,
        entity_ifc_time,
        entity_ifc_player,
        entity_ifc_variant,
        entity_ifc_simple_variant,
        entity_ifc_towing,
        entity_ifc_card,
        entity_ifc_occupant,
        entity_ifc_morph,
        entity_ifc_rpg,
        entity_ifc_combo,
        entity_ifc_ise,     // ise, ise_variant and red_and_black_ise
        entity_ifc_tentacle,
        entity_ifc_sound
    };

    // SM3 entity_base::my_ifc_storage; retail keeps SM3's interface vector and adds the lock and presence mask
    class interface_storage {

    public:
        mash::vector<generic_interface> interfaces; // packed in index order of the set mask bits
        u8                              reserved_014[0x04];
        engine_recursive_lock           ifc_lock;
        u32                             ifc_mask;   // bit n is set while the interface with index n exists

        // inlined; retail reads the mask without taking ifc_lock
        bool has_ifc(e_entity_ifc index) const {
            return (ifc_mask >> index) & 1;
        }

        generic_interface* get_ifc(e_entity_ifc index);
    };

    ASSERT_OFFSETOF(interface_storage, interfaces, 0x00);
    ASSERT_OFFSETOF(interface_storage, ifc_lock,   0x18);
    ASSERT_OFFSETOF(interface_storage, ifc_mask,   0x28);
} // treyarch
