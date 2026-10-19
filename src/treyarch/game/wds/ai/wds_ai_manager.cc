#include <algorithm>

#include "treyarch/game/wds/ai/wds_ai_manager.hh"

using namespace treyarch;

// sub_9754C0
bool wds_ai_manager::path_graph_exists(const resource_key &id) {
    if (cached_path_graph &&
        cached_path_graph->id.hash == id.hash &&
        cached_path_graph->id.type == id.type)
        
        return true;

    path_graph** found = std::find_if(path_graph_list.begin(),
                                      path_graph_list.end(),
                                      path_graph_compare_t{ id });

    if (found == path_graph_list.end())
        return false;

    cached_path_graph = *found;

    return true;
}

// inlined @ sub_97A7A0
void wds_ai_manager::add_path_graph(path_graph* graph) {
    if (!path_graph_exists(graph->id))
        path_graph_list.push_back(graph);
}

// sub_9755A0
void wds_ai_manager::remove_path_graph(path_graph* graph) {
    if (cached_path_graph == graph)
        cached_path_graph = nullptr;

    path_graph** found = std::find(path_graph_list.begin(), path_graph_list.end(), graph);

    if (found != path_graph_list.end())
        path_graph_list.erase(found);
}
