#pragma once

#include "treyarch/game/quest_descriptions.hh"
#include "treyarch/game/reference.hh"
#include "treyarch/shared/dinkumware/map.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class static_spawn_item;
    struct spawn_point_list;

    // the base of every flag collection; mashed ones get their retail vtable from the type registry
    class flag_collection_base : public mash::mash_virtual_base {

    public:
        u8 unk_04[0x0C];
    };

    struct spawn_table_unk_68_entry {
        mash::string unk_00;
        mash::string unk_0c;
        mash::string unk_18;

        // inlined @ sub_937950
        void construct_mashed_class() {
            unk_00.construct_mashed_class();
            unk_0c.construct_mashed_class();
            unk_18.construct_mashed_class();
        }

        // inlined @ sub_928DB0
        void destruct_mashed_class() {
            unk_00.destruct_mashed_class();
            unk_0c.destruct_mashed_class();
            unk_18.destruct_mashed_class();
        }

        // inlined @ sub_937B50
        void unmash(mash::mash_info_struct* mash_info, void*, mash::buffer_type buffer) {
            unk_00.unmash(mash_info, this, buffer);
            unk_0c.unmash(mash_info, this, buffer);
            unk_18.unmash(mash_info, this, buffer);
        }
    };

    struct spawn_table_unk_68 {
        mash::string                   name;
        mash::string                   expression;
        mash::vector
            <spawn_table_unk_68_entry> entries;
        quest_condition*               condition;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    struct spawn_table_unk_7c {
        mash::string                     expression;
        mash::vector
            <mash::vector<mash::string>> spawn_items; // spawn item names, one list per level
        quest_condition*                 condition;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class spawn_table_interface {

    public:
        void* self; // the resource handler points this at the interface itself

        mash::vector
            <reference<spawn_item_collection>>        spawn_item_collections;
        mash::vector
            <reference<spawn_point_list_collection>>  spawn_point_list_collections;
        mash::vector
            <reference<static_spawn_item_collection>> static_spawn_item_collections;
        mash::vector
            <flag_collection_base>                    flag_collections;
        mash::vector
            <mash::string>                            unk_54;
        mash::vector
            <spawn_table_unk_68>                      unk_68;
        mash::vector
            <spawn_table_unk_7c>                      unk_7c; // one per act

        // the spawn table manager builds these once the interface is loaded
        dinkumware::map
            <mash::string, static_spawn_item*>*       spawn_items_by_name;
        dinkumware::map
            <mash::string, spawn_point_list*>*        spawn_point_lists_by_name;
        dinkumware::map
            <mash::string, flag_collection_base*>*    flag_collections_by_name;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);

        void finalize(mash::allocation_scope scope);
    };

    ASSERT_SIZEOF  (flag_collection_base, 0x10);

    ASSERT_SIZEOF  (spawn_table_unk_68_entry, 0x24);

    ASSERT_SIZEOF  (spawn_table_unk_68,             0x30);
    ASSERT_OFFSETOF(spawn_table_unk_68, entries,    0x18);
    ASSERT_OFFSETOF(spawn_table_unk_68, condition,  0x2C);

    ASSERT_SIZEOF  (spawn_table_unk_7c,              0x24);
    ASSERT_OFFSETOF(spawn_table_unk_7c, spawn_items, 0x0C);
    ASSERT_OFFSETOF(spawn_table_unk_7c, condition,   0x20);

    ASSERT_SIZEOF  (spawn_table_interface,                                0x9C);
    ASSERT_OFFSETOF(spawn_table_interface, spawn_point_list_collections,  0x18);
    ASSERT_OFFSETOF(spawn_table_interface, static_spawn_item_collections, 0x2C);
    ASSERT_OFFSETOF(spawn_table_interface, flag_collections,              0x40);
    ASSERT_OFFSETOF(spawn_table_interface, unk_54,                        0x54);
    ASSERT_OFFSETOF(spawn_table_interface, unk_68,                        0x68);
    ASSERT_OFFSETOF(spawn_table_interface, unk_7c,                        0x7C);
    ASSERT_OFFSETOF(spawn_table_interface, spawn_items_by_name,           0x90);
    ASSERT_OFFSETOF(spawn_table_interface, spawn_point_lists_by_name,     0x94);
    ASSERT_OFFSETOF(spawn_table_interface, flag_collections_by_name,      0x98);
} // treyarch
