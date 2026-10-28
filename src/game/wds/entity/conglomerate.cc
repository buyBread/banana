#include "game/wds/entity/conglomerate.hh"
#include "game/binary_search.hh"

using namespace treyarch;

// sub_62F880
e_conglom_member_index conglomerate::get_member_index(const string_hash &nodename) const {
    // a bone miss leaves its insertion point for the members search
    i32 found = -1;

    if (bone_indices.size() &&
        binary_search_array(nodename, bone_indices.get_vector().data, (i32)bone_indices.size(), &found))

        return (e_conglom_member_index)(members.size + found);

    if (members.size && binary_search_array_deref(nodename, members.data, (i32)members.size, &found))
        return (e_conglom_member_index)found;

    return nodename == id ? base_conglom_index : invalid_conglom_member;
}

// sub_616950
entity* conglomerate::get_member(e_conglom_member_index index) const {
    if (index >= 0 && (u32)index < members.size)
        return members.data[index];

    return index == base_conglom_index ? (entity*)this : nullptr;
}

// sub_6169D0
bool conglomerate::has_member(const string_hash &nodename) const {
    if (!members.size)
        return false;

    i32 index;

    return binary_search_array_deref(nodename, members.data, (i32)members.size, &index);
}

// sub_40A810
bool conglomerate::has_member_for_linking(const string_hash &nodename) const {
    e_conglom_member_index index = get_member_index(nodename);

    return index >= 0 && (u32)index < bone_indices.size() + members.size;
}
