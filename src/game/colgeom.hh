#pragma once

#include "game/boolx.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class actor;
    class po;

    // SM3's find_int_flags_t, the ones read so far
    enum e_find_int_flags : u32 {
        fi_collide_entity          = 0x00000002,
        fi_collide_camera          = 0x00000010,
        fi_collide_camera_see_thru = 0x00000100,
        fi_collide_characters      = 0x00080000
    };

    // SM3's collision_geometry types, plus cg_convex_hull's
    enum e_collision_geometry_type : u32 {
        collision_geometry_type_none,
        collision_geometry_type_capsule,
        collision_geometry_type_mesh,
        collision_geometry_type_convex_hull
    };

    class collision_geometry : public mash::mash_virtual_base {

    public:
        actor* owner;
        boolx  valid;
        boolx  dynamic;
        u8     pad_00a[0x02];
        u32    flags;        // e_find_int_flags

        virtual collision_geometry* make_instance(actor* owner) const = 0;
        virtual void                unk_010() = 0;
        virtual void                unk_011() = 0;
        virtual void                xform(const po &the_po) = 0;
        virtual f32                 get_colgeom_radius() const = 0;
        virtual vector3d            get_local_space_bounding_sphere_center() const = 0;
        virtual f32                 get_bounding_sphere_radius() const = 0;
        virtual f32                 get_core_radius() const = 0;
        virtual const vector3d     &get_abs_position() const = 0;
        virtual const vector3d     &get_pivot() const = 0;
        virtual u32                 get_type() const = 0;
        virtual bool                is_pivot_valid() const = 0;
        virtual bool                is_dynamic() const = 0;
        virtual void                unk_022() = 0;
    };

    // SM3's capsule
    class capsule {

    public:
        vector3d base;
        vector3d end;
        f32      radius;
    };

    // SM3's collision_capsule; the capsule is in the owner's frame
    class collision_capsule : public collision_geometry {

    public:
        capsule rel_cap;
    };

    ASSERT_OFFSETOF(collision_geometry, owner, 0x04);
    ASSERT_OFFSETOF(collision_geometry, flags, 0x0C);

    ASSERT_SIZEOF(capsule, 0x1C);

    ASSERT_OFFSETOF(collision_capsule, rel_cap, 0x10);
} // treyarch
