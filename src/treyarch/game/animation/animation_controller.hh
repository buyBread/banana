#pragma once

#include "treyarch/nal/nal.hh"
#include "treyarch/nal/pose.hh"
#include "treyarch/shared/container/fixed_vector.hh"
#include "util/macros/sanity_assert.hh"
#include "util/types.hh"

namespace treyarch {
    // what a controller points at; the milestone asserts on it as `state`
    struct animation_controller_state {
        struct unk_02c_entry {
            u8   reserved_000[0x6C];
            bool unk_06c; // raise_animation_signals_for_time_span takes it to include the span's start
        };

        u8                        reserved_000[0x2C];
        container::fixed_vector
            <unk_02c_entry*, 2>   unk_02c[15];
        u8                        reserved_0e0[0x08];
        nal::pose                 combined_pose;
        u8                        reserved_128[0xE8];
        nal::nal_jq_batch_input_t pose_input; // add_pose_jq_batch fills it for the pose job
    };

    class animation_controller {

    public:
        animation_controller_state* state;

        void get_matrix_data_from_pose(bool unk);
    };

    ASSERT_OFFSETOF(animation_controller_state::unk_02c_entry, unk_06c, 0x6C);

    ASSERT_OFFSETOF(animation_controller_state, unk_02c,       0x02C);
    ASSERT_OFFSETOF(animation_controller_state, combined_pose, 0x0E8);
    ASSERT_OFFSETOF(animation_controller_state, pose_input,    0x210);

    ASSERT_SIZEOF(animation_controller, 0x04);
} // treyarch
