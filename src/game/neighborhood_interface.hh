#pragma once

#include "game/quest_descriptions.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/virtual_base.hh"
#include "game/math/algebra.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // the base of every neighborhood definition; mashed ones get their retail vtable from the type registry
    class neighborhood_definition : public mash::mash_virtual_base {

    public:
        mash::string name;
        u32          unk_10;
        vector3d     center; // what the city-life actives call the neighborhood center
    };

    // the spawn table manager keeps the last one whose condition holds
    struct neighborhood_interface_unk_04 {
        mash::string                  expression;
        u32                           unk_0c;
        u32                           unk_10;
        u32                           unk_14;
        u32                           unk_18;
        mash::string                  city_wide_set_name; // names the city-wide spawn item collection
        mash::vector
            <neighborhood_definition> meta_neighborhoods;
        mash::vector
            <neighborhood_definition> neighborhoods;
        quest_condition*              condition;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    class neighborhood_interface {

    public:
        void* self; // the resource handler points this at the interface itself

        mash::vector
            <neighborhood_interface_unk_04> unk_04;

        void construct_mashed_class();
        void destruct_mashed_class();
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer);
    };

    ASSERT_SIZEOF  (neighborhood_definition,         0x20);
    ASSERT_OFFSETOF(neighborhood_definition, name,   0x04);
    ASSERT_OFFSETOF(neighborhood_definition, center, 0x14);

    ASSERT_SIZEOF  (neighborhood_interface_unk_04,                     0x54);
    ASSERT_OFFSETOF(neighborhood_interface_unk_04, unk_0c,             0x0C);
    ASSERT_OFFSETOF(neighborhood_interface_unk_04, city_wide_set_name, 0x1C);
    ASSERT_OFFSETOF(neighborhood_interface_unk_04, meta_neighborhoods, 0x28);
    ASSERT_OFFSETOF(neighborhood_interface_unk_04, neighborhoods,      0x3C);
    ASSERT_OFFSETOF(neighborhood_interface_unk_04, condition,          0x50);

    ASSERT_SIZEOF  (neighborhood_interface,         0x18);
    ASSERT_OFFSETOF(neighborhood_interface, unk_04, 0x04);
} // treyarch
