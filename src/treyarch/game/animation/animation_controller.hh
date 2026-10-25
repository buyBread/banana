#pragma once

#include "treyarch/nal/nal.hh"
#include "treyarch/nal/pose.hh"
#include "treyarch/shared/container/fixed_vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"
#include "vc8/vector.hh"

namespace treyarch {
    namespace nal {
        struct anim_instance;
        struct transport_list;
    }

    class animation_controller;
    class entity;

    // a bump allocator over a scratch buffer
    struct scratch_arena {
        u32 size;
        u32 alignment;
        u8* base;
        u8* cursor;
    };

    // what a controller points at; the milestone asserts on it as `state`
    struct animation_controller_state {
        struct unk_02c_entry {
            nal::anim_instance*         nal_inst;   // the als's meta instance samples the pose when this is null
            void*                       als_inst;
            f32                         normalized_time;
            f32                         prev_normalized_time;
            nal::pose                   pose;
            f32                         alpha;
            u8                          reserved_054[0x10];
            f32                         speed;
            f32                         secondary_speed;
            bool                        unk_06c; // raise_animation_signals_for_time_span takes it to include the span's start
            u8                          pad_06d[0x03];
            animation_controller_state* owner;

            void raise_animation_signals(f32 time, f32 prev_time, entity* the_entity);
        };

        // the controller job's input and output; the flush fills it each frame
        struct advance_job {
            struct pose_record {
                unk_02c_entry* entry;
                f32            prev_time;
                f32            time;
                u32            unk_0c;
            };

            pose_record           pose_records[10];
            u32                   num_pose_records;
            animation_controller* controller;       // null skips the job
            bool                  skip_matrices;
            u8                    pad_a9[0x03];
            f32                   delta_t;
            bool                  unk_b0;           // copied to skip_matrices when the controller is queued
            u8                    pad_b1[0x03];
        };

        nal::po                   trajectory;   // the pose job writes the combined pose's trajectory here
        u8                        unk_020;
        bool                      is_suspended; // mirrors the als's
        u8                        reserved_022[0x0A];
        container::fixed_vector
            <unk_02c_entry*, 2>   unk_02c[15];
        u8                        reserved_0e0[0x04];
        entity*                   unk_0e4; // the entity the controller animates
        nal::pose                 combined_pose;
        animation_controller*     unk_128;    // set with unk_144 (sub_5ADB20); the jobs skip a controller that has it
        vc8::vector<void*>        unk_12c;    // an als with something in here advances with the high priority ones
        nal::transport_list*      skeleton;     // the actor's m_skeleton
        void*                     skeleton_map;
        u32                       unk_144;
        advance_job               job;
        u8                        reserved_1fc[0x14];
        nal::nal_jq_batch_input_t pose_input; // add_pose_jq_batch fills it for the pose job
    };

    // a layer's playing records, newest last
    using animation_records = container::fixed_vector<animation_controller_state::unk_02c_entry*, 2>;

    // the transport signals the newest record crossed this frame
    void raise_newest_animation_signals(animation_records* records);

    class animation_controller {

    public:
        animation_controller_state* state;

        void get_matrix_data_from_pose(bool unk);

        /* fills the state's pose job input, sampling the als's instances itself, and posts it; immediate also runs it and publishes.
           false when the arena can't hold the matrices */
        bool add_pose_jq_batch(scratch_arena* arena, bool immediate);

        // the controller job: playback times, envelopes and the pose records
        static void advance(animation_controller* controller);
    };

    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, als_inst,             0x04);
    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, normalized_time,      0x08);
    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, prev_normalized_time, 0x0C);
    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, pose,                 0x10);
    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, alpha,                0x50);
    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, speed,                0x64);
    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, secondary_speed,      0x68);
    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, unk_06c,              0x6C);
    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, owner,                0x70);

    ASSERT_SIZEOF  (animation_controller_state::advance_job::pose_record,  0x10);
    ASSERT_OFFSETOF(animation_controller_state::advance_job::pose_record, prev_time, 0x04);
    ASSERT_OFFSETOF(animation_controller_state::advance_job::pose_record, time,      0x08);

    ASSERT_OFFSETOF(animation_controller_state::advance_job, num_pose_records, 0xA0);
    ASSERT_OFFSETOF(animation_controller_state::advance_job, controller,       0xA4);
    ASSERT_OFFSETOF(animation_controller_state::advance_job, skip_matrices,    0xA8);
    ASSERT_OFFSETOF(animation_controller_state::advance_job, delta_t,          0xAC);
    ASSERT_OFFSETOF(animation_controller_state::advance_job, unk_b0,           0xB0);

    ASSERT_OFFSETOF(animation_controller_state, unk_020,       0x020);
    ASSERT_OFFSETOF(animation_controller_state, is_suspended,  0x021);
    ASSERT_OFFSETOF(animation_controller_state, unk_02c,       0x02C);
    ASSERT_OFFSETOF(animation_controller_state, unk_0e4,       0x0E4);
    ASSERT_OFFSETOF(animation_controller_state, combined_pose, 0x0E8);
    ASSERT_OFFSETOF(animation_controller_state, unk_128,       0x128);
    ASSERT_OFFSETOF(animation_controller_state, unk_12c,       0x12C);
    ASSERT_OFFSETOF(animation_controller_state, skeleton,      0x13C);
    ASSERT_OFFSETOF(animation_controller_state, skeleton_map,  0x140);
    ASSERT_OFFSETOF(animation_controller_state, unk_144,       0x144);
    ASSERT_OFFSETOF(animation_controller_state, job,           0x148);
    ASSERT_OFFSETOF(animation_controller_state, pose_input,    0x210);

    ASSERT_SIZEOF(animation_controller, 0x04);
} // treyarch
