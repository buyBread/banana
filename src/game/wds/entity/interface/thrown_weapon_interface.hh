#pragma once

#include "game/wds/entity/interface/weapon_interface_extender.hh"
#include "treyarch/mash/types.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class thrown_weapon_interface : public weapon_interface_extender {

    public:
        u8 reserved_00c[0x2D4];

        explicit thrown_weapon_interface(mash::generating_vtable_version so_do_nothing) : weapon_interface_extender(so_do_nothing) {}

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        e_entity_ifc get_ifc_id() const override;

        void set_owner(entity* owner) override;

        i32 unk_019(void* unk) override;
        i32 unk_020() override;
        i32 unk_021() override;

        virtual i32   unk_022();
        virtual i32   unk_023();
        virtual i32   unk_024();
        virtual i32   unk_025();
        virtual i32   unk_026(i32 unk, i32 unk_2, i32 unk_3);
        virtual i32   unk_027(i32 unk);
        virtual void* spawn_grenade(bool unk, void* unk_2, void* unk_3, f32 unk_4, bool unk_5, i32 unk_6);
    };

    ASSERT_SIZEOF(thrown_weapon_interface, 0x2E0);
} // treyarch
