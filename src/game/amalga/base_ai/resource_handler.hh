#pragma once

#include "game/amalga/mashable_resource_handler.hh"
#include "game/wds/ai/core_ai_resource.hh"

namespace treyarch { namespace amalga {
    using base_ai_resource_handler = mashable_resource_handler<core_ai_resource>;
}} // treyarch::amalga
