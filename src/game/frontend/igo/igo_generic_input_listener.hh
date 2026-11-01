#pragma once

#include "treyarch/mash/types.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // every input slot takes one word; the base's 15-18 hand it on to 3-6
    class igo_generic_input_listener {

    public:
        u32 unk_04;
        u32 unk_08;

        explicit igo_generic_input_listener(mash::generating_vtable_version) {}

        // sub_699520
        virtual ~igo_generic_input_listener() = default;

        // inlined into the deleting destructors
        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        virtual void unk_001(i32 unk);
        virtual void unk_002(i32 unk);
        virtual void unk_003(i32 unk);
        virtual void unk_004(i32 unk);
        virtual void unk_005(i32 unk);
        virtual void unk_006(i32 unk);
        virtual void unk_007(i32 unk);
        virtual void unk_008(i32 unk);
        virtual void unk_009(i32 unk);
        virtual void unk_010(i32 unk);
        virtual void unk_011(i32 unk);
        virtual void unk_012(i32 unk);
        virtual void unk_013(i32 unk);
        virtual void unk_014(i32 unk);
        virtual void unk_015(i32 unk);
        virtual void unk_016(i32 unk);
        virtual void unk_017(i32 unk);
        virtual void unk_018(i32 unk);
        virtual void unk_019(i32 unk);
        virtual void unk_020(i32 unk);
        virtual void unk_021(i32 unk);
        virtual void unk_022(i32 unk);
        virtual void unk_023(u32 mask);
    };

    ASSERT_SIZEOF  (igo_generic_input_listener,         0x0C);
    ASSERT_OFFSETOF(igo_generic_input_listener, unk_04, 0x04);
    ASSERT_OFFSETOF(igo_generic_input_listener, unk_08, 0x08);
} // treyarch
