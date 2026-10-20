#include "retail.hh"
#include "treyarch/amalga/resource_manager.hh"
#include "treyarch/amalga/resource_types.hh"
#include "treyarch/game/static_colgeom_root.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/resource_key.hh"

using namespace treyarch;

// sub_79C4B0
void static_colgeom_root::construct_mashed_class() {
    self = nullptr;
}

// sub_796D00
void static_colgeom_root::destruct_mashed_class() {}

// sub_7A1850
void static_colgeom_root::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    custom_unmash(mash_info, this, buffer);
}

// sub_785250
void static_colgeom_root::custom_unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    if (!num_objects) {
        tree_root = nullptr;
        my_aabb   = nullptr;

        return;
    }

    // the stored pointers are offsets into the data block
    data   = (u8*)mash_info->read_from_buffer(buffer, (i32)data_size, 128);
    unk_04 = data + (u32)unk_04;

    tree_root = (static_colgeom_tree_root*)mash_info->read_from_buffer(buffer, sizeof(static_colgeom_tree_root), 128);

    tree_root->unk_30 = data + (u32)tree_root->unk_30;
    tree_root->unk_34 = tree_root->unk_30;
    tree_root->unk_38 = data;

    my_aabb = mash_info->read_from_buffer(buffer, 0x38, 128);

    string_hash empty;
    empty.initialize(mash::ALLOCATED);

    if (hull_meshes_file == empty)
        return;

    resource_key key;
    key.set(hull_meshes_file, (u32)amalga::e_resource_type::convex_hull_mesh);

    u8* hull_meshes = amalga::resource_manager::get_resource(key, nullptr, nullptr);

    auto* hulls = (static_colgeom_hull*)(data + hulls_offset);

    for (u32 index = 0; index < hull_count; ++index)
        hulls[index].hull_mesh = hull_meshes + (u32)hulls[index].hull_mesh;
}
