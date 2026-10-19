#include "treyarch/game/wds/ai/path.hh"
#include "treyarch/game/wds/references.hh"

using namespace treyarch;

// inlined @ sub_979970
void path_graph_node::construct_mashed_class() {
    edge_indices.construct_mashed_class();

    if (node_nugget) {
        mash::mash_info_struct::construct_class(node_nugget);

        if (node_nugget)
            node_nugget->set_user_data(this);
    }
}

// inlined @ sub_976150
void path_graph_node::destruct_mashed_class() {
    clear();

    edge_indices.destruct_mashed_class();

    if (node_nugget) {
        node_nugget->destruct_mashed_class();
        node_nugget = nullptr;
    }
}

// inlined @ sub_978890
void path_graph_node::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    edge_indices.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(node_nugget, this, buffer);
    else
        node_nugget = nullptr;
}

// sub_95DBF0
void path_graph_node::clear() {
    pt            = references::zerovec.get();
    my_region     = nullptr;
    my_path_graph = nullptr;

    edge_indices.clear();

    my_node_handle = 0;
    neighbor_it    = -1;

    if (node_nugget) {
        if (node_nugget->is_from_mash()) {
            node_nugget->destruct_mashed_class();
            node_nugget = nullptr;

            return;
        }

        delete node_nugget;
    }

    node_nugget = nullptr;
}

// sub_97AA70
void path_graph::construct_mashed_class() {
    self = nullptr;

    id   .construct_mashed_class();
    nodes.construct_mashed_class();
    edges.construct_mashed_class();

    initialize(mash::FROM_MASH);
}

// sub_979A40
void path_graph::destruct_mashed_class() {
    finalize(mash::FROM_MASH);

    id   .destruct_mashed_class();
    nodes.destruct_mashed_class();
    edges.destruct_mashed_class();
}

// sub_979D40
void path_graph::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    id   .unmash(mash_info, this, buffer);
    nodes.unmash(mash_info, this, buffer);
    edges.unmash(mash_info, this, buffer);
}

// sub_97A7A0
void path_graph::initialize(mash::allocation_scope scope) {
    if (scope == mash::ALLOCATED) {
        edges.clear();
        nodes.clear();

        search_path = nullptr;

        return;
    }

    for (path_graph_node* node : nodes)
        node->my_path_graph = this;

    references::g_world_ptr.read()->ai_mgr.add_path_graph(this);

    search_path = nullptr;
}

// inlined @ sub_979A40
void path_graph::finalize(mash::allocation_scope) {
    references::g_world_ptr.read()->ai_mgr.remove_path_graph(this);

    edges.clear();
    nodes.clear();
}
