#pragma once

#include "treyarch/game/wds/ai/a_star.hh"
#include "treyarch/game/wds/region.hh"
#include "treyarch/shared/resource_key.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "treyarch/shared/mash/vector_basic.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/math/types/vector3.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ai {
    class nugget; // impl (mash)

    class path;
    class path_graph;

    class path_graph_node {

    public:
        vector3                 pt;
        region*                 my_region;
        path_graph*             my_path_graph;
        nugget*                 node_nugget;
        mash::vector_basic<i16> edge_indices;
        a_star::node_handle     my_node_handle;
        i32                     neighbor_it;
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
    };

    ASSERT_SIZEOF  (path_graph_edge,                   0x14);
    ASSERT_OFFSETOF(path_graph_edge, flags,            0x00);
    ASSERT_OFFSETOF(path_graph_edge, distance,         0x04);
    ASSERT_OFFSETOF(path_graph_edge, weight_modifier,  0x08);
    ASSERT_OFFSETOF(path_graph_edge, node_indices,     0x0C);
    ASSERT_OFFSETOF(path_graph_edge, destination_node, 0x10);

    class path_graph {

    public:
        void*                 vtable;
        resource_key          id;
        mash::vector
            <path_graph_node> nodes;
        mash::vector
            <path_graph_edge> edges;
        path*                 search_path;
    };

    ASSERT_SIZEOF  (path_graph,              0x38);
    ASSERT_OFFSETOF(path_graph, vtable,      0x00);
    ASSERT_OFFSETOF(path_graph, id,          0x04);
    ASSERT_OFFSETOF(path_graph, nodes,       0x0C);
    ASSERT_OFFSETOF(path_graph, edges,       0x20);
    ASSERT_OFFSETOF(path_graph, search_path, 0x34);

    class path : public a_star::search_record {

    protected:
        dinkumware::vector
            <path_graph_node*> path_start_to_goal;
        path_graph*            the_path;
        i32                    waypoint;
        u8                     path_setup_flag;
        u8                     reserved_045[0x03];
        mash::string           reserved_048;
        u8                     reserved_054[0x04];
        a_star::node_pool_t    node_pool_storage;

    protected:
        virtual bool                 assign_astar_node_handle (a_star::searchable_t searchable, a_star::node_handle new_handle);
        virtual a_star::node_handle  get_astar_node_handle    (a_star::searchable_t searchable );
        virtual void*                reset_neighbor_iterator  (a_star::searchable_t current_location);
        virtual a_star::searchable_t get_next_neighbor        (a_star::searchable_t current_location, void* iterator);
        virtual f32                  get_travel_cost          (a_star::searchable_t from_location, a_star::searchable_t to_location);
        virtual f32                  get_cost_estimate_to_goal(a_star::searchable_t current_location, a_star::searchable_t search_goal);
    };

    ASSERT_SIZEOF(path, 0xA8);
}} // treyarch::ai
