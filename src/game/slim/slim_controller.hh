#pragma once

#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch { namespace slim {
    // runs one machine for one owner
    class slim_generic_controller {

    public:
        void**                   vtable;
        u8                       reserved_004[0x34];
        u32                      unk_038;            // strategy 1, tactics 2
        u32                      flags;              // the ai frames copy 0x80 into the strategy and tactics controllers
        u8                       reserved_040[0x0C];
        slim_generic_controller* unk_04c;            // the controller it was made under, else itself
        u8                       reserved_050[0x1C];

        // slot 2
        void unk_002(f32 delta_t) {
            ((void (__thiscall*)(slim_generic_controller*, f32))vtable[2])(this, delta_t);
        }
    };

    class slim_actor_controller : public slim_generic_controller {

    public:
        u8  reserved_06c[0x0C];
        i32 unk_078;            // both take the exit code of the combat frame it runs under
        i32 unk_07c;
    };

    ASSERT_SIZEOF  (slim_generic_controller,          0x6C);
    ASSERT_OFFSETOF(slim_generic_controller, unk_038, 0x38);
    ASSERT_OFFSETOF(slim_generic_controller, flags,   0x3C);
    ASSERT_OFFSETOF(slim_generic_controller, unk_04c, 0x4C);

    ASSERT_SIZEOF  (slim_actor_controller,          0x80);
    ASSERT_OFFSETOF(slim_actor_controller, unk_07c, 0x7C);
}} // treyarch::slim
