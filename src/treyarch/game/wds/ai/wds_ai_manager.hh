#pragma once

#include "vc8/vector.hh"
#include "treyarch/game/wds/ai/path.hh"
#include "treyarch/shared/resource_key.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    struct path_graph_compare_t {
        resource_key id;

        bool operator()(const path_graph* graph) const {
            return graph->id.hash == id.hash && graph->id.type == id.type;
        }
    };

    class wds_ai_manager {

    private:
        vc8::vector
            <path_graph*> path_graph_list;
        path_graph*       cached_path_graph;

    public:
        bool path_graph_exists(const resource_key &id);
        void add_path_graph(path_graph* graph);
        void remove_path_graph(path_graph* graph);
    };

    ASSERT_SIZEOF(wds_ai_manager, 0x14);
} // treyarch
