#pragma once

#include <array>

#include "game/als/als.hh"
#include "game/animation/animation_controller.hh"
#include "game/als/internal.hh"
#include "game/als/mocomp.hh"
#include "game/als/param.hh"
#include "game/als/state_machine.hh"
#include "game/hash/algo.hh"
#include "game/math/algebra/vector3d.hh"
#include "treyarch/mm/mm.hh"
#include "game/mutex.hh"
#include "game/resource_key.hh"
#include "banana/util/macros/sanity_assert.hh"
#include "banana/util/memory_reference.hh"
#include "banana/util/types.hh"
#include "vc8/vector.hh"

namespace treyarch { namespace als {
    class expression_animation_entry;

    // an actor's animation logic: its base machine, its layers and the mocomp moving it
    class animation_logic_system : public animation_logic_system_interface {

    public:
        internal_parameter_cache           the_cache;
        animation_logic_system_shared*     als_shared;
        vc8::vector<layer_state_machine*>  the_layers;
        base_state_machine                 the_base_sm;
        actor*                             the_actor;
        animation_controller*              the_controller;
        vector3d                           actor_position;
        i32                                mocomp_layer_index;
        motion_compensator*                curr_mocomp;
        motion_compensator*                the_mocomp_data; // 256 bytes the mocomps are built in
        engine_recursive_lock*             access_lock;
        const expression_animation_entry*  curr_expression; // the entry the morph came from
        bool                               is_suspended;
        bool                               needs_reinit_anim;
        bool                               unk_a72;
        bool                               publishing_matrices; // the controller's matrices aren't skipped
        bool                               saved_skip_matrices; // its request from before they were
        bool                               force_matrices;      // publish next frame whatever the visibility
        u8                                 pad_a76[0x02];
        animation_logic_system_interface*  coordinator;
        i32                                coordinator_unk;
        vc8::vector
            <animation_logic_system_interface*> coordinated;

        explicit animation_logic_system(actor* _the_actor);
        ~animation_logic_system() override;

        void* operator new(std::size_t size) noexcept {
            return mm::allocate(size);
        }

        void operator delete(void* allocation) noexcept {
            mm::free(allocation);
        }

        state_machine_interface* get_als_layer(layer_id the_layer) override;
        bool                     has_als_layer(layer_id the_layer) const override;

        bool   did_transition_succeed() const override;
        bool   did_transition_succeed_from_request() const override;
        void   suspend_logic_system(bool suspend) override;
        bool   als_is_suspended() const override;
        void   create_instance_data(animation_logic_system_shared* shared_portion) override;
        void   delete_instance_data() override;
        void   reset_animation_player() override;
        engine_recursive_lock* get_access_lock() const override;
        bool   is_high_priority() const override;
        actor* get_actor() const override;
        void   cache_actor_position() override;

        void                              set_coordinator(animation_logic_system_interface* new_coordinator, i32 unk) override;
        bool                              has_coordinator() const override;
        animation_logic_system_interface* get_coordinator() const override;
        i32                               get_coordinator_unk() const override;
        bool                              is_coordinator() const override;
        vc8::vector
            <animation_logic_system_interface*>* get_coordinated() override;
        void                              add_coordinated(animation_logic_system_interface* the_als) override;
        void                              remove_coordinated(animation_logic_system_interface* the_als) override;

        void                  force_matrix_update() override;
        animation_controller* get_controller() const override;
        void                  frame_advance(f32 delta_t, bool force) override;

        virtual void set_expression(const expression_animation_entry* entry); // picks the entry's morph when it changes
        virtual void change_mocomp(const state* curr_state, layer_id layer);

        // runs what the frame queued, stage by stage; threaded lets the stages use the workers
        static void frame_advance_flush_queue(bool threaded);
        static void process_nal_queue(bool threaded);

        const als_meta_anim_table_shared* get_meta_anim_table() const;

        // the controller may skip its matrices while the actor isn't visible, unless it's tied to others or the camera wants them
        void update_matrix_publishing(bool visible, bool keep_publishing);

        string_hash get_internal_hash(state_machine* the_sm, param_id param);

        state_machine* get_als_layer_internal(layer_id the_layer);
        bool           has_als_layer_internal(layer_id the_layer) const;
        bool           is_als_interruptable() const;
        void           end_layer(layer_id the_layer);

    protected:
        // the flush's per-item setup; false leaves the controller out of the job
        static bool prepare_controller_job(animation_controller* controller, f32 delta_t);
        static void prepare_pre_event_job (state_machine* machine, f32 delta_t);

        void enter_biped_physics(state_machine** machines, i32 num_machines, bool unk);
        void exit_biped_physics();
        void update_seated_in_car(state_machine* the_sm);
    };

