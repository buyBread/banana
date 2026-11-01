#pragma once

#include "game/event/event_callback.hh"
#include "game/arch_base_vhandle.hh"
#include "treyarch/mash/string_hash.hh"
#include "treyarch/mash/types.hh"
#include "treyarch/mash/virtual_base.hh"
#include "banana/util/macros/sanity_assert.hh"

namespace treyarch {
    class event;

    namespace chuck { namespace vm {
        class script_executable;
        class script_function;
        class script_instance;
    }}

    // its vtable is mash_virtual_base's, then one is_* test per subclass
    class arch_base : public mash::mash_virtual_base {

    public:
        arch_base_vhandle my_handle;

        arch_base() = default;

        explicit arch_base(mash::generating_vtable_version so_do_nothing) : mash_virtual_base(so_do_nothing) {}

        ~arch_base() override;

        void destruct_mashed_class() override;
        void unmash(mash::mash_info_struct* mash_info,
                    void*                   containing_class_ptr,
                    mash::buffer_type       buffer) override;

        mash::virtual_types_key get_virtual_type_key() const override;
        bool                    is_subclass_of(mash::virtual_types_key parent_class) const override;
        i32                     get_mash_sizeof() const override;

        virtual bool is_an_arch_base() const;
        virtual bool is_an_entity_base() const;
        virtual bool is_an_entity() const;
        virtual bool is_an_actor() const;
        virtual bool is_a_conglomerate() const;
        virtual bool is_a_conglomerate_clone() const;
        virtual bool is_a_dynamic_conglomerate_clone() const;
        virtual bool is_a_grenade() const;
        virtual bool is_a_beam() const;
        virtual bool is_a_particle_generator() const;
        virtual bool is_a_polytube() const;
        virtual bool is_a_sky() const;
        virtual bool is_a_sign() const;
        virtual bool is_a_web_wall() const;
        virtual bool is_a_marker() const;
        virtual bool is_an_ai_cover_marker() const;
        virtual bool is_an_anchor_marker() const;
        virtual bool is_a_line_marker_base() const;
        virtual bool is_a_line_anchor() const;
        virtual bool is_a_camera() const;
        virtual bool is_a_game_camera() const;
        virtual bool is_a_marky_camera() const;
        virtual bool is_a_spiderman_camera() const;
        virtual bool is_an_animated_camera() const;
        virtual bool is_a_light_source() const;
        virtual bool is_a_door() const;
        virtual bool unk_035() const;
        virtual bool is_a_trigger() const;
        virtual bool is_a_box_trigger() const;
        virtual bool is_a_sphere_trigger() const;
        virtual bool is_a_double_sphere_trigger() const;
        virtual bool unk_040() const;
        virtual bool is_a_vehicle() const;
        virtual bool is_a_water_box() const;
        virtual bool unk_043() const;
        virtual bool unk_044() const;
        virtual bool is_a_fight_group() const;
        virtual bool is_a_fakir() const;
        virtual bool is_a_script_instance_shadow() const;
        virtual bool is_a_script_controller() const;
        virtual bool is_a_mission_progression_data() const;
        virtual bool is_a_mission_stage_data() const;
        virtual bool is_a_mission_objective_data() const;
        virtual bool is_a_finger_of_god() const;

        void raise_event(string_hash event_type_id);
        void raise_event(event* raised_event);

        u32 add_callback(      string_hash                 event_type_id,
                               chuck::vm::script_instance* instance,
                               chuck::vm::script_function* function,
                         const void*                       parameters,
                               bool                        one_shot);
        u32 add_callback(string_hash                  event_type_id,
                         code_event_callback_function function,
                         void*                        parameters,
                         bool                         one_shot);

        void remove_callback(u32 callback_id, string_hash event_type_id);

        void clear_script_callbacks(chuck::vm::script_executable* script_exe);
        void clear_script_callback(string_hash function_name);
        void clear_script_callback(u32 id);

    private:
        void finalize(mash::allocation_scope scope);
    };

    ASSERT_SIZEOF  (arch_base,            0x08);
    ASSERT_OFFSETOF(arch_base, my_handle, 0x04);
} // treyarch
