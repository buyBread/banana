#pragma once

#include "treyarch/game/wds/ai/path.hh"
#include "treyarch/shared/dinkumware/vector.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch {
    class wds_ai_manager {
    
    private:
        dinkumware::vector
            <ai::path_graph*> path_graph_list;
        ai::path_graph*       cached_path_graph;
    };

    ASSERT_SIZEOF(wds_ai_manager, 0x14);
} // treyarch