    namespace references {
        // what frame_advance leaves for the flush; parallel arrays share an index
        inline util::memory_reference<std::array<animation_controller*, 500>> queued_controllers        { 0x00FD43D0 };
        inline util::memory_reference<std::array<motion_compensator*, 500>>   queued_mocomps            { 0x00FFBE58 };
        inline util::memory_reference<std::array<f32, 500>>                   queued_deltas             { 0x00FD74A0 };
        inline util::memory_reference<std::array<animation_records*, 500>>    queued_signal_records     { 0x00FD6C40 };
        inline util::memory_reference<std::array<state_machine*, 500>>        queued_event_machines     { 0x00FFC670 };
        inline util::memory_reference<std::array<state_machine*, 500>>        queued_pre_event_machines { 0x00FD7C78 };
        inline util::memory_reference<std::array<f32, 500>>                   queued_pre_event_deltas   { 0x00FF8498 };

        inline util::memory_reference<i32> num_queued_controllers        { 0x00FD4BA0 };
        inline util::memory_reference<i32> num_queued_mocomps            { 0x00FFC628 };
        inline util::memory_reference<i32> num_queued_deltas             { 0x00FD7C70 };
        inline util::memory_reference<i32> num_queued_signal_records     { 0x00FD7410 };
        inline util::memory_reference<i32> num_queued_event_machines     { 0x00FFCE40 };
        inline util::memory_reference<i32> num_queued_pre_event_machines { 0x00FD8448 };
        inline util::memory_reference<i32> num_queued_pre_event_deltas   { 0x00FF8C68 };

        // the pose job's result matrices come out of this, 16-aligned
        inline util::memory_reference<u8> pose_scratch { 0x00FD8490 };

        inline util::memory_reference<engine_recursive_lock> anim_start_lock { 0x00FD8458 }; // finding and starting a state's animation
        inline util::memory_reference<engine_recursive_lock> queue_lock      { 0x00FD8478 };
    } // references

    // what a state's param block can say about how its animation starts
    inline const string_hash capsule_base_id               = string_hash(hash::djb2("capsule_base"));
    inline const string_hash capsule_end_id                = string_hash(hash::djb2("capsule_end"));
    inline const string_hash capsule_radius_id             = string_hash(hash::djb2("capsule_radius"));
    inline const string_hash blend_time_override_id        = string_hash(hash::djb2("blend_time_override"));
    inline const string_hash custom_blend_time_id          = string_hash(hash::djb2("custom_blend_time"));
    inline const string_hash start_time_offset_override_id = string_hash(hash::djb2("start_time_offset_override"));
    inline const string_hash mirrored_animation_id         = string_hash(hash::djb2("mirrored_animation"));
    inline const string_hash expression_animation_data_id  = string_hash(hash::djb2("expression_animation_data"));
    inline const string_hash seated_in_car_id              = string_hash(hash::djb2("seated_in_car"));

    inline const string_hash als_state_change       = string_hash(hash::djb2("ALS_STATE_CHANGE"));
    inline const string_hash blend_in_complete_time = string_hash(hash::djb2("BLEND_IN_COMPLETE_TIME"));

    f32 get_default_blend_time(u32 flags, f32 duration);

    // what an entity's resource asks for; post-load setup turns it into the actor's als
    struct als_res_data {
        resource_key                      key;
        animation_logic_system_interface* the_als;

        void post_unmash_fixup(actor* owner, u32 unk);
    };

    ASSERT_SIZEOF  (animation_logic_system,                     0xA90);
    ASSERT_OFFSETOF(animation_logic_system, the_cache,          0x008);
    ASSERT_OFFSETOF(animation_logic_system, als_shared,         0x274);
    ASSERT_OFFSETOF(animation_logic_system, the_layers,         0x278);
    ASSERT_OFFSETOF(animation_logic_system, the_base_sm,        0x288);
    ASSERT_OFFSETOF(animation_logic_system, the_actor,          0xA48);
    ASSERT_OFFSETOF(animation_logic_system, the_controller,     0xA4C);
    ASSERT_OFFSETOF(animation_logic_system, actor_position,     0xA50);
    ASSERT_OFFSETOF(animation_logic_system, mocomp_layer_index, 0xA5C);
    ASSERT_OFFSETOF(animation_logic_system, curr_mocomp,        0xA60);
    ASSERT_OFFSETOF(animation_logic_system, the_mocomp_data,    0xA64);
    ASSERT_OFFSETOF(animation_logic_system, access_lock,        0xA68);
    ASSERT_OFFSETOF(animation_logic_system, curr_expression,    0xA6C);
    ASSERT_OFFSETOF(animation_logic_system, is_suspended,       0xA70);
    ASSERT_OFFSETOF(animation_logic_system, needs_reinit_anim,  0xA71);
    ASSERT_OFFSETOF(animation_logic_system, unk_a72,            0xA72);
    ASSERT_OFFSETOF(animation_logic_system, publishing_matrices, 0xA73);
    ASSERT_OFFSETOF(animation_logic_system, force_matrices,      0xA75);
    ASSERT_OFFSETOF(animation_logic_system, coordinator,        0xA78);
    ASSERT_OFFSETOF(animation_logic_system, coordinator_unk,    0xA7C);
    ASSERT_OFFSETOF(animation_logic_system, coordinated,        0xA80);

    ASSERT_SIZEOF  (als_res_data,          0x0C);
    ASSERT_OFFSETOF(als_res_data, the_als, 0x08);
}} // treyarch::als
