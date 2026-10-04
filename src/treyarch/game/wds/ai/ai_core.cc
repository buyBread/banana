#include "retail.hh"
#include "treyarch/game/wds/ai/ai_core.hh"

using namespace treyarch;

// inlined @ sub_981D50
info_node* ai_core::get_info_node(u32 index) const {
    if (!my_info_node_list || !((info_node_mask >> index) & 1))
        return nullptr;

    return (*my_info_node_list)[retail::sub_401F50(info_node_mask & ((1u << index) - 1))];
}
