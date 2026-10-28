#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // a vehicle's seats and who's in them
    class occupant_interface : public conglomerate_interface {

    public:
        static const i32 _CAR_NUM_SEATS = 6;

        arch_base_vhandle m_potential_seat_occupants[_CAR_NUM_SEATS]; // on their way to the seat
        arch_base_vhandle m_seat_occupants[_CAR_NUM_SEATS];
        i32               m_potential_vehicle_count;
        i32               m_vehicle_occupant_count;
        u8                reserved_044[0x04];
        bool              disabled_escape_enabled;
        bool              eject_kills_occupants;
        u8                pad_04a[0x02];
        f32               occupant_damage_tolerance_percentage;
        bool              clown_car;
        u8                reserved_051[0x0F];
    };

    ASSERT_SIZEOF  (occupant_interface,                                       0x60);
    ASSERT_OFFSETOF(occupant_interface, m_potential_seat_occupants,           0x0C);
    ASSERT_OFFSETOF(occupant_interface, m_seat_occupants,                     0x24);
    ASSERT_OFFSETOF(occupant_interface, m_potential_vehicle_count,            0x3C);
    ASSERT_OFFSETOF(occupant_interface, m_vehicle_occupant_count,             0x40);
    ASSERT_OFFSETOF(occupant_interface, disabled_escape_enabled,              0x48);
    ASSERT_OFFSETOF(occupant_interface, eject_kills_occupants,                0x49);
    ASSERT_OFFSETOF(occupant_interface, occupant_damage_tolerance_percentage, 0x4C);
    ASSERT_OFFSETOF(occupant_interface, clown_car,                            0x50);
} // treyarch
