#pragma once

#include "game/arch_base.hh"
#include "game/boolx.hh"
#include "game/input/game_axis.hh"
#include "game/input/game_button.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the pad as scripts see it; one exists, made when the script system starts
    class script_controller : public arch_base {

    public:
        boolx       enabled;     // everything reads as 0 while it's clear
        u8          pad_009[0x07];
        game_button buttons[25]; // by the script's button number, unchecked
        game_axis   axes[4];     // by the script's axis number, unchecked
    };

    namespace references {
        // set while the confirm and cancel buttons are swapped
        inline util::memory_reference<u8> x_and_o_inverted { 0x0102CDB4 };
    } // references

    ASSERT_SIZEOF  (script_controller,          0x8DC);
    ASSERT_OFFSETOF(script_controller, buttons, 0x010);
    ASSERT_OFFSETOF(script_controller, axes,    0x77C);
} // treyarch
