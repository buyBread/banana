#pragma once

#include "game/wds/entity/actor.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mash/vector_basic_ref_counted.hh"
#include "game/math/po.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_tentacle_info;

    // members first, then the bones past them
    enum e_conglom_member_index : i16 {
        invalid_conglom_member = -2,
        base_conglom_index     = -1
    };

    struct conglomerate_bone_index {
        string_hash name;
        i32         model_po_index;
        u8          reserved_08[0x0C];

        friend bool operator<(string_hash lhs, const conglomerate_bone_index &rhs) { return lhs < rhs.name; }
        friend bool operator>(string_hash lhs, const conglomerate_bone_index &rhs) { return rhs.name < lhs; }
    };

    // members are kept sorted by id
    inline bool operator<(string_hash lhs, const entity &rhs) { return lhs < rhs.id; }
    inline bool operator>(string_hash lhs, const entity &rhs) { return rhs.id < lhs; }

    class conglomerate : public actor {

    public:
        u8                            reserved_0d8[0x10];
        mash::vector<entity>          members;
        mash::vector
            <ai_tentacle_info>*       tentacles;
        u8                            reserved_100[0x24];
        mash::vector_basic<po>        all_model_po; // als::biped_physics_mocomp saves and restores them
        u8                            reserved_134[0x1C];
        mash::vector_basic_ref_counted
            <conglomerate_bone_index> bone_indices;
        u8                            reserved_154[0x20];
        string_hash                   skeleton_name_hash;
        u8                            reserved_178[0x20];

        e_conglom_member_index get_member_index(const string_hash &nodename) const;
        entity*                get_member      (e_conglom_member_index index) const;

        bool has_member            (const string_hash &nodename) const;
        bool has_member_for_linking(const string_hash &nodename) const;
    };

    ASSERT_SIZEOF  (conglomerate_bone_index,                 0x14);
    ASSERT_OFFSETOF(conglomerate_bone_index, model_po_index, 0x04);

    ASSERT_SIZEOF  (conglomerate,                     0x198);
    ASSERT_OFFSETOF(conglomerate, members,            0x0E8);
    ASSERT_OFFSETOF(conglomerate, tentacles,          0x0FC);
    ASSERT_OFFSETOF(conglomerate, all_model_po,       0x124);
    ASSERT_OFFSETOF(conglomerate, bone_indices,       0x150);
    ASSERT_OFFSETOF(conglomerate, skeleton_name_hash, 0x174);
} // treyarch
