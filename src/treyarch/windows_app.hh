#pragma once

#include <windows.h>

#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    class windows_app {

    public:
        void** vtable;
        HWND   window;
    };

    namespace references {
        inline util::memory_reference<void*>        windows_app_vtable { 0x00BF01B8 };
        inline util::memory_reference<windows_app*> application        { 0x011137A4 };
    } // references

    ASSERT_SIZEOF  (windows_app,         0x08);
    ASSERT_OFFSETOF(windows_app, vtable, 0x00);
    ASSERT_OFFSETOF(windows_app, window, 0x04);
} // treyarch
