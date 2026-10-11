#pragma once

#include "treyarch/game/speedtree/wrapper.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"

namespace treyarch {
    struct speed_tree_billboards_only_node {
        ngl::render_node                  base;
        speed_tree_wrapper*               wrapper;
        speed_tree_billboards_only_params params;

        // sub_7D7F70
        void render() {
            wrapper->render_billboards_only(&params);
        }
    };

    struct speed_tree_billboards_node {
        ngl::render_node         base;
        speed_tree_wrapper*      wrapper;
        speed_tree_render_params params;

        // sub_7D7F80
        void render() {
            wrapper->render_billboards(&params);
        }
    };

    struct speed_tree_branches_node {
        ngl::render_node         base;
        speed_tree_wrapper*      wrapper;
        speed_tree_render_params params;

        // sub_7D7F90
        void render() {
            wrapper->render_branches(&params);
        }
    };

    struct speed_tree_fronds_node {
        ngl::render_node         base;
        speed_tree_wrapper*      wrapper;
        speed_tree_render_params params;

        // sub_7D7FA0
        void render() {
            wrapper->render_fronds(&params);
        }
    };

    struct speed_tree_leafs_node {
        ngl::render_node         base;
        speed_tree_wrapper*      wrapper;
        speed_tree_render_params params;

        // sub_7D7FB0
        void render() {
            wrapper->render_leafs(&params);
        }
    };

    namespace references {
        inline util::memory_reference<void*> speed_tree_billboards_only_node_vtable { 0x00BD9804 };
        inline util::memory_reference<void*> speed_tree_billboards_node_vtable      { 0x00BD9838 };
        inline util::memory_reference<void*> speed_tree_branches_node_vtable        { 0x00BD9868 };
        inline util::memory_reference<void*> speed_tree_fronds_node_vtable          { 0x00BD9898 };
        inline util::memory_reference<void*> speed_tree_leafs_node_vtable           { 0x00BD98C4 };
    } // references

    ASSERT_SIZEOF  (speed_tree_billboards_only_node,          0x18);
    ASSERT_OFFSETOF(speed_tree_billboards_only_node, wrapper, 0x0C);
    ASSERT_OFFSETOF(speed_tree_billboards_only_node, params,  0x10);

    ASSERT_SIZEOF  (speed_tree_billboards_node,          0x50);
    ASSERT_OFFSETOF(speed_tree_billboards_node, wrapper, 0x0C);
    ASSERT_OFFSETOF(speed_tree_billboards_node, params,  0x10);

    ASSERT_SIZEOF(speed_tree_branches_node, 0x50);
    ASSERT_SIZEOF(speed_tree_fronds_node,   0x50);
    ASSERT_SIZEOF(speed_tree_leafs_node,    0x50);
} // treyarch
