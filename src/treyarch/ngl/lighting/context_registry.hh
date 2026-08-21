#pragma once

#include "treyarch/ngl/lighting/context.hh"
#include "util/macros/sanity_assert.hh"

namespace treyarch { namespace ngl { namespace lighting {
    struct context_registry_node {
        context_registry_node* next;
        context_registry_node* previous;
        light_context*         value;
    };

    struct context_registry {
        u32                    allocator_state;
        context_registry_node* head;
        u32                    size;
    };

    void register_context(light_context* context);
    void reset_context_registry();

    ASSERT_SIZEOF  (context_registry_node,           0x0C);
    ASSERT_OFFSETOF(context_registry_node, next,     0x00);
    ASSERT_OFFSETOF(context_registry_node, previous, 0x04);
    ASSERT_OFFSETOF(context_registry_node, value,    0x08);

    ASSERT_SIZEOF  (context_registry,                  0x0C);
    ASSERT_OFFSETOF(context_registry, allocator_state, 0x00);
    ASSERT_OFFSETOF(context_registry, head,            0x04);
    ASSERT_OFFSETOF(context_registry, size,            0x08);
}}} // treyarch::ngl::lighting
