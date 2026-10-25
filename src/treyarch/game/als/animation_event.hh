#pragma once

#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace als {
    // raised when the state's animation passes time; mashed ones get their retail vtables from the type registry
    class animation_event : public mash::mash_virtual_base {

    public:
        string_hash name;
        f32         time; // seconds into the animation
    };

    class animation_event_with_param : public animation_event {

    public:
        u32 param_type; // 1 an integer, 2 a float, 3 a string_hash
        u32 param;
    };

    ASSERT_SIZEOF  (animation_event,       0x0C);
    ASSERT_OFFSETOF(animation_event, name, 0x04);
    ASSERT_OFFSETOF(animation_event, time, 0x08);

    ASSERT_SIZEOF  (animation_event_with_param,             0x14);
    ASSERT_OFFSETOF(animation_event_with_param, param_type, 0x0C);
    ASSERT_OFFSETOF(animation_event_with_param, param,      0x10);
}} // treyarch::als
