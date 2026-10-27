#include <cstdio>
#include <new>

#include "treyarch/ngl/list/list.hh"
#include "treyarch/ngl/list/render_callback.hh"
#include "treyarch/ngl/scene/scene.hh"

using namespace treyarch;

// sub_9D8C70
void ngl::render_callback_node::render() {
    fn(data);
}

// sub_9D8C80
void ngl::render_callback_node::get_desc(char* desc) {
    std::sprintf(desc, "CustomNode\t");
}

// sub_9D8CA0
void ngl::render_callback_node::get_sort_info(ngl::sort_info &info) {
    info = sort_info;
}

// sub_9D9350
ngl::scene* ngl::list_add_custom_node(custom_node_fn fn, void* data, const ngl::sort_info* sort_info) {
    validate_matrices(references::build_scene.read());

    auto* value = new (list_alloc(sizeof(render_callback_node), 16)) render_callback_node;

    if (!value)
        return nullptr;

    value->sort_info = *sort_info;
    value->type      = node_custom;
    value->fn        = fn;
    value->data      = data;

    return list_add_node(value);
}
