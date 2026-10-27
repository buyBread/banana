#pragma once

#include "game/amalga/mashable_resource_handler.hh"
#include "game/wds/ai/ai_pf_region_graph.hh"

namespace treyarch { namespace amalga {
    using ai_region_paths_resource_handler = mashable_resource_handler<ai_pf_region_graph>;
}} // treyarch::amalga
