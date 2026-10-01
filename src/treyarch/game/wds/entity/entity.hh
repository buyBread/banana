#pragma once

#include "treyarch/game/wds/entity/interface/combo_interface.hh"
#include "treyarch/game/wds/entity/interface/player_interface.hh"
#include "treyarch/game/wds/entity/interface/time_interface.hh"
#include "treyarch/game/wds/entity/interface_storage.hh"
#include "treyarch/shared/math/po.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class entity {

    public:
        void**             vtable;
        u8                 reserved_004[0x0C];
        po*                my_abs_po;
        u8                 reserved_014[0x08];
        interface_storage* my_ifc_storage;

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
    };

    ASSERT_OFFSETOF(entity, my_abs_po,      0x10);
    ASSERT_OFFSETOF(entity, my_ifc_storage, 0x1C);
} // treyarch
