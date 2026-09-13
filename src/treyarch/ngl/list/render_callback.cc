#include "treyarch/ngl/list/render_callback.hh"

using namespace treyarch;

void ngl::render_callback::render(ngl::render_callback::node* value) {
    value->callback(value->data);
}
