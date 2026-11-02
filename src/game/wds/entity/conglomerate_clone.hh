#pragma once

#include "game/color.hh"
#include "game/wds/entity/actor.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mm/mm.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    struct conglomerate_clone_copy_data {
        u8                reserved_00[0x0C];
        arch_base_vhandle shared_conglom_vhandle;
        void*             core_render_members;
        i32               num_render_members;
        u8                reserved_18[0x04];

        // inlined @ sub_6656E0
        ~conglomerate_clone_copy_data() {
            if (core_render_members)
                mm::free(core_render_members);

            core_render_members = nullptr;
            num_render_members  = 0;
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }
    };

    class conglomerate_clone : public actor {

    public:
        conglomerate_clone_copy_data* copy_data;

        explicit conglomerate_clone(mash::generating_vtable_version so_do_nothing) : actor(so_do_nothing) {}

        ~conglomerate_clone() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool            is_a_conglomerate_clone() const override;
        e_entity_flavor get_flavor() const override;
        void            post_unmash_fixup(amalga::resource_pack_slot* pack_slot) override;
        u32             get_entity_size() const override;
        u32             possibly_render() const override;
        void            unk_108(f32 unk) override;
        f32             get_visual_radius() const override;
        vector3d        get_visual_center() const override;

        virtual void set_clone_members_extra_info();

        static string_hash my_type_key;
    };

    class dynamic_conglomerate_clone : public conglomerate_clone {

    public:
        mash::vector_basic<color32>   render_colors;
        u8                            reserved_0fc[0x04];
        last_collision_free_state_t** last_collision_free_states; // one per member, by clone_member_idx
        u8                            reserved_104[0x04];

        explicit dynamic_conglomerate_clone(mash::generating_vtable_version so_do_nothing) : conglomerate_clone(so_do_nothing) {}

        ~dynamic_conglomerate_clone() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool            is_a_dynamic_conglomerate_clone() const override;
        e_entity_flavor get_flavor() const override;
        u32             get_entity_size() const override;
        u32             possibly_render() const override;
        u32             possibly_collide() const override;
        bool            possibly_camera_collide() const override;
        bool            possibly_walkable() const override;
        void            unk_108(f32 unk) override;
        f32             get_colgeom_radius() const override;
        vector3d        get_colgeom_center() const override;
        void            set_clone_members_extra_info() override;

        void save_last_collision_free_state(actor* actor_member, const po &matrix, const capsule &rel_cap);

        static string_hash my_type_key;

    private:
        void finalize(mash::allocation_scope scope);
    };

    ASSERT_SIZEOF  (conglomerate_clone_copy_data,                         0x1C);
    ASSERT_OFFSETOF(conglomerate_clone_copy_data, shared_conglom_vhandle, 0x0C);
    ASSERT_OFFSETOF(conglomerate_clone_copy_data, core_render_members,    0x10);
    ASSERT_OFFSETOF(conglomerate_clone_copy_data, num_render_members,     0x14);

    ASSERT_SIZEOF  (conglomerate_clone,            0xEC);
    ASSERT_OFFSETOF(conglomerate_clone, copy_data, 0xE8);

    ASSERT_SIZEOF  (dynamic_conglomerate_clone,                             0x108);
    ASSERT_OFFSETOF(dynamic_conglomerate_clone, render_colors,              0x0EC);
    ASSERT_OFFSETOF(dynamic_conglomerate_clone, last_collision_free_states, 0x100);
} // treyarch
