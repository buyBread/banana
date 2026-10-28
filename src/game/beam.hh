#pragma once

#include "game/boolx.hh"
#include "game/color.hh"
#include "game/math/algebra/vector2d.hh"
#include "game/math/algebra/vector3d.hh"
#include "game/wds/entity/entity.hh"
#include "treyarch/mash/string.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    namespace ngl { namespace shaders { namespace pcuv {
        struct pcuv_material;
    }}} // ngl::shaders::pcuv

    class beam : public entity {

    public:
        enum e_flags : u32 {
            last_frame_valid       = 0x001,
            hit_world              = 0x002,
            hit_hero               = 0x004,
            no_clip_to_hero        = 0x008,
            no_collide_world       = 0x010,
            no_collide_hero        = 0x020,
            no_clip_to_beamable    = 0x040,
            no_clip_to_world       = 0x080,
            detects_stealth        = 0x100,
            dynamic_collision      = 0x200,
            world_entity_collision = 0x400,

            no_clipping = no_clip_to_hero | no_clip_to_beamable | no_clip_to_world
        };

        u8                                  reserved_074[0x2C];
        beam*                               next_beam;
        beam*                               prev_beam;
        f32                                 thickness;
        f32                                 max_length;
        f32                                 start_dist;      // kept within 0 and max_length
        u8                                  reserved_0b4[0x04];
        color32                             my_start_color;
        color32                             my_end_color;
        u32                                 beam_flags;
        vector3d                            static_endpoint;
        f32                                 static_len;
        f32                                 curr_len;
        vector3d                            impact_point;
        vector3d                            impact_normal;
        vector2d                            uv_anim;
        vector2d                            uv_coords[2];
        f32                                 tiles_per_meter;
        boolx                               additive;
        boolx                               using_uv_anim;
        u8                                  pad_10e[0x02];
        ngl::shaders::pcuv::pcuv_material*  my_material;     // made on the first set_material

        void set_flag(e_flags flag) {
            beam_flags |= flag;
        }

        void clear_flag(e_flags flag) {
            beam_flags &= ~flag;
        }

        bool is_flagged(e_flags flag) const {
            return (beam_flags & flag) != 0;
        }

        void set_uv_anim(const vector2d &anim) { // any real movement turns the scrolling on
            uv_anim       = anim;
            using_uv_anim = uv_anim.length2() > 0.001f;
        }

        void set_thickness(f32 value);
        void set_max_length(f32 length);
        void set_beam_color(const color32 &value);
        void set_point_to_point(const vector3d &from, const vector3d &to);
        void set_material(const mash::string &name);
    };

    ASSERT_SIZEOF  (beam,                  0x114);
    ASSERT_OFFSETOF(beam, next_beam,       0x0A0);
    ASSERT_OFFSETOF(beam, my_start_color,  0x0B8);
    ASSERT_OFFSETOF(beam, beam_flags,      0x0C0);
    ASSERT_OFFSETOF(beam, impact_point,    0x0D8);
    ASSERT_OFFSETOF(beam, uv_anim,         0x0F0);
    ASSERT_OFFSETOF(beam, tiles_per_meter, 0x108);
    ASSERT_OFFSETOF(beam, additive,        0x10C);
    ASSERT_OFFSETOF(beam, my_material,     0x110);
} // treyarch
