#pragma once

#include "game/wds/ai/info_node.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/vector.hh"
#include "game/arch_base_vhandle.hh"
#include "game/hash/algo.hh"
#include "game/math/algebra/vector3d.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // what a script asks the next rappel to do; the inode's own params
    inline const string_hash should_fire_rope_key       = string_hash(hash::djb2("should_fire_rope"));
    inline const string_hash desired_attach_pos_key     = string_hash(hash::djb2("desired_attach_pos"));
    inline const string_hash desired_attach_entity_key  = string_hash(hash::djb2("desired_attach_entity"));
    inline const string_hash desired_rope_pivot_key     = string_hash(hash::djb2("desired_rope_pivot"));
    inline const string_hash desired_rope_length_key    = string_hash(hash::djb2("desired_rope_length"));
    inline const string_hash desired_rope_fire_mode_key = string_hash(hash::djb2("desired_rope_fire_mode"));
    inline const string_hash desired_fire_speed_key     = string_hash(hash::djb2("desired_fire_speed"));
    inline const string_hash desired_fire_accel_key     = string_hash(hash::djb2("desired_fire_accel"));
    inline const string_hash desired_fire_curve_rad_key = string_hash(hash::djb2("desired_fire_curve_rad"));

    struct ai_rappel_rope {
        info_node*  owner;
        u8          reserved_04[0x38];
        f32         constraint;        // the owner's distance from the attach point, taken when the line attaches
        u8          reserved_40[0x20];
        bool        attached;
        u8          reserved_61[0x03];
        string_hash rope_pivot;        // the owner's member the rope hangs from; empty for the owner itself
        f32         rope_length;
        u8          reserved_6c[0x04];
        u32         unk_70;            // the fired line's tentacle; nonzero while it's out
        f32         unk_74;
        f32         unk_78;
        f32         unk_7c;
        u8          reserved_80[0x04];

        f32  get_constraint() const;
        void set_rope_length(f32 length);
        void set_rope_pivot(string_hash pivot); // a pivot the owner can't link to is stored as empty
    };

    class ai_rappel_inode : public info_node {

    public:
        ai_rappel_rope*              current_rope;
        mash::vector<ai_rappel_rope> released_ropes;
        f32                          speed;
        u8                           reserved_04c[0x04];

        void set_should_fire_rope(bool fire);
        void set_desired_attach_pos(vector3d pos);
        void set_desired_attach_entity(arch_base_vhandle entity);
        void set_desired_rope_pivot(string_hash pivot);
        void set_desired_rope_length(f32 length);
        void set_desired_rope_fire_mode(bool fire);
        void set_desired_fire_speed(f32 fire_speed);
        void set_desired_fire_accel(f32 accel);
        void set_desired_fire_curve_rad(f32 rad);

        vector3d    get_desired_attach_pos() const;
        string_hash get_desired_rope_pivot() const;

        bool is_engaged() const;
        void release_rope(f32 unk_7c, f32 unk_74); // hands the current rope over to released_ropes
    };

    ASSERT_SIZEOF  (ai_rappel_rope,              0x84);
    ASSERT_OFFSETOF(ai_rappel_rope, constraint,  0x3C);
    ASSERT_OFFSETOF(ai_rappel_rope, attached,    0x60);
    ASSERT_OFFSETOF(ai_rappel_rope, rope_pivot,  0x64);
    ASSERT_OFFSETOF(ai_rappel_rope, rope_length, 0x68);
    ASSERT_OFFSETOF(ai_rappel_rope, unk_70,      0x70);
    ASSERT_OFFSETOF(ai_rappel_rope, unk_7c,      0x7C);

    ASSERT_SIZEOF  (ai_rappel_inode,                 0x50);
    ASSERT_OFFSETOF(ai_rappel_inode, current_rope,   0x30);
    ASSERT_OFFSETOF(ai_rappel_inode, released_ropes, 0x34);
    ASSERT_OFFSETOF(ai_rappel_inode, speed,          0x48);
} // treyarch
