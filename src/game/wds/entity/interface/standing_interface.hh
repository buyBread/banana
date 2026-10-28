#pragma once

#include "game/wds/entity/interface/generic_interface.hh"
#include "game/arch_base_vhandle.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    // SM3's standing_interface flags
    enum e_standing_interface_flags : u32 {
        standing_interface_flag_enabled                        = 0x00000001,
        standing_interface_flag_effectively_standing           = 0x00000002,
        standing_interface_flag_was_effectively_standing       = 0x00000004,
        standing_interface_flag_adv_standing                   = 0x00000008,
        standing_interface_flag_floor_offset_uses_render_scale = 0x00000010,
        standing_interface_flag_always_standing                = 0x00000020,
        standing_interface_flag_adv_crawling                   = 0x00000040,
        standing_interface_flag_crazy_crawl_ik                 = 0x00000080,
        standing_interface_flag_prev_stand_pos_valid           = 0x00000100,
        standing_interface_flag_swimming                       = 0x00000200,
        standing_interface_flag_wall_sprinting                 = 0x00000400,
        standing_interface_flag_combat_disabled                = 0x00000800
    };

    class standing_interface : public actor_interface {

    public:
        u8                reserved_00c[0x24];
        f32               floor_offset_scale;
        f32               floor_offset_override; // preferred by get_floor_offset when positive
        f32               last_elevation;
        f32               cur_elevation;
        f32               last_floor_offset;
        u32               flags;
        f32               unk_048;
        arch_base_vhandle control_parent;
        u8                reserved_050[0x28];

        // inlined
        bool is_enabled() const {
            return (flags & standing_interface_flag_enabled) != 0;
        }

        // inlined
        bool is_effectively_standing() const {
            return (flags & standing_interface_flag_effectively_standing) != 0;
        }

        // inlined
        void enable(bool i) {
            if (i)
                flags |= standing_interface_flag_enabled;
            else
                flags &= ~standing_interface_flag_enabled;
        }

        // sub_4C2D00
        void set_swimming(bool i) {
            if (i)
                flags |= standing_interface_flag_swimming;
            else
                flags &= ~standing_interface_flag_swimming;
        }

        f32  get_pure_anim_floor_offset() const;
        void set_floor_offset_override(f32 new_override);

        void manage_standing(f32 delta_t, bool force, f32 max_force_delta);
    };

    ASSERT_OFFSETOF(standing_interface, floor_offset_scale, 0x30);
    ASSERT_OFFSETOF(standing_interface, cur_elevation,      0x3C);
    ASSERT_OFFSETOF(standing_interface, flags,              0x44);
    ASSERT_OFFSETOF(standing_interface, control_parent,     0x4C);
    ASSERT_SIZEOF(standing_interface, 0x78);
} // treyarch
