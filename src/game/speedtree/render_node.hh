#pragma once

#include "game/speedtree/wrapper.hh"
#include "treyarch/ngl/list/render_node.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"

namespace treyarch {
    struct speed_tree_billboards_only_node : ngl::render_node {
        speed_tree_wrapper*               wrapper;
        speed_tree_billboards_only_params params;

        // sub_7D7F70
        void render() override {
            wrapper->render_billboards_only(&params);
        }

        // sub_7BC730
        void get_desc(char* desc) override {
            std::strcpy(desc, "SpeedTreeBillboardsOnlyNode");
        }
    };

    struct speed_tree_billboards_node : ngl::render_node {
        speed_tree_wrapper*      wrapper;
        speed_tree_render_params params;

        // sub_7D7F80
        void render() override {
            wrapper->render_billboards(&params);
        }

        // sub_7BC770
        void get_desc(char* desc) override {
            std::strcpy(desc, "SpeedTreeBillboardsNode");
        }
    };

    struct speed_tree_branches_node : ngl::render_node {
        speed_tree_wrapper*      wrapper;
        speed_tree_render_params params;

        // sub_7D7F90
        void render() override {
            wrapper->render_branches(&params);
        }

        // sub_7BC7A0
        void get_desc(char* desc) override {
            std::strcpy(desc, "SpeedTreeBranchesNode");
        }
    };

    struct speed_tree_fronds_node : ngl::render_node {
        speed_tree_wrapper*      wrapper;
        speed_tree_render_params params;

        // sub_7D7FA0
        void render() override {
            wrapper->render_fronds(&params);
        }

        // sub_7BC7E0
        void get_desc(char* desc) override {
            std::strcpy(desc, "SpeedTreeFrondsNode");
        }
    };

    struct speed_tree_leafs_node : ngl::render_node {
        speed_tree_wrapper*      wrapper;
        speed_tree_render_params params;

        // sub_7D7FB0
        void render() override {
            wrapper->render_leafs(&params);
        }

        // sub_7BC810
        void get_desc(char* desc) override {
            std::strcpy(desc, "SpeedTreeLeafsNode");
        }
    };

    namespace references {
        inline util::memory_reference<void*> speed_tree_billboards_only_node_vftable { 0x00BD9804 };
        inline util::memory_reference<void*> speed_tree_billboards_node_vftable      { 0x00BD9838 };
        inline util::memory_reference<void*> speed_tree_branches_node_vftable        { 0x00BD9868 };
        inline util::memory_reference<void*> speed_tree_fronds_node_vftable          { 0x00BD9898 };
        inline util::memory_reference<void*> speed_tree_leafs_node_vftable           { 0x00BD98C4 };
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
