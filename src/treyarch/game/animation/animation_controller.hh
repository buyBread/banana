#pragma once

#include "treyarch/game/als/meta_anim.hh"
#include "treyarch/game/als/meta_anim_instance.hh"
#include "treyarch/nal/nal.hh"
#include "treyarch/nal/pose.hh"
#include "treyarch/shared/container/fixed_vector.hh"
#include "treyarch/shared/math/po.hh"
#include "treyarch/shared/mutex.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"
#include "vc8/vector.hh"

namespace treyarch {
    namespace nal {
        struct anim;
        struct anim_instance;
        struct pose_layout;
        struct skeleton_map;
        struct transport_list;
    }

    class actor;
    class animation_controller;
    class entity;
    struct anim_event_param;

    // a bump allocator over a scratch buffer
    struct scratch_arena {
        u32 size;
        u32 alignment;
        u8* base;
        u8* cursor;
    };

    // what a controller points at; the milestone asserts on it as `state`
    struct animation_controller_state {
        // an animation playing on a layer; one of a nal instance or an als meta instance samples it
        struct layer_anim {
            nal::anim_instance*         nal_inst;
            als::als_anim_instance*     als_inst;
            f32                         normalized_time;
            f32                         prev_normalized_time;
            nal::pose                   pose;
            f32                         alpha;
            f32                         blend_start;          // alpha runs from alpha_start to alpha_end over these normalized times
            f32                         blend_end;
            f32                         alpha_start;
            f32                         alpha_end;
            f32                         speed;
            f32                         secondary_speed;
            bool                        just_started;         // until the frame after it starts; its signals then include the span's start
            u8                          pad_06d[0x03];
            animation_controller_state* owner;

            layer_anim(animation_controller_state* the_owner, const als::nal_als_instance_bridge &instance, nal::pose_layout* layout);
            ~layer_anim();

            // from a pool
            void* operator new   (std::size_t size) noexcept;
            void  operator delete(void* allocation) noexcept;

            // another 1024 entries, threaded first to last; null past the block limit
            static layer_anim* grow_pool();

            // inlined @ sub_5BA290
            // inlined @ sub_5BA510
            // inlined @ sub_5D3950
            // inlined @ sub_5D3370
            f32 get_duration() const {
                if (nal_inst)
                    return nal_inst->anim->duration;

                if (als_inst)
                    return als_inst->get_duration();

                return 0.0f;
            }

            f32 get_inverse_duration() const;

            // inlined @ sub_5BB5A0
            // inlined @ sub_5BA370
            // inlined @ sub_5D3950
            void set_speed(f32 new_speed) {
                speed = new_speed;

                if (new_speed < 0.0f)
                    speed = 0.0f;
            }

            // inlined @ sub_5BB600
            // inlined @ sub_5BA440
            // inlined @ sub_5D3950
            void set_secondary_speed(f32 new_speed) {
                secondary_speed = new_speed;

                if (new_speed < 0.0f)
                    secondary_speed = 0.0f;
            }

            // inlined @ sub_5B9F60
            // inlined @ sub_5BA140
            bool is_looping() const {
                if (nal_inst)
                    return nal_inst->force_looping || (nal_inst->anim->flags & nal::anim_flag_looping);

                if (als_inst)
                    return als_inst->is_looping();

                return false;
            }

            // the alpha follows the time through the blend
            void set_normalized_time(f32 time);

            void raise_animation_signals(f32 time, f32 prev_time, entity* the_entity);
        };

        // the controller job's input and output; the flush fills it each frame
        struct advance_job {
            struct pose_record {
                layer_anim* anim;
                f32         prev_time;
                f32         time;
                bool        newest;
                u8          pad_0d[0x03];
            };

            pose_record           pose_records[10];
            u32                   num_pose_records;
            animation_controller* controller;       // null skips the job
            bool                  skip_matrices;
            u8                    pad_a9[0x03];
            f32                   delta_t;
            bool                  wants_skip_matrices; // copied to skip_matrices when the controller is queued
            u8                    pad_b1[0x03];
        };

