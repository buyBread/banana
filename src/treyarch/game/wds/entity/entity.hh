#pragma once

#include "treyarch/game/wds/entity/interface/combo_interface.hh"
#include "treyarch/game/wds/entity/interface/player_interface.hh"
#include "treyarch/game/wds/entity/interface/time_interface.hh"
#include "treyarch/game/wds/entity/interface_storage.hh"
#include "treyarch/shared/container/fixed_vector.hh"
#include "treyarch/shared/math/po.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    struct region;

    class entity {

    public:
        void**                              vtable;
        u8                                  reserved_004[0x0C];
        po*                                 my_abs_po;
        u32                                 unk_014;
        u8                                  reserved_018[0x04];
        interface_storage*                  my_ifc_storage;
        u8                                  reserved_020[0x30];
        container::fixed_vector<region*, 8> regions;

        // inlined; the milestone's has_<name>_ifc()
        bool has_ifc(e_entity_ifc index) const {
            return my_ifc_storage && my_ifc_storage->has_ifc(index);
        }

        // inlined; the milestone's entity_base::<name>_ifc()
        time_interface* time_ifc() const {
            return (time_interface*)my_ifc_storage->get_ifc(entity_ifc_time);
        }

        player_interface* player_ifc() const {
            return (player_interface*)my_ifc_storage->get_ifc(entity_ifc_player);
        }

        combo_interface* combo_ifc() const {
            return (combo_interface*)my_ifc_storage->get_ifc(entity_ifc_combo);
        }

        void invoke_render_phase();

        region* get_primary_region();
    };

    ASSERT_OFFSETOF(entity, my_abs_po,      0x10);
    ASSERT_OFFSETOF(entity, unk_014,        0x14);
    ASSERT_OFFSETOF(entity, my_ifc_storage, 0x1C);
    ASSERT_OFFSETOF(entity, regions,        0x50);
} // treyarch
