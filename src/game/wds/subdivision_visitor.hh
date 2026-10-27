#pragma once

#include "game/mutex.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct subdivision_node;

    enum e_visit_result : i32 {
        continue_visiting              = 0,
        early_exit_within_this_cell    = 1,
        stop_visiting_farther_cells    = 2,
        stop_visiting_within_this_cell = 3
    };

    using subdivision_leaf_handle = subdivision_node*;

    // visit receives each leaf the query reaches
    struct subdivision_visitor {
        subdivision_visitor() : visited(nullptr) {}

        // sub_968AF0
        virtual ~subdivision_visitor() {}

        virtual e_visit_result visit(subdivision_leaf_handle leaf_handle) = 0;

        void* visited; // 0xC4 bytes from a shared pool between sub_779710 and sub_7797D0
    };

    namespace references {
        // held around visitor queries (sub_5891C0, sub_78D520, sub_7CB590, ...)
        inline util::memory_reference<engine_recursive_lock> subdivision_query_lock { 0x01075E10 };
    } // references

    ASSERT_SIZEOF  (subdivision_visitor,          0x08);
    ASSERT_OFFSETOF(subdivision_visitor, visited, 0x04);
} // treyarch
