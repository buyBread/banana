#pragma once

#include "treyarch/ngl/list/render_node.hh"

namespace treyarch { namespace ngl { namespace list {
    void sort_render_list_by_hash(render_node* &head, u32 count);
    void sort_render_list_by_distance(render_node* &head, u32 count);
}}} // treyarch::ngl::list
