#include "game/spawn_table_interface.hh"
#include "treyarch/mash/mash_info.hh"
#include "treyarch/mm/heap.hh"

using namespace treyarch;

// inlined @ sub_94CC20
void spawn_table_unk_68::construct_mashed_class() {
    name      .construct_mashed_class();
    expression.construct_mashed_class();
    entries   .construct_mashed_class();

    if (condition)
        mash::mash_info_struct::construct_class(condition);
}

// inlined @ sub_93F2F0
void spawn_table_unk_68::destruct_mashed_class() {
    name      .destruct_mashed_class();
    expression.destruct_mashed_class();
    entries   .destruct_mashed_class();

    if (condition) {
        condition->destruct_mashed_class();
        condition = nullptr;
    }
}

// sub_940120
void spawn_table_unk_68::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    name      .unmash(mash_info, this, buffer);
    expression.unmash(mash_info, this, buffer);
    entries   .unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// inlined @ sub_9565B0
void spawn_table_unk_7c::construct_mashed_class() {
    expression .construct_mashed_class();
    spawn_items.construct_mashed_class();

    if (condition)
        mash::mash_info_struct::construct_class(condition);
}

// inlined @ sub_954290
void spawn_table_unk_7c::destruct_mashed_class() {
    expression .destruct_mashed_class();
    spawn_items.destruct_mashed_class();

    if (condition) {
        condition->destruct_mashed_class();
        condition = nullptr;
    }
}

// sub_953D80
void spawn_table_unk_7c::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    expression .unmash(mash_info, this, buffer);
    spawn_items.unmash(mash_info, this, buffer);

    u8 class_mashed;
    mash_info->read_from_buffer(mash::SHARED_BUFFER, class_mashed);

    if (class_mashed == mash::member_class_mashed)
        mash_info->unmash_class(condition, this, buffer);
    else
        condition = nullptr;
}

// sub_956CC0
void spawn_table_interface::construct_mashed_class() {
    self = nullptr;

    spawn_item_collections       .construct_mashed_class();
    spawn_point_list_collections .construct_mashed_class();
    static_spawn_item_collections.construct_mashed_class();
    flag_collections             .construct_mashed_class();
    unk_54                       .construct_mashed_class();
    unk_68                       .construct_mashed_class();
    unk_7c                       .construct_mashed_class();

    spawn_items_by_name       = nullptr;
    spawn_point_lists_by_name = nullptr;
    flag_collections_by_name  = nullptr;
}

// sub_956690
void spawn_table_interface::destruct_mashed_class() {
    finalize(mash::FROM_MASH);

    spawn_item_collections       .destruct_mashed_class();
    spawn_point_list_collections .destruct_mashed_class();
    static_spawn_item_collections.destruct_mashed_class();
    flag_collections             .destruct_mashed_class();
    unk_54                       .destruct_mashed_class();
    unk_68                       .destruct_mashed_class();
    unk_7c                       .destruct_mashed_class();
}

// sub_956DE0
void spawn_table_interface::unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
    spawn_item_collections       .unmash(mash_info, this, buffer);
    spawn_point_list_collections .unmash(mash_info, this, buffer);
    static_spawn_item_collections.unmash(mash_info, this, buffer);
    flag_collections             .unmash(mash_info, this, buffer);
    unk_54                       .unmash(mash_info, this, buffer);
    unk_68                       .unmash(mash_info, this, buffer);
    unk_7c                       .unmash(mash_info, this, buffer);
}

// sub_953C60
void spawn_table_interface::finalize(mash::allocation_scope) {
    if (spawn_items_by_name) {
        spawn_items_by_name->~map();

        memory::heap::free(spawn_items_by_name);
    }

    if (spawn_point_lists_by_name) {
        spawn_point_lists_by_name->~map();

        memory::heap::free(spawn_point_lists_by_name);
    }

    if (flag_collections_by_name) {
        flag_collections_by_name->~map();

        memory::heap::free(flag_collections_by_name);
    }
}
