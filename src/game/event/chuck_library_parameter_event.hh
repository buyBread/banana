#pragma once

#include "game/event/event.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the base of the script library's chuck_sle_* events; only ever built by retail
    class chuck_library_parameter_event : public event {

    public:
        ~chuck_library_parameter_event() override;
    };

    class chuck_sle_city_life_completed_t : public chuck_library_parameter_event {

    public:
        f32 spawn_id;

        ~chuck_sle_city_life_completed_t() override;
    };

    ASSERT_SIZEOF  (chuck_library_parameter_event, 0x10);

    ASSERT_SIZEOF  (chuck_sle_city_life_completed_t,           0x14);
    ASSERT_OFFSETOF(chuck_sle_city_life_completed_t, spawn_id, 0x10);
} // treyarch
