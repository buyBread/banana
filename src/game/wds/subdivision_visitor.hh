#pragma once

#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // vtable slot 1 receives each region the query reaches
    struct subdivision_visitor {
        void* vtable;
        void* visited; // 0xC4 bytes from a shared pool between sub_779710 and sub_7797D0
    };

    namespace references {
        // held around visitor queries (sub_5891C0, sub_78D520, sub_7CB590, ...)
        inline util::memory_reference<engine_recursive_lock> subdivision_query_lock { 0x01075E10 };
    } // references

    ASSERT_SIZEOF  (subdivision_visitor,          0x08);
    ASSERT_OFFSETOF(subdivision_visitor, visited, 0x04);
} // treyarch
