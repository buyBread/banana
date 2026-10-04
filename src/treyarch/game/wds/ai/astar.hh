#pragma once

#include "treyarch/shared/slot_pool.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class astar_node;

    using astar_searchable_t = void*;
    using astar_node_handle  = u32;
    using astar_node_pool    = slot_pool<astar_node, astar_node_handle>;
    using astar_node_slot    = slot_t<astar_node, astar_node_handle>;

    class astar_node {

    public:
        astar_searchable_t client_data;
        astar_node_handle  my_handle;
        astar_node_handle  parent_handle;
        f32                cost;
        f32                total;
        f32                estimate_to_goal;
        bool               on_open;
    };

    ASSERT_SIZEOF  (astar_node,                   0x1C);
    ASSERT_OFFSETOF(astar_node, client_data,      0x00);
    ASSERT_OFFSETOF(astar_node, my_handle,        0x04);
    ASSERT_OFFSETOF(astar_node, parent_handle,    0x08);
    ASSERT_OFFSETOF(astar_node, cost,             0x0C);
    ASSERT_OFFSETOF(astar_node, total,            0x10);
    ASSERT_OFFSETOF(astar_node, estimate_to_goal, 0x14);
    ASSERT_OFFSETOF(astar_node, on_open,          0x18);

    class astar_priority_queue {

    public:
        dinkumware::vector<astar_node*> heap;

        void clear() {
            heap.clear();
        }
    };

    class astar_search_record {

        astar_searchable_t                      goal;
        astar_node*                             best_node;
        astar_priority_queue                    open_list;
        astar_node_pool*                        node_pool;
        bool                                    search_complete;
        bool                                    goal_found;
        u8                                      search_mode;
        u8                                      reserved_023;
        dinkumware::vector<astar_searchable_t>* path_goal_to_start;
        dinkumware::vector<astar_searchable_t>* auxiliary_path;

        static astar_node_pool default_node_pool;

    protected:
        virtual bool               assign_astar_node_handle (astar_searchable_t searchable, astar_node_handle new_handle)         = 0;
        virtual astar_node_handle  get_astar_node_handle    (astar_searchable_t searchable)                                       = 0;
        virtual void*              reset_neighbor_iterator  (astar_searchable_t current_location)                                 = 0;
        virtual astar_searchable_t get_next_neighbor        (astar_searchable_t current_location, void* iterator)                 = 0;
        virtual f32                get_travel_cost          (astar_searchable_t from_location, astar_searchable_t to_location)    = 0;
        virtual f32                get_cost_estimate_to_goal(astar_searchable_t current_location, astar_searchable_t search_goal) = 0;

    public:
        enum { astar_default_node_pool_size = 0x800 };

        astar_search_record() : goal(0),
                                node_pool(0),
                                search_complete(false),
                                goal_found(false),
                                path_goal_to_start(0),
                                auxiliary_path(0) {}

        virtual ~astar_search_record();

        virtual void setup(astar_searchable_t search_start, astar_searchable_t search_goal, dinkumware::vector<astar_searchable_t> *path_goal_to_start, astar_node_pool* pool);
        virtual bool search(unsigned int max_iterations_this_call = 0);
    };

    ASSERT_SIZEOF(astar_search_record, 0x2C);
} // treyarch
