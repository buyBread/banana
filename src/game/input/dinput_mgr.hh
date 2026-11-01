#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <windows.h>
#include <dinput.h>

#include "game/singleton.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // built by its retail create_inst
    class dinput_mgr : public singleton<dinput_mgr, 0x0111378C> {

    public:
        IDirectInput8A* direct_input;
    };

    ASSERT_SIZEOF  (dinput_mgr,               0x08);
    ASSERT_OFFSETOF(dinput_mgr, direct_input, 0x04);
} // treyarch
