#include "treyarch/ngl/debug/debug.hh"
#include "treyarch/ngl/list/render_cursor.hh"

using namespace treyarch;

ngl::render_node* ngl::list::begin_render_nodes(render_node* head) {
    debug_state &debug = ngl::references::debug.get();

    references::previous_render_node.write(nullptr);
    references::current_render_node.write(head);

    render_node* node = head;

    while (node && debug.current_node < debug.test_node_start) {
        node = node->next;
        ++debug.current_node;

        references::current_render_node.write(node);
    }

    if (debug.current_node > debug.test_node_end) {
        node = nullptr;
        references::current_render_node.write(nullptr);
    }

    return node;
}

ngl::render_node* ngl::list::advance_render_node() {
    debug_state &debug = ngl::references::debug.get();
    
    render_node* node = references::current_render_node.read();

    references::previous_render_node.write(node);

    do {
        node = node->next;

        ++debug.current_node;

        references::current_render_node.write(node);
    } while (node && debug.current_node < debug.test_node_start);

    if (debug.current_node > debug.test_node_end) {
        node = nullptr;

        references::current_render_node.write(nullptr);
    }

    return node;
}

void ngl::list::render_nodes(render_node* head) {
    for (render_node* node = begin_render_nodes(head); node; node = advance_render_node())
        node->render();
}
