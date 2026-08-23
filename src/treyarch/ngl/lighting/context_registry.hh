#pragma once

#include "treyarch/ngl/lighting/context.hh"
#include "treyarch/shared/container/legacy_list.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace ngl { namespace lighting {
    using context_registry_node = container::legacy_list_node<light_context*>;
    using context_registry      = container::legacy_list<light_context*>;

    void register_context(light_context* context);
    void reset_context_registry();

    ASSERT_SIZEOF(context_registry_node, 0x0C);
    ASSERT_SIZEOF(context_registry,      0x0C);
}}} // treyarch::ngl::lighting