        nal::po                   trajectory;      // the pose job writes the combined pose's trajectory here
        bool                      sync_to_master;
        bool                      is_suspended;    // mirrors the als's
        u8                        pad_022[0x02];
        animation_controller*     master;
        u32                       synced_layer;    // the layer whose newest anim follows the master's
        container::fixed_vector
            <layer_anim*, 2>      layer_anims[15]; // the newest last
        u8                        reserved_0e0[0x04];
        actor*                    the_actor;
        nal::pose                 combined_pose;
        animation_controller*     unk_128;    // set with unk_144 (sub_5ADB20); the jobs skip a controller that has it
        vc8::vector
            <animation_controller*> slaves;     // the controllers following this one; an als with any advances with the high priority ones
        nal::transport_list*      skeleton;     // the actor's m_skeleton
        nal::skeleton_map*        skeleton_map;
        u32                       unk_144;
        advance_job               job;
        u32                       unk_1fc;
        engine_recursive_lock     lock;          // the controller job holds it
        nal::nal_jq_batch_input_t pose_input;    // add_pose_jq_batch fills it for the pose job
        f32                       unk_2f4;       // a hold: the base layer jumps to unk_2f8 seconds, then waits this long
        f32                       unk_2f8;

        bool should_follow_master() const;
    };

    // a layer's playing anims, newest last
    using animation_records = container::fixed_vector<animation_controller_state::layer_anim*, 2>;

    // the transport signals the newest record crossed this frame
    void raise_newest_animation_signals(animation_records* records);

    /* seconds from anim_time to the nearest of the layer's newest anim's signals with the name, or back to it since;
       -1 without one. the first match tells the param, a nearer one replaces it */
    f32 get_time_to_animation_signal   (animation_controller* controller,
                                        u32                   layer,
                                        string_hash           signal_name,
                                        f32                   anim_time,
                                        bool*                 exists,
                                        anim_event_param*     param);
    f32 get_time_since_animation_signal(animation_controller* controller,
                                        u32                   layer,
                                        string_hash           signal_name,
                                        f32                   anim_time,
                                        bool*                 exists,
                                        anim_event_param*     param);

    // raises the anim's signals between the two normalized times on the entity
    void raise_animation_signals_for_time_span(nal::anim* the_anim,
                                               f32        t,
                                               f32        t_prev,
                                               entity*    the_entity,
                                               bool       include_start,
                                               bool       looping);

    class animation_controller {

    public:
        animation_controller_state* state;

        void get_matrix_data_from_pose(bool unk);

        /* fills the state's pose job input, sampling the als's instances itself, and posts it; immediate also runs it and publishes.
           false when the arena can't hold the matrices */
        bool add_pose_jq_batch(scratch_arena* arena, bool immediate);

        // the controller job: playback times, blends, retiring blended-out anims, and the pose records
        static void advance(animation_controller* controller);

        // advance's playback on the spot, raising each layer's newest anim's signals; biped physics runs it
        void frame_advance(f32 delta_t);

        bool                  has_master() const;
        animation_controller* get_master() const;
        bool                  has_slaves() const;
        bool                  is_synced_to_master() const;
        actor*                get_actor() const;
        u32                   get_synced_layer() const; // 0 without a master

        // the anim becomes the layer's newest, cross-fading from the one before over blend_time; a layer keeps two
        void play(const als::nal_als_anim_bridge     &anim,
                        f32                           start_time,
                        f32                           blend_time,
                        u32                           flags,
                        bool                          unk,
                        u32                           layer,
                        als::animation_logic_system*  the_als,
                        als::state_machine*           the_sm,
                        bool                          looping);

        void kill_layer_anims(u32 layer);

        // every layer's newest anim back to unit speeds
        void reset_speeds();

        // the layer's newest anim's; times 0 and the rest 1 without one
        f32  get_anim_duration       (u32 layer);
        f32  get_anim_normalized_time(u32 layer) const;
        f32  get_anim_speed          (u32 layer) const;
        f32  get_secondary_anim_speed(u32 layer) const;
        f32  get_anim_time           (u32 layer);
        bool is_anim_looping         (u32 layer);
        bool has_anim_instance       (u32 layer) const; // the newest has a nal or als instance
        void set_anim_normalized_time(u32 layer, f32 time);
        void set_anim_speed          (u32 layer, f32 speed);
        void set_secondary_anim_speed(u32 layer, f32 speed);

