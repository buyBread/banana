#include "treyarch/ngl/list/arena.hh"
#include "treyarch/ngl/list/render_callback.hh"
#include "treyarch/ngl/scene/matrices.hh"
#include "treyarch/ngl/scene/references.hh"

using namespace treyarch;

// sub_9D8C70
void ngl::render_callback::render(ngl::render_callback::node* value) {
    value->callback(value->data);
}

// sub_9D9350
ngl::scene* ngl::render_callback::list_add_custom_node(function callback, void* data, const sort_info* sorting) {
    validate_matrices(ngl::references::current_scene.read());

    auto* value = (node*)list::allocate(sizeof(node), 16);

    if (!value)
        return nullptr;

    value->base.vtable = &references::node_vtable.get();
    value->sorting     = *sorting;
    value->type        = 1;
    value->callback    = callback;
    value->data        = data;

    return list_add_node(&value->base);
}
