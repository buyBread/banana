#pragma once

#include "game/wds/entity/actor.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
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

    // SM3's conglomerate_flags_t, but 0x100
    enum e_conglomerate_flags : u32 {
        conglomerate_flag_visual_radius_dirty   = 0x00000001,
        conglomerate_flag_colgeom_radius_dirty  = 0x00000002,
        conglomerate_flag_has_colgeom_members   = 0x00000004,
        conglomerate_flag_has_ai_members        = 0x00000008,
        conglomerate_flag_possibly_render_valid = 0x00000010,
        conglomerate_flag_possibly_render       = 0x00000020,
        conglomerate_flag_is_cloned             = 0x00000040,
        conglomerate_flag_unk_00000100          = 0x00000100 // ped_manager::add_ped sets it with the float at +0x188
    };

    class conglomerate : public actor {

    public:
        mash::vector<entity>          members;
        mash::vector
            <ai_tentacle_info>*       tentacles;
        u8                            reserved_100[0x04];
        mash::vector_basic<po>        member_abs_po;
        mash::vector_basic<po>        all_rel_po;
        mash::vector_basic<po>        all_model_po; // als::biped_physics_mocomp saves and restores them
        mash::vector_basic_ref_counted
            <po>                      original_rel_po;
        mash::vector_basic_ref_counted
            <i8>                      member_parents;
        conglomerate_bone_index       root_index;
        mash::vector_basic_ref_counted
            <conglomerate_bone_index> bone_indices;
        mash::vector_basic<u16>       unk_154;
        mash::vector_basic_ref_counted
            <i16>                     skeleton_to_conglom_bone_mapping;
        mash::vector_basic_ref_counted
            <po>                      unassigned_skin_bone_rel_pos;
        mash::vector_basic_ref_counted
            <u8>                      unassigned_skin_bone_indices;
        mash::vector_basic_ref_counted
            <u8>                      unassigned_skin_bone_parent_indices;
        string_hash                   skeleton_name_hash;
        u8                            reserved_178[0x18];
        u32                           conglomerate_flags;
        u8                            reserved_194[0x04];

        explicit conglomerate(mash::generating_vtable_version so_do_nothing) : actor(so_do_nothing) {}

        ~conglomerate() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool            is_a_conglomerate() const override;
        void            set_flag_recursive(u32 flag, bool on) override;
        void            set_ext_flag_recursive(u32 flag, bool on) override;
        void            set_visible(bool visible, bool from_conglom) override;
        e_entity_flavor get_flavor() const override;
        void            post_unmash_fixup(amalga::resource_pack_slot* pack_slot) override;
        u32             get_entity_size() const override;
        void            clear_region(region* r, i32 i_know_what_i_am_doing) override;
        void            force_region(region* r) override;
        void            force_regions(entity* e) override;
        u32             possibly_render() const override;
        u32             possibly_collide() const override;
        bool            possibly_camera_collide() const override;
        bool            possibly_walkable() const override;
        void            region_update_poss_collide() override;
        bool            is_still_visible() const override;
        void            unk_108(f32 unk) override;
        void            render(f32 camera_distance, f32 entity_translucency_pct) override;
        f32             get_visual_radius() const override;
        vector3d        get_visual_center() const override;
        vector3d        unk_115() override;
        void            set_render_color(color32 color) override;
        void            set_render_alpha_mod(f32 alpha) override;
        void            set_render_scale(const vector3d &scale) override;
        void*           get_light_set() const override;
        void            set_recursive_age(f32 age) override;
        void            set_colgeom_flag_recursive(u32 flag, bool set) override;
        f32             get_colgeom_radius() const override;
        vector3d        get_colgeom_center() const override;
        void            radius_changed(bool force) override;

        virtual void unk_158(i32 unk, i32 unk_2); // add_moved unless the region is forced
        virtual void frame_done();
        virtual void frame_done_including_members();

        e_conglom_member_index get_member_index(const string_hash &nodename) const;
        entity*                get_member      (e_conglom_member_index index) const;

        bool has_member            (const string_hash &nodename) const;
        bool has_member_for_linking(const string_hash &nodename) const;

        static string_hash my_type_key;

    private:
        void finalize(mash::allocation_scope scope);
    };

    ASSERT_SIZEOF  (conglomerate_bone_index,                 0x14);
    ASSERT_OFFSETOF(conglomerate_bone_index, model_po_index, 0x04);

    ASSERT_SIZEOF  (conglomerate,                                      0x198);
    ASSERT_OFFSETOF(conglomerate, members,                             0x0E8);
    ASSERT_OFFSETOF(conglomerate, tentacles,                           0x0FC);
    ASSERT_OFFSETOF(conglomerate, member_abs_po,                       0x104);
    ASSERT_OFFSETOF(conglomerate, all_rel_po,                          0x114);
    ASSERT_OFFSETOF(conglomerate, all_model_po,                        0x124);
    ASSERT_OFFSETOF(conglomerate, original_rel_po,                     0x134);
    ASSERT_OFFSETOF(conglomerate, member_parents,                      0x138);
    ASSERT_OFFSETOF(conglomerate, root_index,                          0x13C);
    ASSERT_OFFSETOF(conglomerate, bone_indices,                        0x150);
    ASSERT_OFFSETOF(conglomerate, unk_154,                             0x154);
    ASSERT_OFFSETOF(conglomerate, skeleton_to_conglom_bone_mapping,    0x164);
    ASSERT_OFFSETOF(conglomerate, unassigned_skin_bone_rel_pos,        0x168);
    ASSERT_OFFSETOF(conglomerate, unassigned_skin_bone_indices,        0x16C);
    ASSERT_OFFSETOF(conglomerate, unassigned_skin_bone_parent_indices, 0x170);
    ASSERT_OFFSETOF(conglomerate, skeleton_name_hash,                  0x174);
    ASSERT_OFFSETOF(conglomerate, conglomerate_flags,                  0x190);
} // treyarch
