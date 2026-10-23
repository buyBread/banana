#pragma once

#include "treyarch/amalga/merged_apk/file.hh"
#include "treyarch/jq/jq.hh"
#include "treyarch/nal/anim.hh"
#include "treyarch/nal/anim_instance.hh"
#include "treyarch/nal/pose.hh"
#include "treyarch/nal/scene_anim.hh"
#include "treyarch/nal/transport_list.hh"
#include "treyarch/shared/fixed_string.hh"
#include "treyarch/shared/math/vmath/matrix.hh"
#include "util/macros/sanity_assert.hh"
#include "util/memory_reference.hh"
#include "util/types.hh"

namespace treyarch { namespace nal {
    constexpr u32 max_batch_poses = 10;

    // filled by the animation controller's add_pose_jq_batch
    struct nal_jq_batch_input_t {
        nal::pose*           combined_pose;
        f32                  t_prev[max_batch_poses];
        f32                  t[max_batch_poses];
        nal::pose*           poses[max_batch_poses];
        f32                  alphas[max_batch_poses];     // alphas[0] is ignored
        u32                  pose_count;
        nal::anim_instance*  instances[max_batch_poses];  // null when the pose is already filled
        nal::po*             trajectory;
        bool                 skip_matrices;
        math::mat43*         result_matrices;
        u32                  result_matrix_count;
        nal::transport_list* transport_list;
    };

    ASSERT_SIZEOF  (nal_jq_batch_input_t,                      0xE4);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, t_prev,              0x04);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, t,                   0x2C);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, poses,               0x54);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, alphas,              0x7C);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, pose_count,          0xA4);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, instances,           0xA8);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, trajectory,          0xD0);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, skip_matrices,       0xD4);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, result_matrices,     0xD8);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, result_matrix_count, 0xDC);
    ASSERT_OFFSETOF(nal_jq_batch_input_t, transport_list,      0xE0);

    void init();

    void nal_jq_callback(jq::batch* batch);

    transport_list* get_skeleton(fixed_string* name);
    
    anim*       get_anim(fixed_string* name);
    scene_anim* get_scene_anim(fixed_string* name);

    void load_anim(amalga::merged_apk::file*       owner,
                   amalga::merged_apk::file_entry* entry,
                   void**                          mapped_sections,
                   void*                           user_data);

    void remove_anim(amalga::merged_apk::file*       owner,
                     amalga::merged_apk::file_entry* entry,
                     void**                          mapped_sections,
                     void*                           user_data);

    void load_scene_anim(amalga::merged_apk::file*       owner,
                         amalga::merged_apk::file_entry* entry,
                         void**                          mapped_sections,
                         void*                           user_data);

    namespace references {
        inline util::memory_reference<bool> skip_linking            { 0x01115B68 };
        inline util::memory_reference<bool> skip_scene_anim_linking { 0x01115B69 };
    } // references
}} // treyarch::nal
