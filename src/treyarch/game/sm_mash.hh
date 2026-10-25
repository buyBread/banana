#pragma once

#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch {
    namespace references {
        inline util::memory_reference<bool> generating_vtable { 0x0102F9A0 };
        inline util::memory_reference<bool> vtables_generated { 0x0102FD40 };
    } // references

    void sm_generate_vtable();
} // treyarch
