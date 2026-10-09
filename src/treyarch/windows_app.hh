#pragma once

#include <windows.h>

#include "treyarch/shared/singleton.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    class windows_app : public singleton<windows_app, 0x011137A4> {

    public:
        HWND window;
    };

    namespace references {
        inline util::memory_reference<void*> windows_app_vtable { 0x00BF01B8 };
    } // references

    ASSERT_SIZEOF  (windows_app,         0x08);
    ASSERT_OFFSETOF(windows_app, vtable, 0x00);
    ASSERT_OFFSETOF(windows_app, window, 0x04);
} // treyarch
