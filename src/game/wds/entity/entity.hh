#pragma once

#include "game/wds/entity/entity_base.hh"
#include "game/container/fixed_vector.hh"
#include "game/color.hh"
#include "game/mutex.hh"
#include "treyarch/mash/string.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "game/math/po.hh"
#include "vc8/list.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/types.hh"

namespace treyarch {
    class terrain;
    struct region;

    namespace ngl {
        struct mesh;
    } // ngl

    class entity : public entity_base {

    public:
        container::fixed_vector<region*, 8> regions;
        u8                                  reserved_074[0x1C];
        u8                                  unk_090[0x0C]; // a mash::string the destructor finalizes
        u8                                  reserved_09c[0x04];

        explicit entity(mash::generating_vtable_version so_do_nothing) : entity_base(so_do_nothing) {}

        ~entity() override;

        void construct_mashed_class() override;
        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        bool            is_an_entity() const override;
        void            set_visible(bool visible, bool from_conglom) override;
        bool            was_occluded_last_frame() override;
        e_entity_flavor get_flavor() const override;
        u32             get_entity_size() const override;

        virtual void          clear_region(region* r, i32 i_know_what_i_am_doing);
        virtual void          compute_sector(terrain* ter, bool use_high_res_intersect, entity* original);
        virtual void          force_region(region* r);
        virtual void          force_current_region();
        virtual void          unforce_regions();
        virtual void          force_regions(entity* e);
        virtual void          set_family_visible(bool visible);
        virtual u32           possibly_render() const;
        virtual u32           possibly_collide() const;
        virtual bool          possibly_camera_collide() const;
        virtual bool          possibly_walkable() const;
        virtual void          region_update_poss_collide();
        virtual bool          unk_100();
        virtual void          unk_101(bool on); // ext flag 0x1000, which also takes the entity in or out of limbo
        virtual bool          unk_102() const;  // ext flag 0x1000
        virtual void          unk_103(bool on); // flag 0x200000, then slot 101
        virtual bool          unk_104() const;  // flag 0x200000
        virtual void          frame_advance(f32 t);
        virtual bool          is_still_visible() const;
        virtual void          render_skeleton();
        virtual void          unk_108(f32 unk);
        virtual void          render(f32 camera_distance, f32 entity_translucency_pct);
        virtual ngl::mesh*    get_mesh() const;
        virtual ngl::mesh*    unk_111() const;
        virtual bool          has_mesh() const;
        virtual f32           get_visual_radius() const;
        virtual vector3d      get_visual_center() const;
        virtual vector3d      unk_115();        // slot 114's answer
        virtual void          suspend(bool update_region);
        virtual void          unsuspend(bool update_region);
        virtual void          set_render_color(color32 color);
        virtual color32       get_render_color() const;
        virtual void          set_render_alpha_mod(f32 alpha);
        virtual f32           get_render_alpha_mod() const;
        virtual void          set_render_scale(const vector3d &scale);
        virtual vector3d      get_render_scale() const;
        virtual void          set_render_zbias(f32 zbias);
        virtual f32           get_render_zbias() const;
        virtual void*         get_light_set() const;
        virtual bool          can_be_a_lego() const;
        virtual bool          can_be_a_conglom_clone() const;
        virtual f32           get_age() const;
        virtual void          set_age(f32 age);
        virtual void          set_recursive_age(f32 age);
        virtual void          start_recording();
        virtual void          stop_recording();
        virtual bool          hates_spiderman();
        virtual mash::string* get_signaller_owner_name(mash::string* result) const;

        void invoke_render_phase();

        region* get_primary_region();

        // inlined
        bool is_in_limbo() {
            return (flags & entity_flag_in_limbo) || !get_primary_region();
        }

        using entity_search_list = vc8::list<entity*>;

        // held while a script reads found_entities
        static void lock_found_entities();
        static void unlock_found_entities();

    private:
        void finalize(mash::allocation_scope scope);
    };

    namespace references {
        // what the last entity search found
        inline util::memory_reference<entity::entity_search_list*> found_entities        { 0x00FFEB2C };
        inline util::memory_reference<bool>                        found_entities_locked { 0x00FFEB1A };
        inline util::memory_reference<engine_recursive_lock>       found_entities_lock   { 0x01003240 };
    } // references

    ASSERT_SIZEOF  (entity,          0xA0);
    ASSERT_OFFSETOF(entity, regions, 0x50);
    ASSERT_OFFSETOF(entity, unk_090, 0x90);
} // treyarch
