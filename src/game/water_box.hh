#pragma once

#include "game/math/algebra/vector3d.hh"
#include "game/wds/entity/actor.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class trigger;

    // a body of water: its box triggers say where it is and where its surface lies
    class water_box : public actor {

    public:
        trigger* m_trigger;
        trigger* unk_0ec; // the surface's
        u8       reserved_0f0[0x28];

        explicit water_box(mash::generating_vtable_version so_do_nothing) : actor(so_do_nothing) {}

        ~water_box() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool is_a_water_box() const override;
        void post_unmash_fixup(amalga::resource_pack_slot* pack_slot) override;
        u32  get_entity_size() const override;

        virtual bool point_inside(const vector3d &point);         // m_trigger's slot 54
        virtual bool unk_159(const vector3d &point);              // m_trigger's slot 55
        virtual bool surface_point_inside(const vector3d &point); // the surface's slot 54
        virtual bool unk_161(const vector3d &point);              // the surface's slot 55

        static string_hash my_type_key;
    };

    ASSERT_SIZEOF  (water_box,            0x118);
    ASSERT_OFFSETOF(water_box, m_trigger, 0x0E8);
    ASSERT_OFFSETOF(water_box, unk_0ec,   0x0EC);
} // treyarch
