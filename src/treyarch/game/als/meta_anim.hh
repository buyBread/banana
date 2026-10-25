#pragma once

#include <cstddef>

#include "treyarch/game/als/als.hh"
#include "treyarch/nal/anim.hh"
#include "treyarch/nal/anim_instance.hh"
#include "treyarch/nal/skeleton_map.hh"
#include "treyarch/nal/transport_list.hh"
#include "treyarch/shared/hash/string_hash.hh"
#include "treyarch/shared/mash/mash_info.hh"
#include "treyarch/shared/mash/string.hh"
#include "treyarch/shared/mash/types.hh"
#include "treyarch/shared/mash/vector.hh"
#include "treyarch/shared/mash/virtual_base.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    class actor;

    namespace als {
        class animation_logic_system;
        class state_machine;

        // the side that's set samples the pose
        struct nal_als_instance_bridge {
            nal::anim_instance* nal_inst;
            void*               als_inst;
        };

        struct alignas(8) meta_key_anim {
            nal_anim_id anim_id;
            nal::anim*  the_anim_ptr;
            u8          pad_024[0x04];

            void construct_mashed_class();
            void destruct_mashed_class();
            void unmash(mash::mash_info_struct* mash_info,
                        void*                   containing_class_ptr,
                        mash::buffer_type       buffer);

            void lookup_nal_animation();
        };

        class als_meta_anim_base : public mash::mash_virtual_base {

        public:
            u8          pad_004[0x04];
            nal_anim_id my_name;

            als_meta_anim_base() = default;

            // inlined @ sub_71B9C0
            // inlined @ sub_71C670
            // inlined @ sub_71EB70
            // inlined @ sub_7204D0
            // inlined @ sub_7205D0
            // inlined @ sub_720760
            explicit als_meta_anim_base(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing) {}

            void construct_mashed_class() override;
            void destruct_mashed_class() override;
            void unmash(mash::mash_info_struct* mash_info,
                        void*                   containing_class_ptr,
                        mash::buffer_type       buffer) override;

            mash::virtual_types_key get_virtual_type_key() const override;
            bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;

            virtual const nal_anim_id         &get_anim_name() const;
            virtual nal::anim*                 get_anim_proxy() const = 0;
            virtual bool                       is_anim_looping() const;
            virtual f32                        get_anim_duration() const;
            virtual const nal::transport_list* get_skeleton() const;

            // the_anim is what the instance will report as its anim; the nal meta anim passes itself
            virtual nal_als_instance_bridge create_anim_inst(nal::transport_list*    the_skel,
                                                             u32                     flags,
                                                             nal::skeleton_map*      skel_map,
                                                             u32                     unk,
                                                             void*                   the_anim,
                                                             animation_logic_system* the_als,
                                                             state_machine*          the_layer) = 0;

            virtual bool is_delay_create();
            virtual void delay_create(actor* the_actor);
            virtual void load_key_anims();

            static string_hash my_type_key;
        };

        // stands in for a meta anim wherever a nal anim is expected; never mashed, the table makes them
        class als_nal_meta_anim : public als_meta_anim_base {

        public:
            als_meta_anim_base* my_meta_anim_data;
            u32                 unk_2c;

            als_nal_meta_anim();

            void* operator new  (std::size_t size) noexcept;
            void* operator new[](std::size_t size) noexcept;
            void  operator delete  (void* allocation) noexcept;
            void  operator delete[](void* allocation) noexcept;

            void construct_mashed_class() override;
            void destruct_mashed_class() override;
            void unmash(mash::mash_info_struct* mash_info,
                        void*                   containing_class_ptr,
                        mash::buffer_type       buffer) override;

            mash::virtual_types_key get_virtual_type_key() const override;
            bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
            i32                     get_mash_sizeof() const override;

            nal::anim*              get_anim_proxy() const override;
            nal_als_instance_bridge create_anim_inst(nal::transport_list*    the_skel,
                                                     u32                     flags,
                                                     nal::skeleton_map*      skel_map,
                                                     u32                     unk,
                                                     void*                   the_anim,
                                                     animation_logic_system* the_als,
                                                     state_machine*          the_layer) override;
            bool                    is_delay_create() override;
            void                    delay_create(actor* the_actor) override;

            void create(als_meta_anim_base* the_meta_anim_data, bool delay);

            static string_hash my_type_key;
        };

        class als_meta_anim_by_filename : public als_meta_anim_base {

        public:
            nal::anim*   the_anim_ptr;
            mash::string m_anim_prefix;

            als_meta_anim_by_filename() = default;

            // inlined @ sub_71B9C0
            // inlined @ sub_71C670
            // inlined @ sub_7204D0
            // inlined @ sub_7205D0
            explicit als_meta_anim_by_filename(mash::generating_vtable_version so_do_nothing) : als_meta_anim_base(so_do_nothing),
                                                                                                m_anim_prefix(so_do_nothing) {}

            void construct_mashed_class() override;
            void destruct_mashed_class() override;
            void unmash(mash::mash_info_struct* mash_info,
                        void*                   containing_class_ptr,
                        mash::buffer_type       buffer) override;

            mash::virtual_types_key get_virtual_type_key() const override;
            bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
            i32                     get_mash_sizeof() const override;

            nal::anim*              get_anim_proxy() const override;
            nal_als_instance_bridge create_anim_inst(nal::transport_list*    the_skel,
                                                     u32                     flags,
                                                     nal::skeleton_map*      skel_map,
                                                     u32                     unk,
                                                     void*                   the_anim,
                                                     animation_logic_system* the_als,
                                                     state_machine*          the_layer) override;
            bool                    is_delay_create() override;
            void                    delay_create(actor* the_actor) override;

            virtual nal_anim_id get_anim_id_to_play(actor* the_actor) = 0;

            static string_hash my_type_key;
        };

        // the shared meta anims, and a nal meta anim standing in for each
        class als_meta_anim_table_shared {

        public:
            mash::vector
                <als_meta_anim_base> my_meta_anim;
            als_nal_meta_anim*       my_nal_meta_anims; // one per meta anim

            void construct_mashed_class();
            void destruct_mashed_class();
            void unmash(mash::mash_info_struct* mash_info,
                        void*                   containing_class_ptr,
                        mash::buffer_type       buffer);

            void initialize();
            void finalize();
        };

        ASSERT_SIZEOF  (nal_als_instance_bridge, 0x08);

        ASSERT_SIZEOF  (meta_key_anim,               0x28);
        ASSERT_OFFSETOF(meta_key_anim, the_anim_ptr, 0x20);

        ASSERT_SIZEOF  (als_meta_anim_base,          0x28);
        ASSERT_OFFSETOF(als_meta_anim_base, my_name, 0x08);

        ASSERT_SIZEOF  (als_nal_meta_anim,                    0x30);
        ASSERT_OFFSETOF(als_nal_meta_anim, my_meta_anim_data, 0x28);
        ASSERT_OFFSETOF(als_nal_meta_anim, unk_2c,            0x2C);

        ASSERT_SIZEOF  (als_meta_anim_by_filename,                0x38);
        ASSERT_OFFSETOF(als_meta_anim_by_filename, the_anim_ptr,  0x28);
        ASSERT_OFFSETOF(als_meta_anim_by_filename, m_anim_prefix, 0x2C);

        ASSERT_SIZEOF  (als_meta_anim_table_shared,                    0x18);
        ASSERT_OFFSETOF(als_meta_anim_table_shared, my_meta_anim,      0x00);
        ASSERT_OFFSETOF(als_meta_anim_table_shared, my_nal_meta_anims, 0x14);
    } // als
} // treyarch
