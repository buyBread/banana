#pragma once

#include "treyarch/shared/slot_pool.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch { namespace ai { namespace a_star {
    class node;

    using searchable_t = void*;
    using node_handle  = u32;
    using node_pool_t  = slot_pool<node, node_handle>;
    using node_slot_t  = slot_t<node, node_handle>;

    class node {

    public:
        searchable_t client_data;
        node_handle  my_handle;
        node_handle  parent_handle;
        f32          cost;
        f32          total;
        f32          estimate_to_goal;
        bool         on_open;
    };

    ASSERT_SIZEOF  (node,                     0x1C);
    ASSERT_OFFSETOF(node, client_data,        0x00);
    ASSERT_OFFSETOF(node, my_handle,          0x04);
    ASSERT_OFFSETOF(node, parent_handle,      0x08);
    ASSERT_OFFSETOF(node, cost,               0x0C);
    ASSERT_OFFSETOF(node, total,              0x10);
    ASSERT_OFFSETOF(node, estimate_to_goal,   0x14);
    ASSERT_OFFSETOF(node, on_open,            0x18);
    ASSERT_SIZEOF  (node_slot_t,              0x20);
    ASSERT_SIZEOF  (node_pool_t,              0x50);

    class priority_queue {
    
    public:

        dinkumware::vector<node*> heap;

        void clear() {
            heap.clear();
        }
    };

    class search_record {

        searchable_t   goal;
        node*          best_node;
        priority_queue open_list;
        node_pool_t*   node_pool;
        bool           search_complete;
        bool           goal_found;
        u8             search_mode;
        u8             reserved_023;
        dinkumware::vector<searchable_t>* path_goal_to_start;
        dinkumware::vector<searchable_t>* auxiliary_path;

        static node_pool_t default_node_pool;

    protected:
        virtual bool         assign_astar_node_handle (searchable_t searchable, node_handle new_handle)         = 0;
        virtual node_handle  get_astar_node_handle    (searchable_t searchable)                                 = 0;
        virtual void*        reset_neighbor_iterator  (searchable_t current_location)                           = 0;
        virtual searchable_t get_next_neighbor        (searchable_t current_location, void* iterator)           = 0;
        virtual f32          get_travel_cost          (searchable_t from_location, searchable_t to_location)    = 0;
        virtual f32          get_cost_estimate_to_goal(searchable_t current_location, searchable_t search_goal) = 0;

    public:
        enum { default_node_pool_size = 0x800 };

        search_record() : goal(0),
                          node_pool(0),
                          search_complete(false),
                          goal_found(false),
                          path_goal_to_start(0),
                          auxiliary_path(0) {}

        virtual ~search_record();

        virtual void setup(searchable_t search_start, searchable_t search_goal, dinkumware::vector<searchable_t> *path_goal_to_start, node_pool_t* pool);
        virtual bool search(unsigned int max_iterations_this_call = 0);
    };

    ASSERT_SIZEOF(search_record, 0x2C);
}}} // treyarch::ai::a_star
