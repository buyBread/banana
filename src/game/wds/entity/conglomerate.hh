#pragma once

#include "game/wds/entity/actor.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/vector.hh"
#include "treyarch/mash/vector_basic.hh"
#include "treyarch/mash/vector_basic_ref_counted.hh"
#include "game/math/po.hh"
#include "vc8/vector.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class ai_tentacle_info;
    class light_source;

    // members first, then the bones past them
    enum e_conglom_member_index : i16 {
        invalid_conglom_member = -2,
        base_conglom_index     = -1
    };

    struct conglomerate_bone_index {
        string_hash            name;
        i32                    model_po_index;
        i32                    parent_index;
        u16                    unk_0c;
        u16                    unk_0e;
        e_conglom_member_index child_index;
        e_conglom_member_index sibling_index;

        friend bool operator<(string_hash lhs, const conglomerate_bone_index &rhs) { return lhs < rhs.name; }
        friend bool operator>(string_hash lhs, const conglomerate_bone_index &rhs) { return rhs.name < lhs; }
    };

    // members and bone connectors are kept sorted by id
    inline bool operator<(string_hash lhs, const entity_base &rhs) { return lhs < rhs.id; }
    inline bool operator>(string_hash lhs, const entity_base &rhs) { return rhs.id < lhs; }

    // SM3's conglomerate_flags_t, but 0x100 and 0x4000
    enum e_conglomerate_flags : u32 {
        conglomerate_flag_visual_radius_dirty   = 0x00000001,
        conglomerate_flag_colgeom_radius_dirty  = 0x00000002,
        conglomerate_flag_has_colgeom_members   = 0x00000004,
        conglomerate_flag_has_ai_members        = 0x00000008,
        conglomerate_flag_possibly_render_valid = 0x00000010,
        conglomerate_flag_possibly_render       = 0x00000020,
        conglomerate_flag_is_cloned             = 0x00000040,
        conglomerate_flag_unk_00000100          = 0x00000100, // ped_manager::add_ped sets it with conglomerate_visual_radius
        conglomerate_flag_unk_00004000          = 0x00004000
    };

    class conglomerate : public actor {

    public:
        mash::vector<entity_base>     members;
        mash::vector
            <ai_tentacle_info>*       tentacles;
        mash::vector
            <entity_base>*            masher_skin_bones;
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
        vc8::vector<entity_base*>*    bone_connectors;
        light_manager*                lightmgr;
        vc8::vector<actor*>*          collision_members;
        vc8::vector<light_source*>*   light_members;
        mutable f32                   conglomerate_visual_radius;
        mutable f32                   conglomerate_colgeom_radius;
        mutable u32                   conglomerate_flags;
        e_conglom_member_index        attach_index;
        u8                            pad_196[0x02];

        explicit conglomerate(mash::generating_vtable_version so_do_nothing) : actor(so_do_nothing) {}

        ~conglomerate() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        void custom_unmash(mash::mash_info_struct* mash_info,
                           void*                   containing_class_ptr,
                           mash::buffer_type       buffer);

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
        light_manager*  get_light_set() const override;
        void            set_recursive_age(f32 age) override;
        void            set_colgeom_flag_recursive(u32 flag, bool set) override;
        f32             get_colgeom_radius() const override;
        vector3d        get_colgeom_center() const override;
        void            radius_changed(bool force) override;

        virtual void compute_sector(terrain* ter, bool use_high_res_intersect); // add_moved unless the region is forced
        virtual void frame_done();
        virtual void frame_done_including_members();

        e_conglom_member_index get_member_index(const string_hash &nodename) const;
        entity_base*           get_member      (e_conglom_member_index index) const;
        entity_base*           get_member      (const string_hash &nodename) const;
        const string_hash     &get_member_id   (e_conglom_member_index index) const;

        bool has_member            (const string_hash &nodename) const;
        bool has_member_for_linking(const string_hash &nodename) const;

        entity_base* get_member_for_linking(e_conglom_member_index index); // a bone gets its connector the first time
        entity_base* get_member_for_linking(const string_hash &nodename);

        bool get_member_abs_position(e_conglom_member_index index, vector3d* pos, const po* model_po_override = nullptr) const;
        bool get_member_abs_position(const string_hash &nodename, vector3d* pos, const po* model_po_override = nullptr) const;
        bool get_member_abs_po      (e_conglom_member_index index, po* the_po, const po* model_po_override = nullptr) const;
        bool get_member_abs_po      (const string_hash &nodename, po* the_po, const po* model_po_override = nullptr) const;

        bool set_member_abs_position(e_conglom_member_index index, const vector3d &pos);
        bool set_member_abs_po      (e_conglom_member_index index, const po &the_po, po* model_po_override = nullptr);
        bool set_member_abs_po      (const string_hash &nodename, const po &the_po, po* model_po_override = nullptr);

        void dirty_bone_connectors(bool force_entire_tree = false);
        void create_parentage_tree();

        // sub_42E530
        // inlined @ sub_617640
        // inlined @ sub_63AC20
        // inlined @ sub_63AEE0
        // inlined @ sub_63AF70
        bool has_collision_members() const {
            return collision_members && collision_members->size() > 0;
        }

        void add_member_lights_to_region     (region* the_region);
        void remove_member_lights_from_region(region* the_region);

        void set_all_collisions_active(bool val);

        static string_hash my_type_key;

    protected:
        void translate_member_model_position_recurse(const conglomerate_bone_index &bone_index, const vector3d &delta); // no bounds check

    private:
        void initialize(mash::allocation_scope scope);
        void finalize(mash::allocation_scope scope);
    };

    namespace references {
        // the variant conglomerates waiting for a random variant
        inline util::memory_reference<vc8::vector<arch_base_vhandle>> random_variant_queue { 0x01003620 };
    } // references

    ASSERT_SIZEOF  (conglomerate_bone_index,                 0x14);
    ASSERT_OFFSETOF(conglomerate_bone_index, model_po_index, 0x04);
    ASSERT_OFFSETOF(conglomerate_bone_index, parent_index,   0x08);
    ASSERT_OFFSETOF(conglomerate_bone_index, child_index,    0x10);
    ASSERT_OFFSETOF(conglomerate_bone_index, sibling_index,  0x12);

    ASSERT_SIZEOF  (conglomerate,                                      0x198);
    ASSERT_OFFSETOF(conglomerate, members,                             0x0E8);
    ASSERT_OFFSETOF(conglomerate, tentacles,                           0x0FC);
    ASSERT_OFFSETOF(conglomerate, masher_skin_bones,                   0x100);
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
    ASSERT_OFFSETOF(conglomerate, bone_connectors,                     0x178);
    ASSERT_OFFSETOF(conglomerate, lightmgr,                            0x17C);
    ASSERT_OFFSETOF(conglomerate, collision_members,                   0x180);
    ASSERT_OFFSETOF(conglomerate, light_members,                       0x184);
    ASSERT_OFFSETOF(conglomerate, conglomerate_visual_radius,          0x188);
    ASSERT_OFFSETOF(conglomerate, conglomerate_colgeom_radius,         0x18C);
    ASSERT_OFFSETOF(conglomerate, conglomerate_flags,                  0x190);
    ASSERT_OFFSETOF(conglomerate, attach_index,                        0x194);
} // treyarch