        // inlined @ sub_5BB810
        // inlined @ sub_5E0D20
        f32 get_anim_prev_normalized_time(u32 layer) const {
            return state->layer_anims[layer].size() ? state->layer_anims[layer].back()->prev_normalized_time : 0.0f;
        }

        // the layer's newest anim's nal anim, through its als instance if it has one
        nal::anim* get_anim_ptr(u32 layer);

        // the pose job's trajectory
        po get_trajectory_po() const;

        // where advance will take the layer's newest anim this frame
        f32 get_next_anim_normalized_time(u32 layer);
    };

    namespace references {
        inline util::memory_reference<animation_controller_state::layer_anim*> layer_anim_free_list { 0x00FFCE58 };

        // the pool's blocks: a count and a chain of { block, next } links
        inline util::memory_reference<i32>    layer_anim_block_count { 0x00FFCE98 };
        inline util::memory_reference<void**> layer_anim_blocks      { 0x00FFCED0 };

        // shared by the animation pools
        inline util::memory_reference<engine_recursive_lock> pool_lock { 0x01126D00 };
    } // references

    ASSERT_SIZEOF  (animation_controller_state::layer_anim,                       0x74);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, als_inst,             0x04);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, normalized_time,      0x08);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, prev_normalized_time, 0x0C);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, pose,                 0x10);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, alpha,                0x50);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, blend_start,          0x54);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, alpha_end,            0x60);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, speed,                0x64);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, secondary_speed,      0x68);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, just_started,         0x6C);
    ASSERT_OFFSETOF(animation_controller_state::layer_anim, owner,                0x70);

    ASSERT_SIZEOF  (animation_controller_state::advance_job::pose_record,  0x10);
    ASSERT_OFFSETOF(animation_controller_state::advance_job::pose_record, prev_time, 0x04);
    ASSERT_OFFSETOF(animation_controller_state::advance_job::pose_record, time,      0x08);
    ASSERT_OFFSETOF(animation_controller_state::advance_job::pose_record, newest,    0x0C);

    ASSERT_OFFSETOF(animation_controller_state::advance_job, num_pose_records, 0xA0);
    ASSERT_OFFSETOF(animation_controller_state::advance_job, controller,       0xA4);
    ASSERT_OFFSETOF(animation_controller_state::advance_job, skip_matrices,    0xA8);
    ASSERT_OFFSETOF(animation_controller_state::advance_job, delta_t,          0xAC);
    ASSERT_OFFSETOF(animation_controller_state::advance_job, wants_skip_matrices, 0xB0);

    ASSERT_OFFSETOF(animation_controller_state, sync_to_master, 0x020);
    ASSERT_OFFSETOF(animation_controller_state, is_suspended,   0x021);
    ASSERT_OFFSETOF(animation_controller_state, master,         0x024);
    ASSERT_OFFSETOF(animation_controller_state, synced_layer,   0x028);
    ASSERT_OFFSETOF(animation_controller_state, layer_anims,    0x02C);
    ASSERT_OFFSETOF(animation_controller_state, the_actor,     0x0E4);
    ASSERT_OFFSETOF(animation_controller_state, combined_pose, 0x0E8);
    ASSERT_OFFSETOF(animation_controller_state, unk_128,       0x128);
    ASSERT_OFFSETOF(animation_controller_state, slaves,        0x12C);
    ASSERT_OFFSETOF(animation_controller_state, skeleton,      0x13C);
    ASSERT_OFFSETOF(animation_controller_state, skeleton_map,  0x140);
    ASSERT_OFFSETOF(animation_controller_state, unk_144,       0x144);
    ASSERT_OFFSETOF(animation_controller_state, job,           0x148);
    ASSERT_OFFSETOF(animation_controller_state, lock,          0x200);
    ASSERT_OFFSETOF(animation_controller_state, pose_input,    0x210);
    ASSERT_OFFSETOF(animation_controller_state, unk_2f4,       0x2F4);

    ASSERT_SIZEOF(animation_controller, 0x04);
} // treyarch
