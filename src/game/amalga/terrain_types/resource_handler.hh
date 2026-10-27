#pragma once

#include "game/amalga/mashable_resource_handler.hh"
#include "game/amalga/terrain_types/terrain_types_set.hh"

namespace treyarch { namespace amalga {
    using terrain_types_resource_handler = mashable_resource_handler<terrain_types_set>;
}} // treyarch::amalga
