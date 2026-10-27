#pragma once

#include "vc8/vector.hh"
#include "game/wds/ai/astar.hh"
#include "game/wds/ai/nugget.hh"
#include "game/wds/region.hh"
#include "game/resource_key.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class path;
    class path_graph;

    class path_graph_node {

    public:
        vector3d                pt;
        region*                 my_region;
        path_graph*             my_path_graph;
        nugget*                 node_nugget;
        mash::vector_basic<i16> edge_indices;
        astar_node_handle       my_node_handle;
        i32                     neighbor_it;

        // sub_971900
        ~path_graph_node() {
            clear();
        }

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
        void clear();
    };

    ASSERT_SIZEOF  (path_graph_node,                 0x30);
    ASSERT_OFFSETOF(path_graph_node, pt,             0x00);
    ASSERT_OFFSETOF(path_graph_node, my_region,      0x0C);
    ASSERT_OFFSETOF(path_graph_node, my_path_graph,  0x10);
    ASSERT_OFFSETOF(path_graph_node, node_nugget,    0x14);
    ASSERT_OFFSETOF(path_graph_node, edge_indices,   0x18);
    ASSERT_OFFSETOF(path_graph_node, my_node_handle, 0x28);
    ASSERT_OFFSETOF(path_graph_node, neighbor_it,    0x2C);

    class path_graph_edge {

    public:
        u32 flags;
        f32 distance;
        f32 weight_modifier;
        i16 node_indices[2];
        i16 destination_node;
        u8  reserved_012[0x02];

        ~path_graph_edge() {
            clear();
        }

        void construct_mashed_class() {}

        void destruct_mashed_class() {
            clear();
        }

        void unmash(mash::mash_info_struct*, void*, mash::buffer_type) {}

        // inlined @ sub_9741C0
        void clear() {
            distance         = 0.0f;
            node_indices[1]  = -1;
            node_indices[0]  = -1;
            flags            = 0;
            weight_modifier  = 1.0f;
            destination_node = -1;
        }
    };

    ASSERT_SIZEOF  (path_graph_edge,                   0x14);
    ASSERT_OFFSETOF(path_graph_edge, flags,            0x00);
    ASSERT_OFFSETOF(path_graph_edge, distance,         0x04);
    ASSERT_OFFSETOF(path_graph_edge, weight_modifier,  0x08);
    ASSERT_OFFSETOF(path_graph_edge, node_indices,     0x0C);
    ASSERT_OFFSETOF(path_graph_edge, destination_node, 0x10);

    class path_graph {

    public:
        void*                 self; // the resource handler points this at the graph itself
        resource_key          id;
        mash::vector
            <path_graph_node> nodes;
        mash::vector
            <path_graph_edge> edges;
        path*                 search_path;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);

    private:
        void initialize(mash::allocation_scope scope);
        void finalize(mash::allocation_scope scope);
    };

    ASSERT_SIZEOF  (path_graph,              0x38);
    ASSERT_OFFSETOF(path_graph, self,        0x00);
    ASSERT_OFFSETOF(path_graph, id,          0x04);
    ASSERT_OFFSETOF(path_graph, nodes,       0x0C);
    ASSERT_OFFSETOF(path_graph, edges,       0x20);
    ASSERT_OFFSETOF(path_graph, search_path, 0x34);

    class path : public astar_search_record {

    protected:
        vc8::vector
            <path_graph_node*> path_start_to_goal;
        path_graph*            the_path;
        i32                    waypoint;
        u8                     path_setup_flag;
        u8                     reserved_045[0x03];
        mash::string           reserved_048;
        u8                     reserved_054[0x04];
        astar_node_pool        node_pool_storage;

    protected:
        virtual bool               assign_astar_node_handle (astar_searchable_t searchable, astar_node_handle new_handle);
        virtual astar_node_handle  get_astar_node_handle    (astar_searchable_t searchable);
        virtual void*              reset_neighbor_iterator  (astar_searchable_t current_location);
        virtual astar_searchable_t get_next_neighbor        (astar_searchable_t current_location, void* iterator);
        virtual f32                get_travel_cost          (astar_searchable_t from_location, astar_searchable_t to_location);
        virtual f32                get_cost_estimate_to_goal(astar_searchable_t current_location, astar_searchable_t search_goal);
    };

    namespace references {
        inline util::memory_reference<vector3d> zerovec { 0x010FC610 }; // SM3's ZEROVEC
    } // references

    ASSERT_SIZEOF(path, 0xA8);
} // treyarch
