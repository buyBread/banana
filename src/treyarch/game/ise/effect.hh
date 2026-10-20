#pragma once

#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ise {
    // the base of every effect; mashed ones get their retail vtable from the type registry
    class effect : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x54]; // todo

        // slot 12; builds the effect's state at +0x14
        void library_loaded() {
            ((void (__thiscall*)(effect*))(*(void***)this)[12])(this);
        }

        // slot 13
        void library_unloaded() {
            ((void (__thiscall*)(effect*))(*(void***)this)[13])(this);
        }
    };

    ASSERT_SIZEOF(effect, 0x58);
}} // treyarch::ise
