#pragma once

#include "treyarch/ngl/list/render_node.hh"
#include "util/memory_reference.hh"

namespace treyarch { namespace ngl { namespace list {
    render_node* begin_render_nodes(render_node* head);
    render_node* advance_render_node();
    
    void render_nodes(render_node* head);

    namespace references {
        inline util::memory_reference<render_node*> current_render_node  { 0x01115C5C };
        inline util::memory_reference<render_node*> previous_render_node { 0x01115C60 };
    } // references
}}} // treyarch::ngl::list
